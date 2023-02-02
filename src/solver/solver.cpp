#include "solver.h"

// --------- ----------- --------- [treeNode] --------- ---------- ----------

// ctor

treeNode::treeNode() = default;

treeNode::treeNode(extendedPlanLibrary* _epl, int _symbol):epl(_epl), symbol(_symbol) {
    if(!epl->ePlanLibrary->isTerminal(symbol)) {
        _rule = epl->ruleDecisionModel.RNC(symbol);
        status = false;
    }
    else {
        _rule = -1;
        status = true;
    }
}

// function

int treeNode::update() { // update with the ExpectedNextObservation as Observation
    if(status) return -1;

    rule r = epl->ePlanLibrary->getRules().at(_rule);
    vector<int> list_candidate;

    for(int i = 0; i < r.getChildren().size(); i++) {
            bool ok = true;

            for(auto& itC : r.getConstraints()) {
                if(itC.second == i) {
                    if(children.count(itC.first) == 0 || !children[itC.first].status) {
                        ok = false;
                        break;
                    }
                }
            }

            if(children.count(i) > 0 && children[i].status) ok = false;
            if(ok) list_candidate.push_back(i);
    }

    int childId = list_candidate[rand() % list_candidate.size()];
    treeNode* child;

    if(children.count(childId) >0) child = &children[childId];
    else {
        children[childId] = treeNode(epl,r.getChildren()[childId]);
        child = &children[childId];
    }

    int result;
    if(child->status) result = child->symbol;
    else result = child->update();

    status = children.size() == r.getChildren().size();

    if(status) {
        for(auto& it : children) {
            if(!it.second.status) {
                status = false;
                break;
            }
        }
    }

    return result;
}

// --------- ----------- ----------- [tree] ----------- ---------- ----------

// ctor

tree::tree() = default;

tree::tree(extendedPlanLibrary* _epl, int goal):epl(_epl) {
    root = treeNode(epl,goal);
}

// functions

vector<int> tree::update() {
    if(root.status) return {};
    int FO = updateFO();
    currentTerminalFO = FO;

    if(FO == -1) return {};

    int PO = epl->noisePrediction.RNC(FO);

    if (PO == -1) return update();

    vector<int> result;

    if (PO < -1) {
        while(result.size() + PO < -1) {
            auto it = epl->ePlanLibrary->getTerminals().begin();

            int rnd  = rand() % epl->ePlanLibrary->getTerminals().size();
            for(int i =0; i<rnd;i++) it++;

            int temp = *(it);

            if(temp >= 0) result.push_back(temp);
        }
    }

    result.push_back(FO);

    return result;
}

int tree::updateFO()
{
    return root.update();
}

// ---------- ---------- ------ [solverParticles] ----- ---------- ----------

// ctor

solverParticle::solverParticle(extendedPlanLibrary* _epl):epl(_epl) {
    int ruleGoal = epl->ruleDecisionModel.RNC(-1);
    goal = epl->ePlanLibrary->getRules().at(ruleGoal).getChildren().at(0);
    planTree = tree(epl,goal);
    expNextObs = planTree.update();
}

// function

bool solverParticle::update() {
    expNextObs.erase(expNextObs.begin());

    if(expNextObs.empty()) expNextObs = planTree.update();

    return(!expNextObs.empty());
}

// ---------- ---------- ---------- [solver] ---------- ---------- ----------

// ctor & dtor

solver::solver(extendedPlanLibrary* _epl, int _nbParticle):epl(_epl), nbParticles(_nbParticle) {
    size_t currentNbParticle = 0;

    while(currentNbParticle < nbParticles) {
        solverParticle newParticle = solverParticle(epl);
        if(newParticle.planTree.root.status) continue;
        if(particles.count(newParticle.expNextObs[0]) > 0) particles[newParticle.expNextObs[0]].push_back(newParticle);
        else {
            particles[newParticle.expNextObs[0]] = vector<solverParticle>();
            particles[newParticle.expNextObs[0]].push_back(newParticle);
        }
        currentNbParticle++;
    }
}

solver::solver() = default;

solver::~solver() = default;


// functions

map<int,int> solver::getGoals() {
    map<int,int> result;

    for(const auto& itObs : particles) {
        for(const auto& itPar : itObs.second) {
            if(result.count(itPar.goal) > 0) result[itPar.goal]++;
            else result[itPar.goal] = 1;
        }
    }

    return result;
}

int solver::getMaxGoal() { // return the goal with the highest probability to append
    map<int,int> goals = getGoals();

    if(goals.empty()) return -1;

    int result = -1;
    int value = 0;

    for(auto& it: goals) {
        if(value < it.second) {
            value = it.second;
            result = it.first;
        }
    }

    return result;
}

int solver::getLastObservation() {
    return previousObservations[previousObservations.size()-1];
}

const size_t& solver::getSize() const {
    return nbParticles;
}


bool solver::addObservation(int obs) {
    previousObservations.push_back(obs);
    vector<solverParticle> validParticles = particles[obs];

    if(validParticles.empty()) {
        particles.clear();
        return false;
    }

    size_t cnt = 0;

    while(validParticles.size() < nbParticles) {
        cnt++;
        solverParticle newParticle = particles[obs][rand() % particles[obs].size()];
        bool ok = newParticle.update();

        if (ok && !newParticle.expNextObs.empty()) {
           cnt = 0;
           validParticles.push_back(newParticle);
        }

        if (cnt > nbParticles) break;
    }

    particles.clear();

    for(auto it : validParticles) {
        if(particles.count(it.expNextObs[0]) >0) particles[it.expNextObs[0]].push_back(it);
        else {
            particles[it.expNextObs[0]] = vector<solverParticle>();
            particles[it.expNextObs[0]].push_back(it);
        }
    }

    if(particles.empty()) return false;

    return status();
}


pair<int,vector<int>> solver::generatePlan() {
    tree t = tree(epl,epl->ePlanLibrary->getRules().at(epl->ruleDecisionModel.RNC(-1)).getChildren().at(0));
    vector<int> result;
    int nextAction = t.updateFO();

    while(nextAction != -1) {
        int PO = epl->noisePrediction.RNC(nextAction);

        if(PO > -1) result.push_back(PO);

        if (PO < -1) {
            size_t init = result.size();

            while(result.size() - init + PO - 1 < -1) {
                auto it = epl->ePlanLibrary->getTerminals().begin();
                int rnd  = rand() % epl->ePlanLibrary->getTerminals().size();

                for(int i =0; i<rnd;i++) it++;

                int temp = *(it);

                if(temp >= 0) result.push_back(temp);
            }
        }

        nextAction = t.updateFO();
    }

    return make_pair(t.root.symbol, result);
}


bool solver::status() {
    return (!particles.empty());
}
