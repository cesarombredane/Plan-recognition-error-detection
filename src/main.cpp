#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctime>

#include "extendedPlanLibrary.h"
#include "solver.h"
#include "CognitiveDistressManagement.h"

using namespace std;

void printResult(map<string, map<int, map<float, vector<int>>>> &result, int nbLib, int nbIt, int libSize, bool AB) {
    cout << "result based on " << nbLib*nbIt << " tests (" << nbLib << " different plan library) for each scenario." << endl;
    cout << "solver nbParticles set on 1000." << endl;
    cout << "library size set on " << libSize << " with " << (AB ? "" : "no ") << "AnormalBehavior." << endl << endl;

    for (auto& method : result) {
        cout << method.first << " : " << endl << endl;

        for (auto& precision : method.second) {
            if (precision.first != -1) cout << "precision : " << precision.first << endl;
            cout << "noisePrediction\tP(effective)\tP(falsePositive)\tP(falseNegative)\tP(GoalFound)\tMoy(latency)" << endl;

            for (auto& noise : precision.second)
                cout << setprecision(4) << noise.first << "\t" << noise.second[0] << "\t\t" << noise.second[1] <<
                     "\t\t\t" << noise.second[2] << "\t\t\t" << noise.second[3] << "\t\t" << noise.second[4] << endl;
        }

        cout << endl;
    }
}

vector<int> testMinCDM(extendedPlanLibrary* ePl, bool distress) {
    vector<int> result;

    solver planGenerator = solver(ePl, 1);
    pair<int,vector<int>> curPlan = planGenerator.generatePlan();

    solver s = solver(ePl, 1000);
    minCDM cdm = minCDM(&s, ePl);

    random_device seeder;
    mt19937 engine(seeder());
    uniform_int_distribution<> distressPointRandomizer(0, (int) curPlan.second.size() - 1);
    uniform_int_distribution<> actionIdRandomizer(0, (int) ePl->ePlanLibrary->getTerminals().size() - 1);

    int distressPoint;
    if (distress) distressPoint = distressPointRandomizer(engine);
    else distressPoint = (int) curPlan.second.size();

    for(int i = distressPoint; i < curPlan.second.size(); i++) {
        int newActionID = actionIdRandomizer(engine);
        auto iter = ePl->ePlanLibrary->getTerminals().begin();
        for(int j = 0; j < newActionID; j++) iter++;
        curPlan.second[i] = *(iter);
    }

    result.push_back(distress); // 1 if distress
    result.push_back(0); // 1 if distress found
    result.push_back(0); // latency

    int found = 0;
    for(int obs = 0; obs < curPlan.second.size(); obs++) {
        if(s.getMaxGoal() == curPlan.first && found == 0) found = 1;

        s.addObservation(curPlan.second[obs]);
        cdm.update();

        if(cdm.getAB()) {
            result[1] = 1;
            result[2] = obs - distressPoint;
            break;
        }
    }

    result.push_back(found); // 1 if plan found
    return result;
}

vector<int> testMaxCDM(extendedPlanLibrary* ePl, double precision, bool distress) {
    vector<int> result;

    solver planGenerator = solver(ePl, 1);
    pair<int,vector<int>> curPlan = planGenerator.generatePlan();

    solver s = solver(ePl, 1000);
    sumCDM cdm = sumCDM(&s, ePl, precision);

    random_device seeder;
    mt19937 engine(seeder());
    uniform_int_distribution<> distressPointRandomizer(0, (int) curPlan.second.size() - 1);
    uniform_int_distribution<> actionIdRandomizer(0, (int) ePl->ePlanLibrary->getTerminals().size() - 1);

    int distressPoint;
    if (distress) distressPoint = distressPointRandomizer(engine);
    else distressPoint = (int) curPlan.second.size();

    for(size_t i = distressPoint; i < curPlan.second.size(); i++) {
        int newActionID = actionIdRandomizer(engine);
        auto iter = ePl->ePlanLibrary->getTerminals().begin();
        for(int j = 0; j < newActionID; j++) iter++;
        curPlan.second[i] = *(iter);
    }

    result.push_back(distress);
    result.push_back(0);
    result.push_back(0);

    int found = 0;
    for(int obs = 0; obs < curPlan.second.size(); obs++) {
        if(s.getMaxGoal() == curPlan.first && found == 0) found = 1;

        s.addObservation(curPlan.second[obs]);
        cdm.update();

        if(cdm.getAB()) {
            result[1] = 1;
            result[2] = obs - distressPoint;
            break;
        }
    }

    result.push_back(found);
    return result;
}

