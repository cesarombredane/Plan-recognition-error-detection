#include "tuple"
#include "string"
#include "random"
#include "planLibrary.h"
#include "probabilityDistribution.h"
#include "bnflite.h"

using namespace bnf;

#ifndef EXTENDEDPLANLIBRARY_H
#define EXTENDEDPLANLIBRARY_H

class extendedPlanLibrary {
    public:
        explicit extendedPlanLibrary(const char*);
        explicit extendedPlanLibrary(planLibrary*);
        extendedPlanLibrary(const extendedPlanLibrary&);
        explicit extendedPlanLibrary(float = 0.0, int = 5,int = 10, int = 2, int = 3, int = 2);

        ~extendedPlanLibrary();

        string toString();

        planLibrary* ePlanLibrary;
        probabilityDistribution decisionModel;
        probabilityDistribution noise;
        map<string,int> ids;
        map<int, string> revIds;
};

#endif // EXTENDEDPLANLIBRARY_H
