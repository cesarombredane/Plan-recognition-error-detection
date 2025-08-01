#include <vector>
#include <utility>
#include "../include/rule.h"
#include <string>

// ctor & dtor

rule::rule() : primitive(-1), id(-1) {}

rule::rule(int _primitive, int _id) : primitive(_primitive), id(_id) {}

rule::rule(int _id, string s) : id(_id)
{
    size_t pos = 0;

    if ((pos = s.find(":")) != string::npos)
        primitive = stoi(s.substr(0, pos));

    s.erase(0, pos + 1);
    pos = s.find("|");
    string s_children = s.substr(0, pos);
    string s_constraints = s.substr(pos + 1, s.find(";"));

    while ((pos = s_children.find(",")) != string::npos)
    {
        children.push_back(stoi(s.substr(0, pos)));
        s_children.erase(0, pos + 1);
    }

    while ((pos = s_constraints.find("),")) != string::npos)
    {
        int _first;
        int _second;
        string temp;
        size_t vir;

        temp = s_constraints.substr(1, pos);
        vir = temp.find(",");

        _first = stoi(temp.substr(0, vir));
        _second = stoi(temp.erase(0, vir + 1));

        constraints.push_back(make_pair(_first, _second));
        s_children.erase(0, pos + 2);
    }
}

rule::~rule() {}

// functions

int rule::getPrimitive() const
{
    return primitive;
}

const vector<int> &rule::getChildren() const
{
    return children;
}

const vector<pair<int, int>> &rule::getConstraints() const
{
    return constraints;
}

void rule::addChild(int child)
{
    children.push_back(child);
}

void rule::addChildren(vector<int> _children)
{
    children.insert(children.end(), _children.begin(), _children.end());
}

void rule::addConstraint(pair<int, int> constraint)
{
    constraints.push_back(constraint);
}

void rule::addConstraints(vector<pair<int, int>> _constraints)
{
    constraints.insert(constraints.end(), _constraints.begin(), _constraints.end());
}

// toStrings

const string rule::toString()
{
    string resu = "";
    resu += to_string(this->primitive);
    resu += "(";

    for (int i = 0; i < static_cast<int>(this->children.size()); i++)
    {
        resu += to_string(this->children[i]);

        if (i != static_cast<int>(this->children.size()) - 1)
            resu += ",";
    }

    resu += "),(";

    for (int j = 0; j < static_cast<int>(this->constraints.size()); j++)
    {
        resu += "(";
        resu += to_string(this->constraints[j].first);
        resu += ",";
        resu += to_string(this->constraints[j].second);
        resu += ")";

        if (j != static_cast<int>(this->constraints.size()) - 1)
            resu += ",";
    }

    resu += ")";

    return resu;
}

const string rule::toString(map<int, string> revIds)
{
    string resu = "";
    resu += revIds[this->primitive];
    resu += "(";

    for (int i = 0; i < static_cast<int>(this->children.size()); i++)
    {
        resu += revIds[this->children[i]];

        if (i != static_cast<int>(this->children.size()) - 1)
            resu += ",";
    }

    resu += "),(";

    for (int j = 0; j < static_cast<int>(this->constraints.size()); j++)
    {
        resu += "(";
        resu += to_string(this->constraints[j].first);
        resu += ",";
        resu += to_string(this->constraints[j].second);
        resu += ")";

        if (j != static_cast<int>(this->constraints.size()) - 1)
            resu += ",";
    }

    resu += ")";

    return resu;
}

// operators

rule::operator int() const
{
    return id;
}

bool rule::operator<(const rule _r) const
{
    return id < (int)_r;
}

bool rule::operator==(const rule _r) const
{
    return id == (int)_r;
}

ostream &operator<<(ostream &os, const rule &r)
{
    os << r.primitive << ":";

    for (int i = 0; i < static_cast<int>(r.children.size()); i++)
    {
        os << r.children[i];

        if (i != static_cast<int>(r.children.size()) - 1)
            os << ",";
    }

    os << "|";

    for (int j = 0; j < static_cast<int>(r.constraints.size()); j++)
    {
        os << "(" << r.constraints[j].first << "," << r.constraints[j].second << ")";

        if (j != static_cast<int>(r.constraints.size()) - 1)
            os << ",";
    }

    os << ";";

    return os;
}
