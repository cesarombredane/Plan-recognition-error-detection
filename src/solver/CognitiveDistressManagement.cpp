#include "CognitiveDistressManagement.h"


// ----------[CognitiveDistressManagement]----------
// constructor
CognitiveDistressManagement::CognitiveDistressManagement() = default;
CognitiveDistressManagement::CognitiveDistressManagement(solver * _s, extendedPlanLibrary* _epl) {
    AnormalBehavior = false;
    proba = 0.0;
    alpha = 0.0;
    s = _s;
    ePL = _epl;
    oldParticles = s->particles;
}

// destructor
CognitiveDistressManagement::~CognitiveDistressManagement() = default;

// functions
bool CognitiveDistressManagement::getAB() const {
    return AnormalBehavior;
}
map<int,float> CognitiveDistressManagement::getGoals() {
    map<int,float> result;

    for(const auto& it : oldParticles)
        for(const auto& itPar : it.second) {
            if(result.count(itPar.goal) > 0) result[itPar.goal]+= 1 / (float)s->nbParticles;
            else result[itPar.goal] = 1 / (float)s->nbParticles;
        }

    return result;
}
map<int,float> CognitiveDistressManagement::getActions() {
    map<int,float> result;

    for(auto it : ePL->noisePrediction.getDistribution()) {
        result[it.first] = (float) (it.second[s->getLastObservation()] * (1.0 / (float) ePL->noisePrediction.getDistribution().size()));
        float dem = 0.0;

        for(auto it2 : it.second) dem += (float) (it2.second * (1.0 / (float) ePL->noisePrediction.getDistribution().size()));

        if(dem == 0.0) result[it.first] = 0.0;
        else result[it.first] = result[it.first] / dem;
    }

    return result;
}
void CognitiveDistressManagement::update() {
    updateAlpha();

    double temp = proba;
    proba = temp + alpha*(1-temp);

    updateAnormalBehavior();

    if (!AnormalBehavior) oldParticles = s->particles;
}
map<int,set<int>> CognitiveDistressManagement::filterAAlpha() {
    map<int,set<int>> result;
    map<int,set<int>> nextPossibleActions;
    set<int> allActions(ePL->ePlanLibrary->getTerminals().begin(),  ePL->ePlanLibrary->getTerminals().end());

    for(auto it : ePL->ePlanLibrary->getGoals()) result[it] = allActions;

    vector<solverParticle> allParticles;

    for(auto it : oldParticles) allParticles.insert(allParticles.end(),it.second.begin(),it.second.end());
    for(const auto& it : allParticles) nextPossibleActions[it.goal].insert(it.planTree.currentTerminalFO);
    for(const auto& it :nextPossibleActions) for(auto itNPA2 : it.second) result[it.first].erase(result[it.first].find(itNPA2));

    return result;
}

// ----------[minCDM]----------
// constructor
minCDM::minCDM(solver * _s, extendedPlanLibrary* _epl) : CognitiveDistressManagement(_s, _epl) {}

// destructor
minCDM::~minCDM() = default;

// functions
void minCDM::updateAlpha() {
    alpha = 1.0;
    map<int,float> G = getGoals();
    map<int,float> A = getActions();
    map<int,set<int>> fAlpha = filterAAlpha();

    for(auto it : G) for(auto it2 : fAlpha[it.first])
        if(A.count(it2) > 0) alpha = min((float) alpha, it.second*A[it2]);
}
void minCDM::updateAnormalBehavior() {
    if (proba > 0.5) AnormalBehavior = true;
}

// ----------[sumCDM]----------
// constructor
sumCDM::sumCDM(solver * _s, extendedPlanLibrary* _epl, double _precision) : CognitiveDistressManagement(_s, _epl), precision(_precision) {}

// destructor
sumCDM::~sumCDM() = default;

// functions
void sumCDM::updateAlpha() {
    alpha = 0.0;
    map<int,float> G = getGoals();
    map<int,float> A = getActions();
    map<int,set<int>> fAlpha = filterAAlpha();

    for(auto it : G) for(auto it2 : fAlpha[it.first])
        if(A.count(it2) > 0) alpha += it.second*A[it2];
}
void sumCDM::updateAnormalBehavior() {
    if (proba > 1.0-(precision/100)) AnormalBehavior = true;
}

// ----------[supportCDM]----------
// constructor
supportCDM::supportCDM(solver * _s, extendedPlanLibrary* _epl, double _precision) : CognitiveDistressManagement(_s, _epl), precision(_precision) {}

// destructor
supportCDM::~supportCDM() = default;

// functions
void supportCDM::updateAlpha() {
    if(oldParticles.count(s->getLastObservation()) == 0 || (float) oldParticles[s->getLastObservation()].size() < (float) s->getSize() * precision / 100) alpha = 1.0;
    else alpha = 0.0;
}
void supportCDM::updateAnormalBehavior() {
    if (proba > 0.5) AnormalBehavior = true;
}
