#include "iostream"
#include "string"
#include "unordered_map"
#include "unordered_set"
#include "rule.h"

using namespace std;

#ifndef PLANLABRARY_H
#define PLANLABRARY_H

class planLibrary {
    public:
        planLibrary();
        ~planLibrary();

        const unordered_set<int>& getTerminals() const;
        const unordered_set<int>& getNonTerminals() const;
        const unordered_set<int>& getGoals() const;
        const unordered_map<int,rule>& getRules() const;
        bool isTerminal(int) const;
        bool isGoal(int) const;
        void addSymbol(int, bool, bool);
        void addRule(const rule&);

        string toString();

    private:
        unordered_set<int> terminals;
        unordered_set<int> nonTerminals;
        unordered_set<int> goals;
        unordered_map<int, rule> rules;
};

#endif // PLANLABRARY_H
