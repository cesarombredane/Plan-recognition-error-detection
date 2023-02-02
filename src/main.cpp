#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctime>
#include "extendedPlanLibrary.h"
#include "solver.h"
#include "CognitiveDistressManagement.h"

using namespace std;

void showRes( map<string, map<int, map<float, vector<int>>>>& result, int nbLib, int nbIt, int libSize, bool AB, const string& resultPath) {
    cout << "result based on " << nbLib*nbIt << " tests (" << nbLib << " differents planLibrarys) for each scenario." << endl;
    cout << "solver nbParticles set on 1000." << endl;
    cout << "library size set on " << libSize << " with ";

    if (!AB) cout << "no ";

    cout << "AnormalBehavior." << endl << endl;

    for (auto& itMethod : result) {
        cout << itMethod.first << " : " << endl << endl;

        for (auto& itPrec : itMethod.second) {
            if (itPrec.first != -1) cout << "precision : " << itPrec.first << endl;

            cout << "noisePrediction\tP(effective)\tP(falsePositive)\tP(falseNegative)\tP(GoalFound)\tMoy(latency)" << endl;

            for (auto& itNoise : itPrec.second)
                cout << setprecision (4) << itNoise.first << "\t" << itNoise.second[0] << "\t\t" << itNoise.second[1] << "\t\t\t" << itNoise.second[2]
                        << "\t\t\t" << itNoise.second[3] << "\t\t" << itNoise.second[4] << endl;
        }

        cout << endl;
    }
}

vector<int> TestCdmMin(extendedPlanLibrary* ePl, bool distress) {
    vector<int> result;

    solver gP = solver(ePl, 1);
    pair<int,vector<int>> curPlan = gP.generatePlan();
    solver s = solver(ePl, 1000);

    CDMMin CDM = CDMMin(&s, ePl);

    size_t DistressPoint; // set up of distress point
    if (distress) DistressPoint = rand() % curPlan.second.size();
    else DistressPoint = curPlan.second.size();

    for(size_t i = DistressPoint; i < curPlan.second.size(); i++) {
        int newActionID = rand() % ePl->ePlanLibrary->getTerminals().size();
        auto iter = ePl->ePlanLibrary->getTerminals().begin();

        for(int j = 0; j < newActionID; j++) iter++;
        curPlan.second[i] = *(iter);
    }

    result.push_back(distress); // 1 if distress
    result.push_back(0); // 1 if distress found
    result.push_back(0); // latency

    int found = 0;
    for(size_t obs = 0; obs < curPlan.second.size(); obs++) {
        if(s.getMaxGoal() == curPlan.first && found == 0) found = 1;

        s.addObservation(curPlan.second[obs]);
        CDM.update();

        if(CDM.getAB()) {
            result[1] = 1;
            result[2] = obs - DistressPoint;

            break;
        }
    }

    result.push_back(found); // 1 if plan found
    return result;
}

vector<int> TestCdmSum(extendedPlanLibrary* ePl, double prec, bool distress) {
    vector<int> result;

    solver gP = solver(ePl, 1);
    pair<int,vector<int>> curPlan = gP.generatePlan();
    solver s = solver(ePl, 1000);

    CDMSum CDM = CDMSum(&s, ePl, prec);

    size_t DistressPoint;
    if (distress) DistressPoint = rand () % curPlan.second.size();
    else DistressPoint = curPlan.second.size();

    for(size_t i = DistressPoint; i < curPlan.second.size(); i++) {
        int newActionID = rand() % ePl->ePlanLibrary->getTerminals().size();
        auto iter = ePl->ePlanLibrary->getTerminals().begin();

        for(int j = 0; j < newActionID; j++) iter++;

        curPlan.second[i] = *(iter);
    }

    result.push_back(distress);
    result.push_back(0);
    result.push_back(0);

    int found = 0;
    for(size_t obs = 0; obs < curPlan.second.size(); obs++) {
        if(s.getMaxGoal() == curPlan.first && found == 0) found = 1;

        s.addObservation(curPlan.second[obs]);
        CDM.update();

        if(CDM.getAB()) {
            result[1] = 1;
            result[2] = obs - DistressPoint;

            break;
        }
    }

    result.push_back(found);
    return result;
}

