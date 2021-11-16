/*!
 *  \file probabilityDistribution.h
 *  \brief implementation of probability distribution used in plan library
 *
 *  \version 1.0
 *  \author Jean Massardi
 */

#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include "planLibrary.h"

using namespace std;

#ifndef PROBABILITYDISTRIBUTION_H
#define PROBABILITYDISTRIBUTION_H

/*!
 *  \class probabilityDistribution
 *  \brief probabilityDistribution is a class that represent a probability distribution.
 *  it can be used in two ways :
 *  - the probability to use a rule to form a nonTerminal action.
 *  - the probability to cofuse a action with another (noise).
 */
class probabilityDistribution {
    public:
        /*!
         *  \brief Constructor
         *  probabilityDistribution constructor
         */
        probabilityDistribution();

        /*!
         *  \brief Constructor
         *  CognitiveDistressManagement constructor
         *
         *  \param _distribution : probabilityDistribution's argument : distribution
         */
        probabilityDistribution(unordered_map<int, unordered_map<int,float>> _distribution);

        /*!
         *  \brief Constructor
         *  probabilityDistribution constructor
         *
         *  \param _pl : pointer to a plan library
         */
        probabilityDistribution(planLibrary& _pl);

        /*!
         *  \brief Constructor
         *  probabilityDistribution constructor
         *
         *  \param _terminals : unordered set of all the non terminals actions (in the plan library)
         *  \param noise : estimation of the noise
         */
        probabilityDistribution(unordered_set<int> _terminals, float noise = 0.0);

        /*!
         *  \brief Destructor
         *  probabilityDistribution destructor 
         */
        virtual ~probabilityDistribution();

        /*!
         *  \brief Getter distribution 
         */
        const unordered_map<int, unordered_map<int,float>>& getDistribution() const {return distribution;};

        /*!
         *  \brief Setter distribution 
         */
        void setProb(int primitive, int element, float probability);

        /*!
         *  \brief check if the distribution is valid
         *  the sum of all probability in a array must to be equal to 1.
         */
        bool isValid() const;

        /*!
         *  \brief choose a random element of the unordered map at index 'i'
         *  this element is chosen based on his probability.
         *
         *  \param i : index of probability distribution
         *  \return random object in the map contained at index i
         */
        int RNC(int i) const;

    private:
        unordered_map<int, unordered_map<int,float>> distribution;/*!< distribution of probability */
};

#endif // PROBABILITYDISTRIBUTION_H
