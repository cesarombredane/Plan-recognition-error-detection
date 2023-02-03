#include "vector"
#include "map"
#include "set"
#include "iostream"
#include "algorithm"

#include <extendedPlanLibrary.h>
#include <solver.h>

using namespace std;

#ifndef COGNITIVEDISTRESSMANAGEMENT_H
#define COGNITIVEDISTRESSMANAGEMENT_H

class CognitiveDistressManagement {
    public :
        CognitiveDistressManagement(solver * s, extendedPlanLibrary* _epl);
        CognitiveDistressManagement();

        ~CognitiveDistressManagement();

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
        map<int,set<int>> filterAAlpha();
};

class minCDM: public CognitiveDistressManagement {
    public :
        minCDM(solver * s, extendedPlanLibrary* _epl);

        ~minCDM();

        void updateAlpha() override;
        void updateAnormalBehavior() override;
};

class sumCDM: public CognitiveDistressManagement {
    public :
        sumCDM(solver * s, extendedPlanLibrary* _epl, double precision);

        ~sumCDM();

        void updateAlpha() override;
        void updateAnormalBehavior() override;

    private :
        double precision;
};

class supportCDM: public CognitiveDistressManagement {
    public :
        supportCDM(solver * s, extendedPlanLibrary* _epl, double precision);

        ~supportCDM();

        void updateAlpha() override;
        void updateAnormalBehavior() override;

    private :
        double precision;
};

#endif // COGNITIVEDISTRESSMANAGEMENT_H