vector<int> TestCdmSupport(extendedPlanLibrary* ePl, double prec, bool distress) {
    vector<int> result;

    solver gP = solver(ePl, 1);
    pair<int,vector<int>> curPlan = gP.generatePlan();
    solver s = solver(ePl, 1000);

    CDMSupport CDM = CDMSupport(&s, ePl, prec);

    size_t DistressPoint;
    if (distress) DistressPoint = rand () % curPlan.second.size();
    else DistressPoint = curPlan.second.size();

    for(size_t i = DistressPoint; i < curPlan.second.size(); i++) {
        int newActionID = rand() % ePl->ePlanLibrary->getTerminals().size();
        auto iter = ePl->ePlanLibrary->getTerminals().begin();

        for(int j = 0; j < newActionID; j++) iter++;

        curPlan.second[i] = *(iter);
    }

    result.push_back(distress);
    result.push_back(0);
    result.push_back(0);

    int found = 0;
    for(size_t obs = 0; obs < curPlan.second.size(); obs++) {
        if(s.getMaxGoal() == curPlan.first && found == 0) found = 1;

        s.addObservation(curPlan.second[obs]);
        CDM.update();

        if(CDM.getAB()) {
            result[1] = 1;
            result[2] = obs - DistressPoint;

            break;
        }
    }

    result.push_back(found);
    return result;
}

/*!
 *  \brief full set of tests on all three methodes
 *
 *  \param distress : with or without annormal behavior
 *  \param nbLib : number of plan library used in the test
 *  \param nbIt : number of iteration on each plan library
 *  \param nbAction : size of plan library
 *  \param resultPath : path to result.txt
 */
