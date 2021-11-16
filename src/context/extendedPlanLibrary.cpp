#include "src/headers/extendedPlanLibrary.h"

planLibrary currentPL = planLibrary();

unordered_map<int,rule> prs_rules;
map<string,int> prs_ids;
vector<tuple<string, int, float>> prs_dm;
vector<tuple<string, string, float>> prs_noise;

int currentSym;
int currentRule;
int tempConstraint;
string currentId;
string currentNSym;

// statics
static bool ar_goal(const char* lexem, size_t len) {
    currentPL.addSymbol(currentSym, false, true);
    if(prs_ids.find(string(lexem,len)) == prs_ids.end()) {
        prs_ids[string(lexem,len)] = currentSym;
        ++currentSym;
    }
    return true;
}
static bool ar_terminals(const char* lexem, size_t len) {
    currentPL.addSymbol(currentSym, true, false);
    if(prs_ids.find(string(lexem,len)) == prs_ids.end()) {
        prs_ids[string(lexem,len)] = currentSym;
        ++currentSym;
    }
    return true;
}
static bool ar_nonTerminals(const char* lexem, size_t len) {
    currentPL.addSymbol(currentSym, false, false);
    if(prs_ids.find(string(lexem,len)) == prs_ids.end()) {
        prs_ids[string(lexem,len)] = currentSym;
        ++currentSym;
    }
    return true;
}
static bool ar_newId(const char* lexem, size_t len) {
    currentRule = stoi(string(lexem,len));
    return true;
}
static bool ar_newRule(const char* lexem, size_t len) {
    prs_rules[currentRule] = rule(prs_ids[string(lexem, len)], currentRule);
    return true;
}
static bool ar_newChild(const char* lexem, size_t len) {
    prs_rules[currentRule].addChild(prs_ids[string(lexem, len)]);
    return true;
}
static bool ar_tempCst(const char* lexem, size_t len) {
    tempConstraint = stoi(string(lexem, len));
    return true;
}
static bool ar_newCst(const char* lexem, size_t len) {
    prs_rules[currentRule].addConstraint(make_pair(tempConstraint, stoi(string(lexem, len))));
    return true;
}
static bool ar_dmId(const char* lexem, size_t len) {
    currentId = string(lexem, len);
    return true;
}
static bool ar_dmR(const char* lexem, size_t len) {
    currentRule = stoi(string(lexem, len));
    return true;
}
static bool ar_dmProb(const char* lexem, size_t len) {
    prs_dm.emplace_back(currentId, currentRule, stof(string(lexem, len)));
    return true;
}
static bool ar_noiseId(const char* lexem, size_t len) {
    currentId = string(lexem, len);
    return true;
}
static bool ar_noiseSym(const char* lexem, size_t len) {
    currentNSym = string(lexem, len);
    return true;
}
static bool ar_noiseProb(const char* lexem, size_t len) {
    prs_noise.emplace_back(currentId, currentNSym, stof(string(lexem, len)));
    return true;
}

