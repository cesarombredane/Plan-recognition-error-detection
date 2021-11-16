#include "src/headers/planLibrary.h"
#include "src/headers/bnflite.h"

using namespace bnf;

// ctor & dtor
planLibrary::planLibrary() = default;
planLibrary::~planLibrary() = default;


// functions

const unordered_set<int>& planLibrary::getTerminals() const {
    return terminals;
}

const unordered_set<int>& planLibrary::getNonTerminals() const {
    return nonTerminals;
}

const unordered_set<int>& planLibrary::getGoals() const {
    return goals;
}

const unordered_map<int,rule>& planLibrary::getRules() const {
    return rules;
}


bool planLibrary::isTerminal(int symbol) const {
    return terminals.count(symbol) > 0;
}

bool planLibrary::isGoal(int symbol) const {
    return goals.count(symbol) > 0;
}


void planLibrary::addRule(rule r) {
    rules[(int)r] = r;

    if(nonTerminals.count(r.getPrimitive()) == 0)
        nonTerminals.insert(r.getPrimitive());

    for(auto it : r.getChildren()) {
        if(nonTerminals.count(it) == 0)
            if(terminals.count(it) == 0)
                nonTerminals.insert(it);
    }
}

void planLibrary::addSymbol(int s, bool t, bool g) {
    if(t)
        terminals.insert(s);
    else
        nonTerminals.insert(s);
    
    if(g)
        goals.insert(s);
}


// toString

string planLibrary::toString() {
    string result;

    result += "Goals(";
    int cnt = 0;

    for (auto itG : this->goals) {
        cnt++;
        result += to_string(itG);

        if(cnt < this->goals.size())
            result+=",";
    }

    result+=") \n";
    result += "Terminals(";
    cnt = 0;

    for (auto itT : this->terminals) {
        cnt++;
        result += to_string(itT);

        if(cnt < this->terminals.size())
            result+=",";
    }

    result+=") \n";
    result += "NonTerminals(";
    cnt = 0;

    for (auto itNT : this->nonTerminals) {
        cnt++;
        result += to_string(itNT);

        if(cnt < this->nonTerminals.size())
            result+=",";
    }

    result+=") \n";
    result += "Rules(";
    cnt = 0;

    for (auto itR : this->rules) {
        cnt++;
        result += to_string(itR.first);
        result+="(" + itR.second.toString() + ")";

        if(cnt < this->rules.size())
            result+=" \n";
    }

    result+=")";

    return result;
}