void FullTestCdm(bool distress, int nbLib, int nbIt, int nbAction, const string& resultPath) {
    map<string, map<int, map<float, vector<int>>>> result;
    cout << "CALCULATION : distress = " << distress << " | nbAction = " << nbAction << " | nbLib = " << nbLib << " | nbIt = " << nbIt << endl;

    cout << "min calculation methode ..." << endl;
    for (float noise = 0.0; noise < 0.4; noise += 0.1) { // variation of noisePrediction
        int ABfound = 0;
        int ABnotFound = 0;
        int notABfound = 0;
        int notABnotFound = 0;
        int latency = 0;
        int GoalFound = 0;

        for (int lib = 0; lib < nbLib; lib++) {
            extendedPlanLibrary ePl = extendedPlanLibrary(noise, 5, nbAction, 2, 3, 2);

            for (int it = 0; it < nbIt; it++) {
                vector<int> resultMin = TestCdmMin(&ePl, distress); // test

                // gestion of results
                GoalFound += resultMin[3];
                if (resultMin[0] == 0 && resultMin[1] == 0) notABnotFound++;
                else if ((resultMin[0] == 0 && resultMin[1] == 1) || (resultMin[2] < 0)) notABfound++;
                else if (resultMin[0] == 1 && resultMin[1] == 0) ABnotFound++;
                else {
                    ABfound++;
                    latency += resultMin[2];
                }
            }
        }

        result["min"][-1][noise].push_back((ABfound + notABnotFound) / (nbLib * nbIt));
        result["min"][-1][noise].push_back(notABfound / (nbLib * nbIt));
        result["min"][-1][noise].push_back(ABnotFound / (nbLib * nbIt));
        result["min"][-1][noise].push_back(GoalFound / (nbLib * nbIt));
        if (ABfound != 0) result["min"][-1][noise].push_back(latency / ABfound);
        else result["min"][-1][noise].push_back(0);
    }

    cout << "sum calculation methode ..." << endl;
    for (float noise = 0.0; noise < 0.4; noise += 0.1) {
        vector<int> precs{1, 3, 5, 10, 15, 20};

        for (auto& prec : precs) { // variation of precision
            int ABfound = 0;
            int ABnotFound = 0;
            int notABfound = 0;
            int notABnotFound = 0;
            int latency = 0;
            int GoalFound = 0;

            for (int lib = 0; lib < nbLib; lib++) {
                extendedPlanLibrary ePl = extendedPlanLibrary(noise, 5, nbAction, 2, 3, 2);

                for (int it = 0; it < nbIt; it++) {
                    vector<int> resultSum = TestCdmSum(&ePl, prec, distress);

                    GoalFound += resultSum[3];
                    if (resultSum[0] == 0 && resultSum[1] == 0) notABnotFound++;
                    else if ((resultSum[0] == 0 && resultSum[1] == 1) || (resultSum[2] < 0)) notABfound++;
                    else if (resultSum[0] == 1 && resultSum[1] == 0) ABnotFound++;
                    else {
                        ABfound++;
                        latency += resultSum[2];
                    }
                }
            }

            result["sum"][prec][noise].push_back((ABfound + notABnotFound) / (nbLib * nbIt));
            result["sum"][prec][noise].push_back(notABfound / (nbLib * nbIt));
            result["sum"][prec][noise].push_back(ABnotFound / (nbLib * nbIt));
            result["sum"][prec][noise].push_back(GoalFound / (nbLib * nbIt));
            if (ABfound != 0) result["sum"][prec][noise].push_back(latency / ABfound);
            else result["sum"][prec][noise].push_back(0);
        }
    }

    cout << "support calculation methode ..." << endl;
    for (float noise = 0.0; noise < 0.4; noise += 0.1) {
        for (int prec = 1; prec < 7; prec += 2) {
            int ABfound = 0;
            int ABnotFound = 0;
            int notABfound = 0;
            int notABnotFound = 0;
            int latency = 0;
            int GoalFound = 0;

            for (int lib = 0; lib < nbLib; lib++) {
                extendedPlanLibrary ePl = extendedPlanLibrary(noise, 5, nbAction, 2, 3, 2);

                for (int it = 0; it < nbIt; it++) {
                    vector<int> resultSupport = TestCdmSupport(&ePl, prec, distress);

                    GoalFound += resultSupport[3];
                    if (resultSupport[0] == 0 && resultSupport[1] == 0) notABnotFound++;
                    else if ((resultSupport[0] == 0 && resultSupport[1] == 1) || (resultSupport[2] < 0)) notABfound++;
                    else if (resultSupport[0] == 1 && resultSupport[1] == 0) ABnotFound++;
                    else {
                        ABfound++;
                        latency += resultSupport[2];
                    }
                }
            }

            result["support"][prec][noise].push_back((ABfound + notABnotFound) / (nbLib * nbIt));
            result["support"][prec][noise].push_back(notABfound / (nbLib * nbIt));
            result["support"][prec][noise].push_back(ABnotFound / (nbLib * nbIt));
            result["support"][prec][noise].push_back(GoalFound / (nbLib * nbIt));
            if (ABfound != 0) result["support"][prec][noise].push_back(latency / ABfound);
            else result["support"][prec][noise].push_back(0);
        }
    }

    cout << endl;
    showRes(result, nbLib, nbIt, nbAction, distress, resultPath);
}

/*!
 *  \brief a set of tests on support or sum methode to find the optimal precision
 *
 *  \param nbGoal : number of goals in the plan library
 *  \param nbAction : number of terminals actions in the plan library
 *  \param strPath : path to result.txt
 */
