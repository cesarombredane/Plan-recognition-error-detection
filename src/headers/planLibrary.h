/*!
 *  \file planLibrary.h
 *  \brief rerpresentation of a plan library
 *
 *  \version 1.0
 *  \author Jean Massardi
 */

#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include "rule.h"

using namespace std;

#ifndef PLANLABRARY_H
#define PLANLABRARY_H

/*!
 *  \class planLibrary
 *  \brief representation of a plan library
 *  a plan library is made of four sets :
 *  - Terminal (atomic actions)
 *  - nonTerminals (all the others actions)
 *  - Goals (goal belong to nonTerminal)
 *  - rules (how to produce nonTerminals)
 */
class planLibrary {
    public:
        /*!
         *  \brief Constructor
         *  planLibrary constructor
         */
        planLibrary();

        /*!
         *  \brief Destructor
         *  planLibrary Destructor
         */
        virtual ~planLibrary();

        /*!
         *  \brief Getter Terminals 
         */
        const unordered_set<int>& getTerminals() const;

        /*!
         *  \brief Getter nonTerminals 
         */
        const unordered_set<int>& getNonTerminals() const;

        /*!
         *  \brief Getter Goals 
         */
        const unordered_set<int>& getGoals() const;

        /*!
         *  \brief Getter Rules 
         */
        const unordered_map<int,rule>& getRules() const;

        /*!
         *  \brief is an action a Terminal 
         *
         *  \param symbol : an action
         *  \return if the action is a Terminal or not
         */
        bool isTerminal(int symbol) const;

        /*!
         *  \brief is an action a Goal 
         *
         *  \param symbol : an action
         *  \return if the action is a Goal or not
         */
        bool isGoal(int symbol) const;
        
        /*!
         *  \brief Add action 
         *  add an action in the right set
         *
         *  \param s : the action number
         *  \param t : is the action a Terminal
         *  \param g : is the action a Goal
         */
        void addSymbol(int s, bool t, bool g);

        /*!
         *  \brief Add rule 
         *
         *  \param r : the rule
         */
        void addRule(rule r);

        /*!
         *  \brief String representation of the PlanLibrary
         */
        string toString();

    private:
        unordered_set<int> terminals;/*!< set of Terminal action */
        unordered_set<int> nonTerminals;/*!< set of nonTerminal action */
        unordered_set<int> goals;/*!< set of Goals (belong to nonTerminal) */
        unordered_map<int,rule> rules;/*!< set of Rules */
};

#endif // PLANLABRARY_H
