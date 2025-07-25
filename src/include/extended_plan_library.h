/*!
 *  \file ExtendedPlanLibrary.h
 *  \brief a plan library with additional information
 *
 *  \version 1.0
 *  \author Jean Massardi
 */

#include "plan_library.h"
#include "probability_distribution.h"
#include <utility>
#include <map>

#ifndef EXTENDEDPLANLIBRARY_H
#define EXTENDEDPLANLIBRARY_H

/*!
 *  \class extendedPlanLibrary
 *  \brief a plan library with additional information
 *  additional information :
 *  - representation of noise
 *  - representation of rule's probability to form a nonTerminal
 *  - map to convert action number to action name and vice versa
 */
class extendedPlanLibrary
{
public:
    /*!
     *  \brief Constructor
     *  extendedPlanLibrary constructor
     *
     *  \param spl : string representation of an extended plan library
     */
    extendedPlanLibrary(const char *spl);

    /*!
     *  \brief Constructor
     *  extendedPlanLibrary constructor
     *
     *  \param _ePlanLibrary : pointer to a plan library
     */
    extendedPlanLibrary(planLibrary *_ePlanLibrary);

    /*!
     *  \brief Constructor
     *  extendedPlanLibrary constructor
     *
     *  \param epl : memory address of an extended plan library
     */
    extendedPlanLibrary(const extendedPlanLibrary &epl);

    /*!
     *  \brief Constructor
     *  extendedPlanLibrary constructor
     *
     *  \param noisePRCT : noise orediction
     *  \param goal : number of goals
     *  \param _size : number of actions
     *  \param depth : depth of plan tree
     *  \param widthAND : number of action to form a rule
     *  \param widthOR : number of different rules to form a nonTerminal action
     */
    extendedPlanLibrary(float noisePRCT = 0.0, int goal = 5, int _size = 10, int depth = 2, int widthAND = 3, int widthOR = 2);

    /*!
     *  \brief Destructor
     *  extendedPlanLibrary destructor
     */
    virtual ~extendedPlanLibrary();

    /*!
     *  \brief String representation of the PlanLibrary
     */
    const string toString();

    planLibrary *ePlanLibrary; /*!< plan library */

    probabilityDistribution decisionModel; /*!< probability distribution used to know the probability to use a rule to form a non Terminal action */
    probabilityDistribution noise;         /*!< probability distribution used to represent the noise */

    map<string, int> ids;    /*!< map to convert actions names to actions numbers */
    map<int, string> revIds; /*!< map to convert actions numbers to actions names */
};

#endif // EXTENDEDPLANLIBRARY_H