// constructor
extendedPlanLibrary::extendedPlanLibrary(const char* spl) {
    currentPL = planLibrary();

    prs_rules = unordered_map<int,rule> ();
    prs_ids = map<string,int>();
    prs_ids["root"] = -1;

    currentSym = 0;
    currentRule = 0;
    tempConstraint = 0;

    Token digit('0','9');
    Token letter('a','z');
    letter.Add('_');
    letter.Add('A','Z');

    Lexem number = !Token("-") + 1 * digit;
    Lexem val = !Token("-") + Token('0') + Token('.') + 1 * digit | Token('1') + Token('.') + 1 * Token('0');
    Lexem name = 1*letter;

    Lexem goals = "Goals";
    Lexem terminals = "Terminals";
    Lexem nonTerminals = "NonTerminals";
    Lexem rules = "Rules";
    Lexem dm = "DecisionModel";
    Lexem nse = "Noise";
    Lexem delimiter = ",";
    Lexem newLine = "\n";

    Rule r_line_goals = goals + Token('(') + name + ar_goal + *(delimiter + name + ar_goal) + Token(')');
    Rule r_line_terminals = terminals + Token('(') + name + ar_terminals + *(delimiter + name + ar_terminals) + Token(')');
    Rule r_line_nonTerminals = nonTerminals + Token('(') + name + ar_nonTerminals + *(delimiter + name + ar_nonTerminals) + Token(')');
    Rule r_rule_children = name + ar_newChild + *(delimiter + name+ ar_newChild );
    Rule r_rule_constraints = *(Token('(') + number + ar_tempCst + delimiter + number + ar_newCst +Token(')') + !delimiter);
    Rule r_rule = number + ar_newId + Token('(') + name + ar_newRule + Token('(') + r_rule_children + Token(')') + delimiter + Token('(') + r_rule_constraints + Token(')') + Token(')');
    Rule r_rules = rules + Token('(') + r_rule + *(delimiter + r_rule) + Token(')');
    Rule r_dm_prob = number + ar_dmR+ Token('(') + val + ar_dmProb + Token(')');
    Rule r_dm_Id = name + ar_dmId + Token('(') +  r_dm_prob + *(delimiter + r_dm_prob) + Token(')');
    Rule r_dm = dm +  Token('(') + r_dm_Id + *(delimiter + r_dm_Id) + Token(')');
    Rule r_noise_prob = name + ar_noiseSym + Token('(') + val + ar_noiseProb + Token(')');
    Rule r_noise_Id = name + ar_noiseId + Token('(') + r_noise_prob + *(delimiter + r_noise_prob) + Token(')');
    Rule r_noise = nse + Token('(') + r_noise_Id + *(delimiter + r_noise_Id) + Token(')');
    Rule root = r_line_goals + r_line_terminals + r_line_nonTerminals + r_rules + !r_dm + !r_noise;

    const char* tail = nullptr;
    int tst = Analyze(root, spl, &tail);

    if (tst > 0) cout << "Plan Library configured"  << endl;
    else cout << "Parsing errors detected, status = " << tst << endl << "stopped at: " << tail << endl;

    for(const auto& it : prs_rules) currentPL.addRule(it.second);

    this->ids = prs_ids;

    for(const auto& it : this->ids) this->revIds[it.second] = it.first;

    this->ePlanLibrary = &currentPL;
    this->decisionModel = probabilityDistribution(*this->ePlanLibrary);

    for(auto it : prs_dm) this->decisionModel.setProb(this->ids[get<0>(it)], get<1>(it), get<2>(it));

    this->noise = probabilityDistribution(this->ePlanLibrary->getTerminals());

    for(auto it : prs_noise) {
        if(get<1>(it) == "miss") this->noise.setProb(this->ids[get<0>(it)], -1, get<2>(it));
        else if(get<1>(it) == "ext") this->noise.setProb(this->ids[get<0>(it)], -2, get<2>(it));
        else this->noise.setProb(this->ids[get<0>(it)], this->ids[get<1>(it)], get<2>(it));
    }
}
extendedPlanLibrary::extendedPlanLibrary(planLibrary* _ePlanLibrary) {
    this->ePlanLibrary = _ePlanLibrary;
    this->decisionModel = probabilityDistribution(*this->ePlanLibrary);
    this->noise = probabilityDistribution(this->ePlanLibrary->getTerminals(), 0);
}
extendedPlanLibrary::extendedPlanLibrary(const extendedPlanLibrary& epl) {
    delete this->ePlanLibrary;
    this->ePlanLibrary = epl.ePlanLibrary;
    this->decisionModel = epl.decisionModel;
    this->noise = epl.noise;
}
extendedPlanLibrary::extendedPlanLibrary(float noisePRCT, int goal, int _size, int depth, int widthAND, int widthOR) {
    ePlanLibrary = new planLibrary();

    int idPrim = 0;
    int idRule = 0;
    char currentChar = 'a';
    string currentString = "G_";
    this->ids["root"] = -1;
    this->ePlanLibrary->addSymbol(-1, false, false);

    while (idPrim < goal) {
        this->ePlanLibrary->addSymbol(idPrim, false, true);
        this->ids[currentString + currentChar] = idPrim;

        if(currentChar == 'z') {
            currentChar = 'a';
            currentString += currentChar;
        }
        else ++currentChar;

        rule g = rule(-1, idRule);
        g.addChild(idPrim);
        ++idRule;
        this->ePlanLibrary->addRule(g);

        for(int i = 0; i < widthOR; ++i) {
            rule r = rule(idPrim, idRule);
            ++idRule;

            random_device seeder;
            mt19937 engine(seeder());
            uniform_int_distribution<> dist1(_size, _size + _size - 1);
            uniform_int_distribution<> dist2(0, 2);
            for(int j = 0; j < widthAND; ++j) {
                r.addChild(dist1(engine));

                for(int rSym = 0; rSym < j; ++rSym) if(dist2(engine) == 2) r.addConstraint(make_pair(rSym, j));
            }

            this->ePlanLibrary->addRule(r);
        }

        ++idPrim;
    }

    idPrim = _size;
    currentString = "S_";
    currentChar = 'a';

    for(int i = 1; i < depth; ++i) {
        while(idPrim < _size * (i + 1)) {
            this->ePlanLibrary->addSymbol(idPrim, false, false);
            this->ids[currentString + currentChar] = idPrim;

            if(currentChar == 'z') {
                currentChar = 'a';
                currentString += currentChar;
            }
            else ++currentChar;


            for(int j = 0; j < widthOR; ++j) {
                rule r = rule(idPrim, idRule);
                ++idRule;

                random_device seeder;
                mt19937 engine(seeder());
                uniform_int_distribution<> dist1(_size, _size + _size * (i + 1) - 1);
                uniform_int_distribution<> dist2(0, 2);
                for(int k = 0; k < widthAND; ++k) {
                    r.addChild(dist2(engine));

                    for(int rSym = 0; rSym < k; ++rSym) if(dist2(engine) == 2) r.addConstraint(make_pair(rSym, k));
                }

                this->ePlanLibrary->addRule(r);
            }

            ++idPrim;
        }
    }

    currentString = "A_";
    currentChar = 'a';

    while (idPrim < _size * (depth + 1)) {
        this->ePlanLibrary->addSymbol(idPrim, true, false);
        this->ids[currentString + currentChar ]= idPrim;

        if(currentChar == 'z') {
            currentChar = 'a';
            currentString += currentChar;
        }
        else ++currentChar;

        ++idPrim;
    }

    this->decisionModel = probabilityDistribution(*this->ePlanLibrary);
    this->noise = probabilityDistribution(this->ePlanLibrary->getTerminals(), noisePRCT);
    for(const auto& it : this->ids) this->revIds[it.second] = it.first;
}

