#include "src/headers/planLibrary.h"

// constructor
planLibrary::planLibrary() = default;

// destructor
planLibrary::~planLibrary() = default;

// functions
const unordered_set<int>& planLibrary::getTerminals() const {
    return this->terminals;
}
const unordered_set<int>& planLibrary::getNonTerminals() const {
    return this->nonTerminals;
}
const unordered_set<int>& planLibrary::getGoals() const {
    return this->goals;
}
const unordered_map<int,rule>& planLibrary::getRules() const {
    return this->rules;
}
bool planLibrary::isTerminal(int symbol) const {
    return this->terminals.count(symbol) > 0;
}
bool planLibrary::isGoal(int symbol) const {
    return this->goals.count(symbol) > 0;
}
void planLibrary::addRule(const rule& r) {
    this->rules[(int)r] = r;

    if(this->nonTerminals.count(r.getPrimitive()) == 0) this->nonTerminals.insert(r.getPrimitive());

    for(auto it : r.getChildren())
        if(this->nonTerminals.count(it) == 0)
            if(this->terminals.count(it) == 0) this->nonTerminals.insert(it);
}
void planLibrary::addSymbol(int symbol, bool terminal, bool goal) {
    if(terminal) this->terminals.insert(symbol);
    else this->nonTerminals.insert(symbol);
    if(goal) this->goals.insert(symbol);
}

// toString
string planLibrary::toString() {
    string result;
    int counter = 0;

    result += "Goals(";
    for (auto& itG : this->goals) {
        ++counter;
        result += to_string(itG);
        if(counter < this->goals.size()) result += ',';
    }
    counter = 0;

    result += ")\n";
    result += "Terminals(";
    for (auto& itT : this->terminals) {
        ++counter;
        result += to_string(itT);
        if(counter < this->terminals.size()) result += ',';
    }
    counter = 0;

    result += ")\n";
    result += "NonTerminals(";
    for (auto& itNT : this->nonTerminals) {
        ++counter;
        result += to_string(itNT);
        if(counter < this->nonTerminals.size()) result += ',';
    }
    counter = 0;

    result += ")\n";
    result += "Rules(";
    for (auto& itR : this->rules) {
        ++counter;
        result += to_string(itR.first);
        result += "(" + itR.second.toString() + ")";
        if(counter < this->rules.size()) result += '\n';
    }
    result += ')';

    return result;
}