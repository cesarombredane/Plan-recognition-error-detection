#include "extendedPlanLibrary.h"
#include "solver.h"
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <algorithm>

using namespace std;

#ifndef COGNITIVEDISTRESSMANAGEMENT_H
#define COGNITIVEDISTRESSMANAGEMENT_H

class CognitiveDistressManagement {
    public :
        CognitiveDistressManagement(solver * s, extendedPlanLibrary* _epl);

        CognitiveDistressManagement();

        ~CognitiveDistressManagement();

        double getProb();

        bool getAB() const;

        void update();

        virtual void updateAlpha() {}

        virtual void updateAnormalBehavior() {}

    protected :
        solver* s{};
        extendedPlanLibrary* ePL{};

        bool AnormalBehavior{};
        double proba{};
        double alpha{};
        map<int, vector<solverParticle>> oldParticles;

        map<int,float> getGoals(); 

        map<int,float> getActions(); 

        map<int,set<int>> filterAAlpla(); 
};

class CDMmin:public CognitiveDistressManagement {
    public :
        CDMmin(solver * s, extendedPlanLibrary* _epl);

        ~CDMmin();

        void updateAlpha();
        void updateAnormalBehavior();
};

class CDMsum:public CognitiveDistressManagement {
    public :
        CDMsum(solver * s, extendedPlanLibrary* _epl, double precision);

        ~CDMsum();

        void updateAlpha();
        void updateAnormalBehavior();

    private :
        double precision;
};

class CDMsupport:public CognitiveDistressManagement {
    public :
        CDMsupport(solver * s, extendedPlanLibrary* _epl, double precision);

        ~CDMsupport();

        void updateAlpha();
        void updateAnormalBehavior();

    private :
        double precision;
};

#endif // COGNITIVEDISTRESSMANAGEMENT_H
