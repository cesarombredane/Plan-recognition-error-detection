#include <vector>
#include "src/headers/rule.h"
#include <string>

// constructor
rule::rule():id(-1),primitive(-1) {}
rule::rule(int _primitive, int _id):id(_id), primitive(_primitive) {}
rule::rule(int _id, string s):id(_id) {
    size_t pos;

    if((pos = s.find(':')) != string::npos)
        primitive = stoi(s.substr(0,pos));

    s.erase(0, pos + 1);
    pos = s.find('|');
    string s_children = s.substr(0, pos);
    string s_constraints = s.substr(pos+1, s.find(';'));

    while ((pos = s_children.find(',')) != string::npos) {
        children.push_back(stoi(s.substr(0, pos)));
        s_children.erase(0, pos + 1);
    }

    while ((pos = s_constraints.find("),")) != string::npos) {
        int _first;
        int _second;
        string temp;
        size_t vir;

        temp = s_constraints.substr(1,pos);
        vir = temp.find(',');

        _first = stoi(temp.substr(0,vir));
        _second = stoi(temp.erase(0,vir+1));

        constraints.emplace_back(_first,_second);
        s_children.erase(0, pos + 2);
    }
}

// destructor
rule::~rule() = default;

// functions
int rule::getPrimitive() const {
    return primitive;
}
const vector<int>& rule::getChildren() const {
    return children;
}
const vector<pair<int,int>>& rule::getConstraints() const {
    return constraints;
}
void rule::addChild(int child) {
    children.push_back(child);
}
void rule::addConstraint(pair<int, int> constraint) {
    constraints.push_back(constraint);
}

// toStrings
string rule::toString() {
    string result;
    result+=to_string(this->primitive);
    result+="(";

    for(int i = 0; i<this->children.size();i++) {
        result += to_string(this->children[i]);

        if(i != this->children.size()-1)
            result += ",";
    }

    result+="),(";

    for(int j = 0; j<this->constraints.size();j++) {
        result+= "(";
        result+= to_string(this->constraints[j].first);
        result+= ",";
        result+= to_string(this->constraints[j].second);
        result+= ")";

        if(j != this->constraints.size()-1)
            result += ",";
    }

    result+=")";

    return result;
}
string rule::toString(map<int,string> revIds) {
    string result;
    result+=revIds[this->primitive];
    result+="(";

    for(int i = 0; i<this->children.size();i++) {
        result += revIds[this->children[i]];

        if(i != this->children.size()-1)
            result += ",";
    }

    result+="),(";

    for(int j = 0; j<this->constraints.size();j++) {
        result+= "(";
        result+= to_string(this->constraints[j].first);
        result+= ",";
        result+= to_string(this->constraints[j].second);
        result+= ")";

        if(j != this->constraints.size()-1)
            result += ",";
    }

    result+=")";

    return result;
}

// operators
rule::operator int() const {
    return id;
}
bool rule::operator<(const rule& _r) const {
    return id<(int)_r;
}
bool rule::operator==(const rule& _r) const {
    return id==(int)_r;
}
ostream& operator<<(ostream& os, const rule& r) {
    os << r.primitive << ":";

    for(int i = 0; i<r.children.size();i++) {
        os << r.children[i];

        if(i != r.children.size()-1)
            os << ",";
    }

    os << "|";

    for(int j = 0; j<r.constraints.size();j++) {
        os << "(" << r.constraints[j].first << ","<< r.constraints[j].second << ")";

        if(j != r.constraints.size()-1)
            os << ",";
    }

    os << ";";
    return os;
}
