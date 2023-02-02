#include "CognitiveDistressManagement.h"


// -- CognitiveDistressManagement ----------------

// ctor & dtor
CognitiveDistressManagement::CognitiveDistressManagement(solver * _s, extendedPlanLibrary* _epl) {
    AnormalBehavior = false;
    proba = 0.0;
    alpha = 0.0;
    s = _s;
    ePL = _epl;
    oldParticles = s->particles;
}
CognitiveDistressManagement::CognitiveDistressManagement() = default;
CognitiveDistressManagement::~CognitiveDistressManagement() = default;

// getters
bool CognitiveDistressManagement::getAB() const {
    return AnormalBehavior;
}

// update at each new observation
void CognitiveDistressManagement::update() {
    updateAlpha();

    double temp = proba;
    proba = temp + alpha*(1-temp);

    updateAnormalBehavior();

    if (!AnormalBehavior) oldParticles = s->particles;
}

map<int,float> CognitiveDistressManagement::getGoals() {
    map<int,float> result;

    for(const auto& itObs : oldParticles) {
        for(const auto& itPar : itObs.second) {
            if(result.count(itPar.goal) > 0) result[itPar.goal]+= 1 / (float)s->nbParticles;
            else result[itPar.goal] = 1 / (float)s->nbParticles;
        }
    }

    return result;
}

map<int,float> CognitiveDistressManagement::getActions() {
    map<int,float> result;

    for(auto itN : ePL->noisePrediction.getDistribution()) {
        result[itN.first] = itN.second[s->getLastObservation()] * (1.0 / ePL->noisePrediction.getDistribution().size());
        float dem = 0.0;

        for(auto itO : itN.second) dem += itO.second*(1.0/ePL->noisePrediction.getDistribution().size());

        if(dem == 0.0) result[itN.first] = 0.0;
        else result[itN.first] = result[itN.first] / dem;
    }

    return result;
}

map<int,set<int>> CognitiveDistressManagement::filterAAlpha() {
    map<int,set<int>> result;
    map<int,set<int>> nextPossibleActions;
    set<int> allActions(ePL->ePlanLibrary->getTerminals().begin(),  ePL->ePlanLibrary->getTerminals().end());

    for(auto itG : ePL->ePlanLibrary->getGoals()) result[itG] = allActions;

    vector<solverParticle> allParticles;

    for(auto itOP : oldParticles) allParticles.insert(allParticles.end(),itOP.second.begin(),itOP.second.end());
    for(const auto& itAP : allParticles) nextPossibleActions[itAP.goal].insert(itAP.planTree.currentTerminalFO);
    for(const auto& itNPA1 :nextPossibleActions) for(auto itNPA2 : itNPA1.second) result[itNPA1.first].erase(result[itNPA1.first].find(itNPA2));

    return result;
}


// -- CDMMin -------------------------------------
CDMMin::CDMMin(solver * _s, extendedPlanLibrary* _epl) : CognitiveDistressManagement(_s, _epl) {}
CDMMin::~CDMMin() = default;

void CDMMin::updateAlpha() {
    alpha = 1.0;
    map<int,float> G = getGoals();
    map<int,float> A = getActions();
    map<int,set<int>> fAlpha = filterAAlpha();

    for(auto itG : G) for(auto itfA : fAlpha[itG.first]) if(A.count(itfA) > 0) alpha = min((float) alpha, itG.second*A[itfA]); // based on fuzzy logic
}
void CDMMin::updateAnormalBehavior() {
    if (proba > 0.5) AnormalBehavior = true;
}

// -- CDMSum -------------------------------------
CDMSum::CDMSum(solver * _s, extendedPlanLibrary* _epl, double _precision) : CognitiveDistressManagement(_s, _epl), precision(_precision) {}
CDMSum::~CDMSum() = default;

void CDMSum::updateAlpha() {
    alpha = 0.0;
    map<int,float> G = getGoals();
    map<int,float> A = getActions();
    map<int,set<int>> fAlpha = filterAAlpha();

    for(auto itG : G) for(auto itfA : fAlpha[itG.first]) if(A.count(itfA) > 0) alpha += itG.second*A[itfA];
}
void CDMSum::updateAnormalBehavior() {
    if (proba > 1.0-(precision/100)) AnormalBehavior = true;
}

// -- CDMSupport ---------------------------------
// only usable with particles filter plan recognition
CDMSupport::~CDMSupport() = default;
CDMSupport::CDMSupport(solver * _s, extendedPlanLibrary* _epl, double _precision) : CognitiveDistressManagement(_s, _epl), precision(_precision) {}

void CDMSupport::updateAlpha() {
    if(oldParticles.count(s->getLastObservation()) == 0 || oldParticles[s->getLastObservation()].size() < s->getSize()*precision/100) alpha = 1.0;
    else alpha = 0.0;
}
void CDMSupport::updateAnormalBehavior() {
    if (proba > 0.5) AnormalBehavior = true;
}
