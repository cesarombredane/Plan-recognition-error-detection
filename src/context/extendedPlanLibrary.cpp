#include "extendedPlanLibrary.h"

// constructor
extendedPlanLibrary::extendedPlanLibrary(float noise, int nbGoal, int nbTerminals, int treeDepth, int nbChildPerRule, int nbPlanPerNonTerminal) {
    // create an empty plan library
    this->ePlanLibrary = new planLibrary();

    // setup of the non-terminal root symbol
    this->ids["root"] = -1;
    this->ePlanLibrary->addSymbol(-1, false, false);

    // init the rule id counter
    int idRuleCounter = 0;
    int idSymbolCounter = 0;

    // init randomizer
    random_device seeder;
    mt19937 engine(seeder());
    uniform_int_distribution<> randomConstraint(0, 2);

    // init the symbol name prefix & counter
    string symbolPrefix = "T_";
    char symbolCounter = 'a';
    for (int i = 0; i < nbTerminals; ++i) {
        // create a new terminal symbol
        this->ePlanLibrary->addSymbol(idSymbolCounter, true, false);
        this->ids[symbolPrefix + symbolCounter] = idSymbolCounter;

        // update symbol prefix & counter
        if(symbolCounter == 'z') {
            symbolCounter = 'a';
            symbolPrefix += '_';
        }
        else ++symbolCounter;

        // update the counter
        ++idSymbolCounter;
    }

    // reset the symbol name prefix
    symbolPrefix = "NT_";
    for(int depthLevel = 1; depthLevel < treeDepth; ++depthLevel) {
        // reset the symbol name counter
        symbolCounter = 'a';
        for (int i = 0; i < nbTerminals; ++i) {
            // create a new non-terminal symbol
            this->ePlanLibrary->addSymbol(idSymbolCounter, false, false);
            this->ids[symbolPrefix + to_string(depthLevel) + "_" + symbolCounter] = idSymbolCounter;

            // update symbol prefix & counter
            if(symbolCounter == 'z') {
                symbolCounter = 'a';
                symbolPrefix += '_';
            }
            else ++symbolCounter;

            // create rules for the non-terminal symbol
            for(int j = 0; j < nbPlanPerNonTerminal; ++j) {
                // create the new rule & update the counter
                rule r = rule(idSymbolCounter, idRuleCounter);
                ++idRuleCounter;

                // init the randomizer for the rule children
                uniform_int_distribution<> randomChildren(nbTerminals * (depthLevel - 1), nbTerminals * depthLevel - 1);

                // add all the children and the constraints to the rule
                for(int posChild = 0; posChild < nbChildPerRule; ++posChild) {
                    // add a random child
                    r.addChild(randomChildren(engine));

                    // iter over the possible constraints to create
                    for(int posConstraint = 0; posConstraint < posChild; ++posConstraint)
                        // randomly add constraint
                        if(randomConstraint(engine) == 2) r.addConstraint(make_pair(posConstraint, posChild));
                }

                // add the rule to the plan library
                this->ePlanLibrary->addRule(r);
            }

            // update the counter
            ++idSymbolCounter;
        }
    }

    // reset the symbol name prefix & counter
    symbolPrefix = "G_";
    symbolCounter = 'a';
    for(int i = 0; i < nbGoal; ++i) {
        // create a new goal symbol
        this->ePlanLibrary->addSymbol(idSymbolCounter, false, true);
        this->ids[symbolPrefix + symbolCounter] = idSymbolCounter;

        // update symbol prefix & counter
        if(symbolCounter == 'z') {
            symbolCounter = 'a';
            symbolPrefix += '_';
        }
        else ++symbolCounter;

        // create the rule that allow to find the goal from the root
        rule g = rule(-1, idRuleCounter);
        g.addChild(idSymbolCounter);
        this->ePlanLibrary->addRule(g);

        // update the counter
        ++idRuleCounter;

        // create rules for the goal symbol
        for(int j = 0; j < nbPlanPerNonTerminal; ++j) {
            // create the new rule & update the counter
            rule r = rule(idSymbolCounter, idRuleCounter);
            ++idRuleCounter;

            // init the randomizer for the rule children
            uniform_int_distribution<> randomChildren(nbTerminals * (treeDepth - 1), nbTerminals * treeDepth - 1);

            for(int posChild = 0; posChild < nbChildPerRule; ++posChild) {
                r.addChild(randomChildren(engine));

                // iter over the possible constraints to create
                for(int posConstraint = 0; posConstraint < posChild; ++posConstraint)
                    // randomly add constraint
                    if(randomConstraint(engine) == 2) r.addConstraint(make_pair(posConstraint, posChild));
            }

            // add the rule to the plan library
            this->ePlanLibrary->addRule(r);
        }

        // update the counter
        ++idSymbolCounter;
    }

    // set the probability distribution for the rule decision model and the noise prediction model
    this->ruleDecisionModel = probabilityDistribution(*this->ePlanLibrary);
    this->noisePrediction = probabilityDistribution(this->ePlanLibrary->getTerminals(), noise);

    // create the reverse id map
    for(const auto& it : this->ids) this->revIds[it.second] = it.first;
}

// destructor
extendedPlanLibrary::~extendedPlanLibrary() = default;

// toString
string extendedPlanLibrary::toString() {
    string result;

    result += "Goals : | ";
    for (auto it : this->ePlanLibrary->getGoals()) result += this->revIds[it] + " | ";

    result += "\nTerminals : | ";
    for (auto it : this->ePlanLibrary->getTerminals()) result += this->revIds[it] + " | ";

    result += "\nNonTerminals : | ";
    for (auto it : this->ePlanLibrary->getNonTerminals()) result += this->revIds[it] + " | ";

    result += "\nRules :\n";
    for (auto it : this->ePlanLibrary->getRules()) result += it.second.toString(&this->revIds) + "\n";

    result += "DecisionModel : \n";
    for (const auto& it : this->ruleDecisionModel.getDistribution()) {
        result += this->revIds[it.first] + " = | ";
        for (auto it2 : it.second) result += to_string(it2.first) + "(" + to_string(it2.second) + ") | ";
        result += "\n";
    }

    result += "Noise : \n";
    for (const auto& it : this->noisePrediction.getDistribution()) {
        result += this->revIds[it.first] + " = | ";
        for (auto it2 : it.second) result += this->revIds[it2.first] + "(" + to_string(it2.second) + ") | ";
        result += "\n";
    }

    return result;
}
