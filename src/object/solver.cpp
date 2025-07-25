#include "solver.h"

// --------- ----------- --------- [treeNode] --------- ---------- ----------

// ctor

treeNode::treeNode() {}

treeNode::treeNode(extendedPlanLibrary *_epl, int _symbol) : epl(_epl), symbol(_symbol)
{
    if (!epl->ePlanLibrary->isTerminal(symbol))
    {
        _rule = epl->decisionModel.RNC(symbol);
        status = false;
    }
    else
    {
        _rule = -1;
        status = true;
    }
}

// function

int treeNode::update()
{ // update with the ExpectedNextObservation as Observation
    if (status)
        return -1;

    rule r = epl->ePlanLibrary->getRules().at(_rule);
    vector<int> list_candidate;

    for (size_t i = 0; i < r.getChildren().size(); i++)
    {
        bool ok = true;

        for (auto &itC : r.getConstraints())
        {
            if (itC.second == i)
            {
                if (children.count(itC.first) == 0)
                {
                    ok = false;
                    break;
                }
                else if (!children[itC.first].status)
                {
                    ok = false;
                    break;
                }
            }
        }

        if (children.count(i) > 0)
            if (children[i].status)
                ok = false;

        if (ok)
            list_candidate.push_back(i);
    }

    int childId = list_candidate[rand() % list_candidate.size()];
    treeNode *child;

    if (children.count(childId) > 0)
        child = &children[childId];
    else
    {
        children[childId] = treeNode(epl, r.getChildren()[childId]);
        child = &children[childId];
    }

    int resu;

    if (child->status)
        resu = child->symbol;
    else
        resu = child->update();

    status = children.size() == r.getChildren().size();

    if (status)
    {
        for (auto &it : children)
        {
            if (!it.second.status)
            {
                status = false;
                break;
            }
        }
    }

    return resu;
}

// --------- ----------- ----------- [tree] ----------- ---------- ----------

// ctor

tree::tree() {}

tree::tree(extendedPlanLibrary *_epl, int goal) : epl(_epl)
{
    root = treeNode(epl, goal);
}

// functions

vector<int> tree::update()
{
    if (root.status)
        return vector<int>();

    int FO = updateFO();
    currentTerminalFO = FO;

    if (FO == -1)
        return vector<int>();

    int PO = epl->noise.RNC(FO);

    if (PO == -1)
        return update();

    vector<int> resu;

    if (PO < -1)
    {
        while (resu.size() + PO < -1)
        {
            auto it = epl->ePlanLibrary->getTerminals().begin();

            int rnd = rand() % epl->ePlanLibrary->getTerminals().size();
            for (int i = 0; i < rnd; i++)
                it++;

            int temp = *(it);

            if (temp >= 0)
                resu.push_back(temp);
        }
    }

    resu.push_back(FO);

    return resu;
}

int tree::updateFO()
{
    return root.update();
}

// ---------- ---------- ------ [solverParticles] ----- ---------- ----------

// ctor

solverParticle::solverParticle(extendedPlanLibrary *_epl) : epl(_epl)
{
    int ruleGoal = epl->decisionModel.RNC(-1);
    goal = epl->ePlanLibrary->getRules().at(ruleGoal).getChildren().at(0);
    planTree = tree(epl, goal);
    expNextObs = planTree.update();
}

// function

bool solverParticle::update()
{
    expNextObs.erase(expNextObs.begin());

    if (expNextObs.empty())
        expNextObs = planTree.update();

    return (!expNextObs.empty());
}

// ---------- ---------- ---------- [solver] ---------- ---------- ----------

// ctor & dtor

solver::solver(extendedPlanLibrary *_epl, int _nbParticle) : epl(_epl), nbParticles(_nbParticle)
{
    size_t currentNbParticle = 0;

    while (currentNbParticle < nbParticles)
    {
        solverParticle newParticle = solverParticle(epl);

        if (newParticle.planTree.root.status)
            continue;

        if (particles.count(newParticle.expNextObs[0]) > 0)
            particles[newParticle.expNextObs[0]].push_back(newParticle);
        else
        {
            particles[newParticle.expNextObs[0]] = vector<solverParticle>();
            particles[newParticle.expNextObs[0]].push_back(newParticle);
        }

        currentNbParticle++;
    }
}

solver::solver() {}

solver::~solver() {}

// functions

map<int, int> solver::getGoals()
{
    map<int, int> resu;

    for (auto itObs : particles)
    {
        for (auto itPar : itObs.second)
        {
            if (resu.count(itPar.goal) > 0)
                resu[itPar.goal]++;
            else
                resu[itPar.goal] = 1;
        }
    }

    return resu;
}

