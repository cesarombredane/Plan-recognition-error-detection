#include "iostream"
#include "vector"
#include "map"

using namespace std;

#ifndef RULE_H
#define RULE_H

class rule {
    public:
        rule();
        rule(int, int);

        virtual ~rule();

        int getPrimitive() const;
        int getId() const;
        const vector<int>& getChildren() const;
        const vector<pair<int,int>>& getConstraints() const;

        void addChild(int);
        void addConstraint(pair<int,int>);

        string toString(map<int,string>*);

        bool operator==(const rule&) const;

    private:
        int primitive;
        int id;
        vector<int> children;
        vector<pair<int,int>> constraints;
};

#endif // RULE_H