vector<int> testSupportCDM(extendedPlanLibrary* ePl, double precision, bool distress) {
    vector<int> result;

    solver planGenerator = solver(ePl, 1);
    pair<int,vector<int>> curPlan = planGenerator.generatePlan();

    solver s = solver(ePl, 1000);
    supportCDM cdm = supportCDM(&s, ePl, precision);

    random_device seeder;
    mt19937 engine(seeder());
    uniform_int_distribution<> distressPointRandomizer(0, (int) curPlan.second.size() - 1);
    uniform_int_distribution<> actionIdRandomizer(0, (int) ePl->ePlanLibrary->getTerminals().size() - 1);

    int distressPoint;
    if (distress) distressPoint = distressPointRandomizer(engine);
    else distressPoint = (int) curPlan.second.size();

    for(size_t i = distressPoint; i < curPlan.second.size(); i++) {
        int newActionID = actionIdRandomizer(engine);
        auto iter = ePl->ePlanLibrary->getTerminals().begin();
        for(int j = 0; j < newActionID; j++) iter++;
        curPlan.second[i] = *(iter);
    }

    result.push_back(distress);
    result.push_back(0);
    result.push_back(0);

    int found = 0;
    for(int obs = 0; obs < curPlan.second.size(); obs++) {
        if(s.getMaxGoal() == curPlan.first && found == 0) found = 1;

        s.addObservation(curPlan.second[obs]);
        cdm.update();

        if(cdm.getAB()) {
            result[1] = 1;
            result[2] = obs - distressPoint;
            break;
        }
    }

    result.push_back(found);
    return result;
}