void testPrecSupport(int nbGoal, int nbAction, const string& strPath) {
    ofstream file(strPath);
    int iteration = 0;

    if (file) {
        file << "nbGoal = " << nbGoal << endl << "nbAction = " << nbAction << endl;
        file << "prec\tnoisePrediction\tefficiency" << endl << endl;
    }

    for (double prec = 0.0; prec <= 5.0; prec+= 0.1) {
        for (float noise = 0.0; noise <= 0.3; noise += 0.01) {
            float right = 0;
            int pc = 0;

            for (size_t lib = 0; lib < 10; lib++) {
                extendedPlanLibrary ePl = extendedPlanLibrary(noise, nbGoal, nbAction, 2, 3, 2);

                for (size_t it = 0; it < 10; it++) {
                    //vector<int> result = TestCdmSum(&ePl, prec, true);
                    vector<int> result = TestCdmSupport(&ePl, prec, true);

                    if (result[1] == 1 && result[2] >= 0)
                        right++;

                    pc++;
                    cout << pc << "% - " << iteration+1 << "/1500" << endl;
                }
            }

            if (file) {
                file << prec << "\t" << noise << "\t" << (right / 100) << endl;
            }
            iteration++;
        }
    }
}

void testPrecSum(int nbGoal, int nbAction, const string& strPath) {
    ofstream file(strPath);
    int iteration = 0;

    if (file) {
        file << "nbGoal = " << nbGoal << endl << "nbAction = " << nbAction << endl;
        file << "prec\tnoisePrediction\tefficiency" << endl << endl;
    }

    for (double prec = 0.0; prec <= 5.0; prec+= 0.1) {
        for (float noise = 0.0; noise <= 0.3; noise += 0.01) {
            float right = 0;
            int pc = 0;

            for (size_t lib = 0; lib < 10; lib++) {
                extendedPlanLibrary ePl = extendedPlanLibrary(noise, nbGoal, nbAction, 2, 3, 2);

                for (size_t it = 0; it < 10; it++) {
                    vector<int> result = TestCdmSum(&ePl, prec, true);

                    if (result[1] == 1 && result[2] >= 0)
                        right++;

                    pc++;
                    cout << pc << "% - " << iteration+1 << "/1500" << endl;
                }
            }

            if (file) {
                file << prec << "\t" << noise << "\t" << (right / 100) << endl;
            }
            iteration++;
        }
    }
}

int main() {
    // extendedPlanLibrary(float expectedNoise, int nbGoal, int nbTerminals, int treeDepth, int nbChildPerRule, int nbPlanPerNonTerminal)
    extendedPlanLibrary ePl = extendedPlanLibrary();
    cout << ePl.toString() << endl;
    FullTestCdm(true, 1, 1, 10, "./out/resultTrue10.txt");

    /*
    FullTestCdm(true, 100, 10, 10, "./out/resultTrue10.txt");
    FullTestCdm(true, 100, 10, 100, "./out/resultTrue100.txt");
    FullTestCdm(false, 100, 10, 10, "./out/resultFalse10.txt");
    FullTestCdm(false, 100, 10, 100, "./out/resultFalse100.txt");

    testPrecSum(5, 10, "./out/resultPrecSum-5-10.txt");
    testPrecSum(10, 10, "./out/resultPrecSum-10-10.txt");
    testPrecSum(20, 10, "./out/resultPrecSum-20-10.txt");
    testPrecSum(5, 45, "./out/resultPrecSum-5-45.txt");
    testPrecSum(5, 100, "./out/resultPrecSum-5-100.txt");

    testPrecSupport(5, 10, "./out/resultPrecSupport-5-10.txt");
    testPrecSupport(10, 10, "./out/resultPrecSupport-10-10.txt");
    testPrecSupport(20, 10, "./out/resultPrecSupport-20-10.txt");
    testPrecSupport(5, 45, "./out/resultPrecSupport-5-45.txt");
    testPrecSupport(5, 100, "./out/resultPrecSupport-5-100.txt");
     */

    return 0;
}
