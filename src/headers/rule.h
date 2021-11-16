/*!
 *  \file rule.h
 *  \brief implementation of the production rules of a non terminal action
 *
 *  \version 1.0
 *  \author Jean Massardi
 */

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>

using namespace std;

#ifndef RULE_H
#define RULE_H

/*!
 *  \class rule
 *  \brief production rules to form a non terminal action
 *  this class contain the children list to create a non terminal action and the order constraints.
 */
class rule {
    public:
        /*!
         *  \brief Constructor
         *  rule constructor
         */
        rule();

        /*!
         *  \brief Constructor
         *  rule constructor
         *
         *  \param _primitive : non terminal action that is created with this rule
         *  \param _id : id of the rule
         */
        rule(int _primitive, int _id);

        /*!
         *  \brief Constructor
         *  rule constructor
         *  
         *  \param _id : id of the rule
         *  \param s : string that identify a rule (primitive : child1, chlid2, child3 | ((index1, index2),(index1, index3)) )
         */
        rule(int _id, string s);

        /*!
         *  \brief Destructor
         *  rule destructor
         */
        virtual ~rule();

        /*!
         *  \brief Getter primitive 
         */
        const int getPrimitive() const;

        /*!
         *  \brief Getter children 
         */
        const vector<int>& getChildren() const; 

        /*!
         *  \brief Getter constraints 
         */
        const vector<pair<int,int>>& getConstraints() const;

        /*!
         *  \brief Add child 
         *
         *  \param child : a new child 
         */
        void addChild(int child);

        /*!
         *  \brief Add constraint 
         *
         *  \param constraint : a new constraint (pair<index1, index2>) the child at index1 must be done before the child at index2.
         */
        void addConstraint(pair<int,int> constraint);

        /*!
         *  \brief Add a list of child 
         *
         *  \param _children : a list of new child 
         */
        void addChildren(vector<int> _children);

        /*!
         *  \brief Add a list of constraint
         *
         *  \param _constraints : a list of new constraint
         */
        void addConstraints(vector<pair<int,int>> _constraints);

        /*!
         *  \brief String representation of the rule
         */
        const string toString();

        /*!
         *  \brief String representation of the rule
         *  all the int are replaced by the name of the actions
         *
         *  \param revIds : map with action's number as keys and action's name as values
         */
        const string toString(map<int,string> revIds);

        /*!
         *  \brief comparison between 2 rules
         *  two rules are compared based on their ids.
         *
         *  \param _r : a rule
         */
        bool operator<(const rule _r) const;

        /*!
         *  \brief equality comparison between 2 rules
         *  two rules are equals if their id is equal.  
         *
         *  \param _r : a rule
         */
        bool operator==(const rule _r) const;

        /*!
         *  \brief get the rule's id
         *
         *  \return the rule's id
         */
        operator int() const;

        friend ostream& operator << (ostream&, const rule&);

    private:
        int primitive; /*!< non terminal action created with the rule */
        int id;/*!< id of the rule */
        vector<int> children;/*!< list of required action to form primitive */
        vector<pair<int,int>> constraints;/*!< list of order constraints */
};

namespace std {
    template<>
    struct hash<rule> {
      size_t operator()(const rule & obj) const {
        return hash<int>()((int)obj);
      }
    };
}

#endif // RULE_H
