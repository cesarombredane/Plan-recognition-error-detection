#include "rule.h"

// constructor
rule::rule():id(-1),primitive(-1) {}
rule::rule(int _primitive, int _id):id(_id), primitive(_primitive) {}

// destructor
rule::~rule() = default;

// functions
int rule::getPrimitive() const {
    return this->primitive;
}
int rule::getId() const {
    return this->id;
}
const vector<int>& rule::getChildren() const {
    return this->children;
}
const vector<pair<int,int>>& rule::getConstraints() const {
    return this->constraints;
}

void rule::addChild(int child) {
    this->children.push_back(child);
}
void rule::addConstraint(pair<int, int> constraint) {
    this->constraints.push_back(constraint);
}

// toStrings
string rule::toString(map<int,string>* revIds) {
    string result;

    // print the primitive of the rule
    result += (*revIds)[this->primitive] + " = children(";

    // print the children of the rule
    for(int i = 0; i < this->children.size(); ++i) {
        result += (*revIds)[this->children[i]];
        if(i != this->children.size() - 1) result += ", ";
    }
    result += ") constraints(";

    // print the constraints of the rule
    for(int i = 0; i < this->constraints.size(); ++i) {
        result += to_string(this->constraints[i].first);
        result += "->";
        result += to_string(this->constraints[i].second);
        if(i != this->constraints.size() - 1) result += " | ";
    }
    result += ')';

    return result;
}

// operators
bool rule::operator==(const rule& _r) const {
    return this->id == _r.getId();
}