void testAllCDM(bool distress, int nbLib, int nbIt, int nbAction) {
    map<string, map<int, map<float, vector<int>>>> result;

    cout << "CALCULATION : distress = " << distress << " | nbAction = " << nbAction << " | nbLib = " << nbLib << " | nbIt = " << nbIt << endl;

    cout << "min calculation methode ..." << endl;
    for (int noise = 0; noise < 4; ++noise) {
        float noiseRatio = (float) noise / 10;
        int anormalBehaviorFound = 0;
        int falseNegative = 0;
        int falsePositive = 0;
        int noAnormalBehaviorFound = 0;
        int latency = 0;
        int foundGoal = 0;

        for (int lib = 0; lib < nbLib; lib++) {
            extendedPlanLibrary ePl = extendedPlanLibrary(noiseRatio, 5, nbAction, 2, 3, 2);

            for (int it = 0; it < nbIt; it++) {
                vector<int> resultMin = testMinCDM(&ePl, distress);

                foundGoal += resultMin[3];
                if (resultMin[0] == 0 && resultMin[1] == 0) noAnormalBehaviorFound++;
                else if ((resultMin[0] == 0 && resultMin[1] == 1) || (resultMin[2] < 0)) falsePositive++;
                else if (resultMin[0] == 1 && resultMin[1] == 0) falseNegative++;
                else {
                    anormalBehaviorFound++;
                    latency += resultMin[2];
                }
            }
        }

        result["min"][-1][noiseRatio].push_back((anormalBehaviorFound + noAnormalBehaviorFound) / (nbLib * nbIt));
        result["min"][-1][noiseRatio].push_back(falsePositive / (nbLib * nbIt));
        result["min"][-1][noiseRatio].push_back(falseNegative / (nbLib * nbIt));
        result["min"][-1][noiseRatio].push_back(foundGoal / (nbLib * nbIt));
        if (anormalBehaviorFound != 0) result["min"][-1][noiseRatio].push_back(latency / anormalBehaviorFound);
        else result["min"][-1][noiseRatio].push_back(0);
    }

    cout << "sum calculation methode ..." << endl;
    for (int noise = 0; noise < 4; ++noise) {
        float noiseRatio = (float) noise / 10;
        vector<int> precisionOptions{1, 3, 5, 10, 15, 20};

        for (auto& precision : precisionOptions) {
            int anormalBehaviorFound = 0;
            int falseNegative = 0;
            int falsePositive = 0;
            int noAnormalBehaviorFound = 0;
            int latency = 0;
            int foundGoal = 0;

            for (int lib = 0; lib < nbLib; lib++) {
                extendedPlanLibrary ePl = extendedPlanLibrary(noiseRatio, 5, nbAction, 2, 3, 2);

                for (int it = 0; it < nbIt; it++) {
                    vector<int> resultSum = testMaxCDM(&ePl, precision, distress);

                    foundGoal += resultSum[3];
                    if (resultSum[0] == 0 && resultSum[1] == 0) noAnormalBehaviorFound++;
                    else if ((resultSum[0] == 0 && resultSum[1] == 1) || (resultSum[2] < 0)) falsePositive++;
                    else if (resultSum[0] == 1 && resultSum[1] == 0) falseNegative++;
                    else {
                        anormalBehaviorFound++;
                        latency += resultSum[2];
                    }
                }
            }

            result["sum"][precision][noiseRatio].push_back((anormalBehaviorFound + noAnormalBehaviorFound) / (nbLib * nbIt));
            result["sum"][precision][noiseRatio].push_back(falsePositive / (nbLib * nbIt));
            result["sum"][precision][noiseRatio].push_back(falseNegative / (nbLib * nbIt));
            result["sum"][precision][noiseRatio].push_back(foundGoal / (nbLib * nbIt));
            if (anormalBehaviorFound != 0) result["sum"][precision][noiseRatio].push_back(latency / anormalBehaviorFound);
            else result["sum"][precision][noiseRatio].push_back(0);
        }
    }

    cout << "support calculation methode ..." << endl;
    for (int noise = 0; noise < 4; ++noise) {
        float noiseRatio = (float) noise / 10;
        for (int precision = 1; precision < 7; precision += 2) {
            int anormalBehaviorFound = 0;
            int falseNegative = 0;
            int falsePositive = 0;
            int noAnormalBehaviorFound = 0;
            int latency = 0;
            int foundGoal = 0;

            for (int lib = 0; lib < nbLib; lib++) {
                extendedPlanLibrary ePl = extendedPlanLibrary(noiseRatio, 5, nbAction, 2, 3, 2);

                for (int it = 0; it < nbIt; it++) {
                    vector<int> resultSupport = testSupportCDM(&ePl, precision, distress);

                    foundGoal += resultSupport[3];
                    if (resultSupport[0] == 0 && resultSupport[1] == 0) noAnormalBehaviorFound++;
                    else if ((resultSupport[0] == 0 && resultSupport[1] == 1) || (resultSupport[2] < 0)) falsePositive++;
                    else if (resultSupport[0] == 1 && resultSupport[1] == 0) falseNegative++;
                    else {
                        anormalBehaviorFound++;
                        latency += resultSupport[2];
                    }
                }
            }

            result["support"][precision][noiseRatio].push_back((anormalBehaviorFound + noAnormalBehaviorFound) / (nbLib * nbIt));
            result["support"][precision][noiseRatio].push_back(falsePositive / (nbLib * nbIt));
            result["support"][precision][noiseRatio].push_back(falseNegative / (nbLib * nbIt));
            result["support"][precision][noiseRatio].push_back(foundGoal / (nbLib * nbIt));
            if (anormalBehaviorFound != 0) result["support"][precision][noiseRatio].push_back(latency / anormalBehaviorFound);
            else result["support"][precision][noiseRatio].push_back(0);
        }
    }

    printResult(result, nbLib, nbIt, nbAction, distress);
}

int main() {
    extendedPlanLibrary ePl = extendedPlanLibrary();
    cout << ePl.toString() << endl;
    testAllCDM(true, 1, 1, 10);

    return 0;
}
