#include "probabilityDistribution.h"

// constructor
probabilityDistribution::probabilityDistribution() = default;
probabilityDistribution::probabilityDistribution(planLibrary& _pl) {
    // create an even distribution for all the rules from one primitive (for all the non-terminal symbol in the plan library)

    // add all the rules to the probability distribution
    for(const auto& it : _pl.getRules()) {
        // create the non-terminal symbol if it doesn't exist in the probability distribution
        if(this->distribution.count(it.second.getPrimitive()) == 0) this->distribution[it.second.getPrimitive()][it.first] = 1;
        else {
            // update all the probability from one non-terminal symbol
            float siz = (float) this->distribution[it.second.getPrimitive()].size();
            for(auto& dit : this->distribution[it.second.getPrimitive()]) dit.second = 1 / (siz + 1);
            this->distribution[it.second.getPrimitive()][it.first] = 1 / (siz + 1);
        }
    }
}
probabilityDistribution::probabilityDistribution(const unordered_set<int>& _terminals, float noise) {
    // create a probability distribution for all the terminal symbols
    for(auto t_i: _terminals) {
        // if there is no noise, the chance of guessing the right symbol is 1
        if(noise == 0) this->distribution[t_i][t_i] = 1;
        else
            // if there is noise, we set the probability to mistake the terminal symbol for another one
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
int probabilityDistribution::getRandomValue(int i) const {
    // init the randomizer
    random_device rd;
    mt19937 e(rd());
    uniform_real_distribution<> dist(0,1);

    float sum = 0;
    auto rand = (float) dist(e);

    // iter over the probability of an element (primitive of terminal symbol)
    for(auto& it : this->distribution.at(i))
        // return a random elem (based on the probability distribution) (rule id or mistaken terminal symbol)
        if(rand <= (sum+=it.second)) return it.first;

    return 0;
}


