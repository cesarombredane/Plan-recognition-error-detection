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

class CDMMin: public CognitiveDistressManagement {
    public :
        CDMMin(solver * s, extendedPlanLibrary* _epl);

        ~CDMMin();

        void updateAlpha() override;
        void updateAnormalBehavior() override;
};

class CDMSum: public CognitiveDistressManagement {
    public :
        CDMSum(solver * s, extendedPlanLibrary* _epl, double precision);

        ~CDMSum();

        void updateAlpha() override;
        void updateAnormalBehavior() override;

    private :
        double precision;
};

class CDMSupport: public CognitiveDistressManagement {
    public :
        CDMSupport(solver * s, extendedPlanLibrary* _epl, double precision);

        ~CDMSupport();

        void updateAlpha() override;
        void updateAnormalBehavior() override;

    private :
        double precision;
};

#endif // COGNITIVEDISTRESSMANAGEMENT_H