map<int, int> solver::getParticles()
{
    map<int, int> resu;

    for (auto itObs : particles)
        resu[itObs.first] = itObs.second.size();

    return resu;
}

map<string, float> solver::getProbGoals()
{
    map<string, float> resu;

    for (auto itObs : particles)
    {
        for (auto itPar : itObs.second)
        {
            if (resu.count(epl->revIds[itPar.goal]) > 0)
                resu[epl->revIds[itPar.goal]] += 1.0 / (float)nbParticles;
            else
                resu[epl->revIds[itPar.goal]] = 1.0 / (float)nbParticles;
        }
    }

    return resu;
}

map<string, float> solver::getProbParticles()
{
    map<string, float> resu;

    for (auto it : particles)
        resu[epl->revIds[it.first]] = (float)it.second.size() / (float)nbParticles;

    return resu;
}

int solver::getMaxGoal()
{ // return the goal with the highest probability to append
    map<int, int> goals = getGoals();

    if (goals.empty())
        return -1;

    int resu = -1;
    int value = 0;

    for (auto &it : goals)
    {
        if (value < it.second)
        {
            value = it.second;
            resu = it.first;
        }
    }

    return resu;
}

int solver::getLastObservation()
{
    return previousObservations[previousObservations.size() - 1];
}

const size_t &solver::getSize() const
{
    return nbParticles;
}

bool solver::addObservation(int obs)
{
    previousObservations.push_back(obs);
    vector<solverParticle> validParticles = particles[obs];

    if (validParticles.size() == 0)
    {
        particles.clear();
        return false;
    }

    size_t cnt = 0;

    while (validParticles.size() < nbParticles)
    {
        cnt++;
        solverParticle newParticle = particles[obs][rand() % particles[obs].size()];
        bool ok = newParticle.update();

        if (ok && !newParticle.expNextObs.empty())
        {
            cnt = 0;
            validParticles.push_back(newParticle);
        }

        if (cnt > nbParticles)
            break;
    }

    particles.clear();

    for (auto it : validParticles)
    {
        if (particles.count(it.expNextObs[0]) > 0)
            particles[it.expNextObs[0]].push_back(it);
        else
        {
            particles[it.expNextObs[0]] = vector<solverParticle>();
            particles[it.expNextObs[0]].push_back(it);
        }
    }

    if (particles.empty())
        return false;

    return status();
}

bool solver::addObservation(string obs)
{
    return addObservation(this->epl->ids[obs]);
}

pair<int, vector<int>> solver::generatePlan()
{
    tree t = tree(epl, epl->ePlanLibrary->getRules().at(epl->decisionModel.RNC(-1)).getChildren().at(0));
    vector<int> resu;
    int nextAction = t.updateFO();

    while (nextAction != -1)
    {
        int PO = epl->noise.RNC(nextAction);

        if (PO > -1)
            resu.push_back(PO);

        if (PO < -1)
        {
            int init = resu.size();

            while (resu.size() - init + PO - 1 < -1)
            {
                auto it = epl->ePlanLibrary->getTerminals().begin();
                int rnd = rand() % epl->ePlanLibrary->getTerminals().size();

                for (int i = 0; i < rnd; i++)
                    it++;

                int temp = *(it);

                if (temp >= 0)
                    resu.push_back(temp);
            }
        }

        nextAction = t.updateFO();
    }

    return make_pair(t.root.symbol, resu);
}

pair<int, vector<int>> solver::generatePlanFO()
{
    tree t = tree(epl, epl->ePlanLibrary->getRules().at(epl->decisionModel.RNC(-1)).getChildren().at(0));
    vector<int> resu;
    int nextAction = t.updateFO();

    while (nextAction != -1)
    {
        resu.push_back(nextAction);
        nextAction = t.updateFO();
    }

    return make_pair(t.root.symbol, resu);
}

bool solver::status()
{
    return (particles.size() != 0);
}

void solver::clear()
{
    previousObservations.clear();
    particles.clear();
    size_t currentNbParticle = 0;

    while (currentNbParticle < nbParticles)
    {
        solverParticle newParticle = solverParticle(epl);

        if (particles.count(newParticle.expNextObs[0]) > 0)
            particles[newParticle.expNextObs[0]].push_back(newParticle);
        else
        {
            particles[newParticle.expNextObs[0]] = vector<solverParticle>();
            particles[newParticle.expNextObs[0]].push_back(newParticle);
        }

        currentNbParticle++;
    }
}
