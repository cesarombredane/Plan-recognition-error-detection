#include "src/headers/probabilityDistribution.h"

// constructor
probabilityDistribution::probabilityDistribution() = default;
probabilityDistribution::probabilityDistribution(unordered_map<int, unordered_map<int,float>> _distribution):distribution(move(_distribution)) {}
probabilityDistribution::probabilityDistribution(planLibrary& _pl) {
    for(const auto& it : _pl.getRules()) {
        if(this->distribution.count(it.second.getPrimitive()) == 0) this->distribution[it.second.getPrimitive()][it.first] = 1;
        else {
            size_t siz = this->distribution[it.second.getPrimitive()].size();
            for(auto& dit : this->distribution[it.second.getPrimitive()])
                dit.second = 1 / (float) (siz + 1);
            this->distribution[it.second.getPrimitive()][it.first] = 1 / (float) (siz + 1);
        }
    }
}
probabilityDistribution::probabilityDistribution(const unordered_set<int>& _terminals, float noise ) {
    for(auto t_i: _terminals) {
        if(noise == 0) this->distribution[t_i][t_i] = 1;
        else
            for(auto t_j: _terminals) {
                if(t_i == t_j) this->distribution[t_i][t_j] = 1 - noise;
                else this->distribution[t_i][t_j] = noise / (float) (_terminals.size() - 1);
            }
    }
}

// destructor
probabilityDistribution::~probabilityDistribution() = default;

// functions
const unordered_map<int, unordered_map<int, float>> &probabilityDistribution::getDistribution() {
    return this->distribution;
}
bool probabilityDistribution::isValid() const {
    for(const auto& it : this->distribution) {
        float sum = 0;
        for(auto dit : it.second)
            sum += dit.second;
        if(sum != 1.0) return false;
    }

    return true;
}
void probabilityDistribution::setProb(int primitive, int element, float probability) {
    this->distribution[primitive][element] = probability;
}
int probabilityDistribution::RNC(int i) const {
    random_device rd;
    mt19937 e(rd());
    uniform_real_distribution<> dist(0,1);

    float sum = 0;
    auto rand = (float) dist(e);

    for(auto& it : this->distribution.at(i))
        if(rand <= (sum+=it.second))
            return it.first;

    return 0;
}


