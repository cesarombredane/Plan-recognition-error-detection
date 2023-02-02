#include "vector"
#include "map"
#include "iostream"

#include <extendedPlanLibrary.h>
#include <rule.h>

#ifndef SOLVER_H
#define SOLVER_H

using namespace std;

/*!
 *  \class treeNode
 *  \brief a node from a plan tree
 */
class treeNode {
    public:
        /*!
         *  \brief Constructor
         *  treeNode constructor
         */
        treeNode();

        /*!
         *  \brief Constructor
         *  treeNode constructor
         *
         *  \param _epl : pointer to an extended plan library
         *  \param _symbol : action represented by the node
         */
        treeNode(extendedPlanLibrary* _epl, int _symbol);

        /*!
         *  \brief updating the tree node
         *
         *  \return an expected next observation
         */
        int update();

        extendedPlanLibrary* epl{};/*!< pointer to an extended plan library */
        int symbol{};/*!< the action represented by the node */
        int _rule{};/*!< the rule used to form the goal */
        bool status{};/*!< is the current action ended (is a Terminal action or all the children are ended) */
        map<int,treeNode> children;/*!< all the children */
};

/*!
 *  \class tree
 *  \brief plan tree
 */
class tree {
    public:
        /*!
         *  \brief Constructor
         *  tree constructor
         */
        tree();

        /*!
         *  \brief Constructor
         *  tree constructor
         *
         *  \param _epl : pointer to an extended plan library
         *  \param goal : goal of the plan tree
         */
        tree(extendedPlanLibrary* _epl, int goal);

        /*!
         *  \brief updating the tree
         *
         *  \return a list of expected next observations
         */
        vector<int> update();

        /*!
         *  \brief updating the tree root
         *
         *  \return an expected next observation
         */
        int updateFO();

        extendedPlanLibrary* epl{};/*!< pointer to an extended plan library */
        treeNode root;/*!< the root of the plan tree */
        int currentTerminalFO{};
};

/*!
 *  \class solverParticle
 *  \brief one particle of the solver
 *  it represent the particle's goal, the plan tree and the expected next observation
 */
class solverParticle {
    public:
        /*!
         *  \brief Constructor
         *  solverParticle constructor
         *
         *  \param _epl : pointer to an extended plan library
         */
        explicit solverParticle(extendedPlanLibrary* _epl);

        /*!
         *  \brief updating the particle
         *
         *  \return if there are expected next observation
         */
        bool update();

        extendedPlanLibrary* epl;/*!< pointer to an extended plan library */
        int goal;/*!< the goal of the particle */
        vector<int> expNextObs;/*!< the expected next observations */
        tree planTree;/*!< the plan tree */
};

/*!
 *  \class solver
 *  \brief Particle filter plan recognition
 *  the solver is used as a particle filter to recognise plan.
 *  it generate, sort and select particles which fit the observation.
 */
class solver {
    public:
        /*!
         *  \brief Constructor
         *  solver constructor
         */
        solver();

        /*!
         *  \brief Constructor
         *  solver constructor
         *
         *  \param _epl : pointer to an extended plan library
         *  \param _nbParticle : number of particles
         */
        solver(extendedPlanLibrary* _epl, int _nbParticle);

        /*!
         *  \brief Destructor
         *  solver destructor
         */
        virtual ~solver();

        /*!
         *  \brief Getter Goals
         *
         *  \return all the possibles goals and how many particles support them
         */
        map<int, int> getGoals();

        /*!
         *  \brief Getter max goal
         *
         *  \return the goal with the highest probability to append
         */
        int getMaxGoal();

        /*!
         *  \brief Getter observation
         *
         *  \return the last observation
         */
        int getLastObservation();

        /*!
         *  \brief Getter nbParticles
         */
        const size_t& getSize() const;

        /*!
         *  \brief Add observation
         *
         *  \param obs : the action number of the observation
         */
        bool addObservation(int obs);

        /*!
         *  \brief generate a random plan
         *
         *  \return a goal and an ordered set of action which form the goal
         */
        pair<int,vector<int>> generatePlan();

        /*!
         *  \brief Getter particles
         *
         *  \return if there are particles in the particle filter
         */
        bool status();

    private:
        extendedPlanLibrary* epl{};/*!< pointer to an extended plan library */
        size_t nbParticles{};/*!< number of particles */
        vector<int> previousObservations;/*!< ordered list of all the previous observation */
        map<int, vector<solverParticle>> particles;/*!< all the particles sort by goals */

    friend class CognitiveDistressManagement;
};

#endif // SOLVER_H
