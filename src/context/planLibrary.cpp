#include "planLibrary.h"

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

void planLibrary::addSymbol(int symbol, bool terminal, bool goal) {
    if (terminal) this->terminals.insert(symbol);
    else this->nonTerminals.insert(symbol);
    if (goal) this->goals.insert(symbol);
}
void planLibrary::addRule(const rule& r) {
    this->rules[r.getId()] = r;
}
