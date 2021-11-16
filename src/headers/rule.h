#include "iostream"
#include "map"

using namespace std;

#ifndef RULE_H
#define RULE_H

class rule {
    public:
        rule();
        rule(int, int);
        rule(int, string);

        virtual ~rule();

        int getPrimitive() const;
        const vector<int>& getChildren() const;
        const vector<pair<int,int>>& getConstraints() const;

        void addChild(int);
        void addConstraint(pair<int,int>);
        void addChildren(vector<int>);
        void addConstraints(vector<pair<int,int>>);

        string toString();
        string toString(map<int,string>);

        bool operator<(const rule&) const;
        bool operator==(const rule&) const;
        explicit operator int() const;
        friend ostream& operator << (ostream&, const rule&);

    private:
        int primitive;
        int id;
        vector<int> children;
        vector<pair<int,int>> constraints;
};

#endif // RULE_H