// destructor
extendedPlanLibrary::~extendedPlanLibrary() = default;

// toString
string extendedPlanLibrary::toString() {
    string result;
    result += "Goals(";
    size_t cnt = 0;

    for (auto itG : this->ePlanLibrary->getGoals()) {
        ++cnt;
        result += this->revIds[itG];
        if(cnt < this->ePlanLibrary->getGoals().size()) result += ',';
    }
    cnt = 0;

    result += ")\n";
    result += "Terminals(";
    for (auto itT : this->ePlanLibrary->getTerminals()) {
        ++cnt;
        result += this->revIds[itT];
        if(cnt < this->ePlanLibrary->getTerminals().size()) result += ',';
    }
    cnt = 0;

    result += ")\n";
    result += "NonTerminals(";
    for (auto itNT : this->ePlanLibrary->getNonTerminals()) {
        ++cnt;
        result += this->revIds[itNT];
        if(cnt < this->ePlanLibrary->getNonTerminals().size()) result += ',';
    }
    cnt = 0;

    result += ")\n";
    result += "Rules(";
    for (auto itR : this->ePlanLibrary->getRules()) {
        ++cnt;
        result += to_string(itR.first);
        result += "(" + itR.second.toString(this->revIds) + ")";
        if(cnt < this->ePlanLibrary->getRules().size()) result += ",\n";
    }
    cnt = 0;

    size_t cnt2;

    result += ')';
    result += '\n';
    result += "DecisionModel(";
    for (const auto& itDM : this->decisionModel.getDistribution()) {
        ++cnt;
        result += this->revIds[itDM.first];
        result += '(';
        cnt2 = 0;
        for (auto itSym : itDM.second) {
            ++cnt2;
            result += to_string(itSym.first) + "(" + to_string(itSym.second) + ")";
            if(cnt2 < itDM.second.size()) result += ',';
        }

        result += ')';
        if(cnt < this->decisionModel.getDistribution().size()) result += ",\n";
    }
    cnt = 0;

    result += ")\n";
    result += "Noise(";
    for (const auto& itN : this->noise.getDistribution()) {
        ++cnt;
        result += this->revIds[itN.first];
        result += '(';
        cnt2 = 0;
        for (auto itSym : itN.second) {
            ++cnt2;

            if(itSym.first == -1) result += "miss(" + to_string(itSym.second) + ")";
            else if(itSym.first == -2) result += "ext(" + to_string(itSym.second) + ")";
            else result += this->revIds[itSym.first] + "(" + to_string(itSym.second) + ")";
            if(cnt2 < itN.second.size()) result += ",";
        }
        result += ')';

        if(cnt < this->noise.getDistribution().size()) result += ",\n";
    }
    result += ")\n";

    return result;
}
