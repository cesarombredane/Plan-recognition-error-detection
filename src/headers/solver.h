#include "vector"
#include "map"
#include "iostream"

#include <extendedPlanLibrary.h>
#include <rule.h>

#ifndef SOLVER_H
#define SOLVER_H

using namespace std;

class treeNode {
    public:
        treeNode();
        treeNode(extendedPlanLibrary* _epl, int _symbol);

        int update();

        extendedPlanLibrary* epl{};
        int symbol{};
        int rule{};
        bool status{};
        map<int,treeNode> children;
};

class tree {
    public:
        tree();
        tree(extendedPlanLibrary* _epl, int goal);

        vector<int> update();

        extendedPlanLibrary* epl{};
        treeNode root;
        int currentTerminalFO{};
};

class solverParticle {
    public:
        explicit solverParticle(extendedPlanLibrary* _epl);

        bool update();

        extendedPlanLibrary* epl;
        int goal;
        vector<int> expNextObs;
        tree planTree;
};

class solver {
    public:
        solver();
        solver(extendedPlanLibrary* _epl, int _nbParticle);

        virtual ~solver();

        map<int, int> getGoals();
        int getMaxGoal();
        int getLastObservation();
        const size_t& getSize() const;
        bool status();

        bool addObservation(int obs);
        pair<int,vector<int>> generatePlan();


    private:
        extendedPlanLibrary* epl{};
        size_t nbParticles{};
        vector<int> previousObservations;
        map<int, vector<solverParticle>> particles;

    friend class CognitiveDistressManagement;
};

#endif // SOLVER_H
