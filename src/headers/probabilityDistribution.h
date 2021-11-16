#include "iostream"
#include "unordered_set"
#include "unordered_map"
#include "random"

#include "planLibrary.h"

using namespace std;

#ifndef PROBABILITYDISTRIBUTION_H
#define PROBABILITYDISTRIBUTION_H

class probabilityDistribution {
    public:
        probabilityDistribution();
        explicit probabilityDistribution(unordered_map<int, unordered_map<int,float>>);
        explicit probabilityDistribution(planLibrary& _pl);
        explicit probabilityDistribution(const unordered_set<int>&, float = 0.0);

        ~probabilityDistribution();

        const unordered_map<int, unordered_map<int,float>>& getDistribution();
        bool isValid() const;
        void setProb(int, int, float);
        int RNC(int i) const;

    private:
        unordered_map<int, unordered_map<int,float>> distribution;
};

#endif // PROBABILITYDISTRIBUTION_H
