#include "../include/plan_library.h"
#include "../include/bnflite.h"

using namespace bnf;

// ctor & dtor

planLibrary::planLibrary() {}

planLibrary::~planLibrary() {}

// functions

const unordered_set<int> &planLibrary::getTerminals() const
{
    return terminals;
}

const unordered_set<int> &planLibrary::getNonTerminals() const
{
    return nonTerminals;
}

const unordered_set<int> &planLibrary::getGoals() const
{
    return goals;
}

const unordered_map<int, rule> &planLibrary::getRules() const
{
    return rules;
}

bool planLibrary::isTerminal(int symbol) const
{
    return terminals.count(symbol) > 0;
}

bool planLibrary::isGoal(int symbol) const
{
    return goals.count(symbol) > 0;
}

void planLibrary::addRule(rule r)
{
    rules[(int)r] = r;

    if (nonTerminals.count(r.getPrimitive()) == 0)
        nonTerminals.insert(r.getPrimitive());

    for (auto it : r.getChildren())
    {
        if (nonTerminals.count(it) == 0)
            if (terminals.count(it) == 0)
                nonTerminals.insert(it);
    }
}

void planLibrary::addSymbol(int s, bool t, bool g)
{
    if (t)
        terminals.insert(s);
    else
        nonTerminals.insert(s);

    if (g)
        goals.insert(s);
}

// toString

const string planLibrary::toString()
{
    string resu = "";

    resu += "Goals(";
    size_t cnt = 0;

    for (auto itG : this->goals)
    {
        cnt++;
        resu += to_string(itG);

        if (cnt < this->goals.size())
            resu += ",";
    }

    resu += ") \n";
    resu += "Terminals(";
    cnt = 0;

    for (auto itT : this->terminals)
    {
        cnt++;
        resu += to_string(itT);

        if (cnt < this->terminals.size())
            resu += ",";
    }

    resu += ") \n";
    resu += "NonTerminals(";
    cnt = 0;

    for (auto itNT : this->nonTerminals)
    {
        cnt++;
        resu += to_string(itNT);

        if (cnt < this->nonTerminals.size())
            resu += ",";
    }

    resu += ") \n";
    resu += "Rules(";
    cnt = 0;

    for (auto itR : this->rules)
    {
        cnt++;
        resu += to_string(itR.first);
        resu += "(" + itR.second.toString() + ")";

        if (cnt < this->rules.size())
            resu += " \n";
    }

    resu += ")";

    return resu;
}