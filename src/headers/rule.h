#include "iostream"
#include "map"

using namespace std;

#ifndef RULE_H
#define RULE_H

class rule {
    public:
        rule();
        rule(int _primitive, int _id);
        rule(int _id, string s);

        virtual ~rule();

        int getPrimitive() const;
        const vector<int>& getChildren() const;
        const vector<pair<int,int>>& getConstraints() const;

        void addChild(int child);
        void addConstraint(pair<int,int> constraint);

        string toString();
        string toString(map<int,string> revIds);

        bool operator<(const rule& _r) const;
        bool operator==(const rule& _r) const;
        explicit operator int() const;
        friend ostream& operator << (ostream&, const rule&);

    private:
        int primitive;
        int id;
        vector<int> children;
        vector<pair<int,int>> constraints;
};

#endif // RULE_H
