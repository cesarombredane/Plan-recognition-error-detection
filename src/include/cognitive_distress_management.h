/*!
 *  \file CognitiveDistressManagement.h
 *  \brief Detection of anomaly based on plan detection
 *  Calculation of probability to observ an anomaly at time t
 *  Three different methodes are used to compute 𝑃(𝐴𝑡)
 *
 *  \version 2.0
 *  \author César Ombredane
 */

#include "extended_plan_library.h"
#include "solver.h"
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <algorithm>

using namespace std;

#ifndef COGNITIVEDISTRESSMANAGEMENT_H
#define COGNITIVEDISTRESSMANAGEMENT_H

/*!
 *  \class CognitiveDistressManagement
 *  \brief CognitiveDistressManagement is a virtual class used to gather all three methodes
 *  𝑃(𝐴𝑡) = 𝑃(𝐴𝑡−1) + 𝑃(𝐴𝑡|¬𝐴𝑡−1) (1 − 𝑃(𝐴𝑡−1))
 */
class CognitiveDistressManagement
{
public:
    /*!
     *  \brief Constructor
     *  CognitiveDistressManagement constructor
     *
     *  \param s : Plan recognition algorithme
     *  \param _epl : Plan library
     */
    CognitiveDistressManagement(solver *s, extendedPlanLibrary *_epl);

    /*!
     *  \brief Constructor
     *  CognitiveDistressManagement constructor
     */
    CognitiveDistressManagement();

    /*!
     *  \brief Destructor
     *  CognitiveDistressManagement destructor
     */
    ~CognitiveDistressManagement();

    /*!
     *  \brief Getter prob
     */
    double getProb();

    /*!
     *  \brief Getter anormal behavior
     */
    bool getAB();

    /*!
     *  \brief Update
     *  update the  CognitiveDistressManagement at each new observation
     */
    void update();

    /*!
     *  \brief updating alpha
     *  alpha = 𝑃(𝐴𝑡|¬𝐴𝑡−1)
     */
    virtual void updateAlpha() {}

    /*!
     *  \brief updating Anormal behavior
     */
    virtual void updateAnormalBehavior() {}

protected:
    solver *s;                /*!< pointer to plan recognition */
    extendedPlanLibrary *ePL; /*!< pointer to plan library */

    bool AnormalBehavior;                          /*!< anomaly at time t */
    double proba;                                  /*!< 𝑃(𝐴𝑡) */
    double alpha;                                  /*!< 𝑃(𝐴𝑡|¬𝐴𝑡−1) */
    map<int, vector<solverParticle>> oldParticles; /*!< save of plan recognition particles */

    /*!
     *  \brief Getter goals
     *  get goals and their probability to append
     */
    map<int, float> getGoals();

    /*!
     *  \brief Getter actions
     *  get actions and their probability to append
     */
    map<int, float> getActions();

    /*!
     *  \brief Getter Anctions
     *  get all the actions which doesn't fit with the plan
     */
    map<int, set<int>> filterAAlpla();
};

/*!
 *  \class CDMmin
 *  \brief CDMmin implement the method 'Min' to compute 𝑃(𝐴𝑡|¬𝐴𝑡−1)
 *  here, ∀𝑎∈𝐴, ∀𝑒∈𝐸, 𝑃(𝐴𝑡|¬𝐴𝑡−1) = 𝑚𝑖𝑛(𝛿𝑎∉e𝑃(𝑎)𝑃(𝑒))
 *  A : all the next possible actions
 *  E : all the possible plan
 */
class CDMmin : public CognitiveDistressManagement
{
public:
    /*!
     *  \brief Constructor
     *  CDMmin constructor
     *
     *  \param s : Plan recognition algorithme
     *  \param _epl : Plan library
     */
    CDMmin(solver *s, extendedPlanLibrary *_epl);

    /*!
     *  \brief Destructor
     *  CDMmin destructor
     */
    ~CDMmin();

    void updateAlpha();
    void updateAnormalBehavior();
};

/*!
 *  \class CDMsum
 *  \brief CDMsum implement the method 'Sum' to compute 𝑃(𝐴𝑡)
 *  here, 𝑃(𝐴𝑡) = 𝑃(𝑃(𝐴𝑡−1) + 𝑃(𝐴𝑡|¬𝐴𝑡−1) (1 − 𝑃(𝐴𝑡−1)) > 𝑠)
 *  and, ∀𝑎∈𝐴, ∀𝑒∈𝐸, 𝑃(𝐴𝑡|¬𝐴𝑡−1) = ΣΣ(𝛿𝑎∉e𝑃(𝑎)𝑃(𝑒))
 *  A : all the next possible actions
 *  E : all the possible plan
 *  s : 1 - precision
 */
class CDMsum : public CognitiveDistressManagement
{
public:
    /*!
     *  \brief Constructor
     *  CDMsum constructor
     *
     *  \param s : Plan recognition algorithme
     *  \param _epl : Plan library
     *  \param precision : precision : (1 - s)
     */
    CDMsum(solver *s, extendedPlanLibrary *_epl, double precision);

    /*!
     *  \brief Destructor
     *  CDMsum destructor
     */
    ~CDMsum();

    void updateAlpha();
    void updateAnormalBehavior();

private:
    double precision; /*!< precision = 1 - s */
};

/*!
 *  \class CDMsupport
 *  \brief CDMsum implement the method 'Support' to compute 𝑃(𝐴𝑡|¬𝐴𝑡−1)
 *  this is the only methos which work only with particle filter plan recognition
 *  here, 𝑃(𝐴𝑡|¬𝐴𝑡−1)=𝑃(𝑛𝑏𝑃𝑠/nb𝑃 < 𝑠)
 *  nbP : number of particles
 *  nbPs : number of particles which support the observation
 *  s : precision
 */
class CDMsupport : public CognitiveDistressManagement
{
public:
    /*!
     *  \brief Constructor
     *  CDMsupport constructor
     *
     *  \param s : Plan recognition algorithme
     *  \param _epl : Plan library
     *  \param precision : precision : s
     */
    CDMsupport(solver *s, extendedPlanLibrary *_epl, double precision);

    /*!
     *  \brief Destructor
     *  CDMsupport destructor
     */
    ~CDMsupport();

    void updateAlpha();
    void updateAnormalBehavior();

private:
    double precision /*!< precision = s */;
};

#endif // COGNITIVEDISTRESSMANAGEMENT_H
