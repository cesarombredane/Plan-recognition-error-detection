#include "src/headers/CognitiveDistressManagement.h"


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

    int temp = proba;
    proba = temp + alpha*(1-temp);

    updateAnormalBehavior();

    if (!AnormalBehavior)
        oldParticles = s->particles;
}

map<int,float> CognitiveDistressManagement::getGoals() {
    map<int,float> resu;

    for(const auto& itObs : oldParticles) {
        for(const auto& itPar : itObs.second) {
            if(resu.count(itPar.goal) > 0)
                resu[itPar.goal]+=1.0/(float)s->nbParticles;
            else
                resu[itPar.goal] = 1.0/(float)s->nbParticles;
        }
    }

    return resu;
}

map<int,float> CognitiveDistressManagement::getActions() {
    map<int,float> resu;

    for(auto itN : ePL->noise.getDistribution()) {
        resu[itN.first] = itN.second[s->getLastObservation()]*(1.0/ePL->noise.getDistribution().size());
        float dem = 0.0;

        for(auto itO : itN.second)
            dem += itO.second*(1.0/ePL->noise.getDistribution().size());

        if(dem == 0.0)
            resu[itN.first] = 0.0;
        else
            resu[itN.first] = resu[itN.first]/dem;
    }

    return resu;
}

map<int,set<int>> CognitiveDistressManagement::filterAAlpla() {
    map<int,set<int>> resu;
    map<int,set<int>> nextPossibleActions;
    set<int> allActions(ePL->ePlanLibrary->getTerminals().begin(),  ePL->ePlanLibrary->getTerminals().end());

    for(auto itG : ePL->ePlanLibrary->getGoals())
        resu[itG] = allActions;

    vector<solverParticle> allParticles;

    for(auto itOP : oldParticles)
        allParticles.insert(allParticles.end(),itOP.second.begin(),itOP.second.end());

    for(auto itAP : allParticles)
        nextPossibleActions[itAP.goal].insert(itAP.planTree.currentTerminalFO);

    for(auto itNPA1 :nextPossibleActions)
        for(auto itNPA2 : itNPA1.second)
            resu[itNPA1.first].erase(resu[itNPA1.first].find(itNPA2));

    return resu;
}


// -- CDMmin -------------------------------------
CDMmin::CDMmin(solver * _s, extendedPlanLibrary* _epl) : CognitiveDistressManagement(_s, _epl) {};
CDMmin::~CDMmin() {}

void CDMmin::updateAlpha() {
    alpha = 1.0;
    map<int,float> G = getGoals();
    map<int,float> A = getActions();
    map<int,set<int>> fAlpha = filterAAlpla();

    for(auto itG : G)
        for(auto itfA : fAlpha[itG.first])
            if(A.count(itfA) > 0)
                alpha = min((float) alpha, itG.second*A[itfA]); // based on fuzzy logic
}
void CDMmin::updateAnormalBehavior() {
    if (proba > 0.5)
            AnormalBehavior = true;
}

// -- CDMsum -------------------------------------
CDMsum::CDMsum(solver * _s, extendedPlanLibrary* _epl, double _precision) : CognitiveDistressManagement(_s, _epl), precision(_precision) {};
CDMsum::~CDMsum() = default;

void CDMsum::updateAlpha() {
    alpha = 0.0;
    map<int,float> G = getGoals();
    map<int,float> A = getActions();
    map<int,set<int>> fAlpha = filterAAlpla();

    for(auto itG : G)
        for(auto itfA : fAlpha[itG.first])
            if(A.count(itfA) > 0)
                alpha += itG.second*A[itfA];
}
void CDMsum::updateAnormalBehavior() {
    if (proba > 1.0-(precision/100))
            AnormalBehavior = true;
}

// -- CDMsupport ---------------------------------
// only usable with particles filter plan recognition
CDMsupport::~CDMsupport() = default;
CDMsupport::CDMsupport(solver * _s, extendedPlanLibrary* _epl, double _precision) : CognitiveDistressManagement(_s, _epl), precision(_precision) {};

void CDMsupport::updateAlpha() {
    if(oldParticles.count(s->getLastObservation()) == 0)
        alpha = 1.0;
    else if(oldParticles[s->getLastObservation()].size() < s->getSize()*precision/100)
        alpha = 1.0;
    else
        alpha = 0.0;
}
void CDMsupport::updateAnormalBehavior() {
    if (proba > 0.5)
            AnormalBehavior = true;
}


