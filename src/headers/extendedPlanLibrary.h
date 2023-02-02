#include "tuple"
#include "string"
#include "random"
#include "algorithm"

#include <planLibrary.h>
#include <probabilityDistribution.h>

#ifndef EXTENDEDPLANLIBRARY_H
#define EXTENDEDPLANLIBRARY_H

class extendedPlanLibrary {
    public:
        explicit extendedPlanLibrary(float = 0.0, int = 5, int = 10, int = 2, int = 3, int = 2);

        ~extendedPlanLibrary();

        string toString();

        planLibrary* ePlanLibrary;
        probabilityDistribution ruleDecisionModel;
        probabilityDistribution noisePrediction;
        map<string,int> ids;
        map<int, string> revIds;
};

#endif // EXTENDEDPLANLIBRARY_H
