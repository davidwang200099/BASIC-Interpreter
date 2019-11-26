#include "coderunner.h"

string readOrder(const string &s, int start = 0, int *p = NULL) {
    string order;
    Rank i = start;
    while(i < s.size() && s[i] == ' ') i++;
    while(i < s.size() && isLGVN(s[i])) order += s[i++];
    if(p) *p = i;
    return order;
}

bool isTHEN(const string &s,Rank start){
    if(start<0) return false;
    if(start+4>s.size()) return false;
    return s[start]=='T'&&s[start+1]=='H'&&s[start+2]=='E'&&s[start+3]=='N';
}

Coderunner::Coderunner() : currentLine(0) {
    expressWelcome();
}

InputType Coderunner::processOrder(const string &lineofcode) {
    Rank start = 0;
    string ordertoRunner = readOrder(lineofcode, start, &start);
    if(ordertoRunner == "QUIT") return QUIT_ORDER;
    if(ordertoRunner == "LIST") {
        listCodes();
        return NORMAL_ORDER;
    }
    if(ordertoRunner == "WRITE") {
        while(isspace(lineofcode[start])) start++;
        string filename = string(lineofcode, start);
        if(!filename.empty()) writeToFile(filename);
        else writeToFile("out.vb");
        return NORMAL_ORDER;
    }
    if(ordertoRunner == "CLEARBUFFER") {
        codes.clear();
        vars.clear();
        return NORMAL_ORDER;
    }
    if(ordertoRunner == "CLEAR") {
        system("clear");
        expressWelcome();
        return NORMAL_ORDER;
    }
    if(ordertoRunner == "RUN") {
        runCodes();
        return NORMAL_ORDER;
    }
    if(ordertoRunner == "RUNFILE") {
        while(isspace(lineofcode[start])) start++;
        string filename = string(lineofcode, start);
        if(!filename.empty()) runFile(filename);
        else cout << "File name empty!\n";
        return NORMAL_ORDER;
    }
    if(ordertoRunner == "LOAD") {
        while(isspace(lineofcode[start])) start++;
        string filename = string(lineofcode, start);
        if(!filename.empty()) loadfromFile(filename);
        else cout << "File name empty!\n";
        return NORMAL_ORDER;
    }
    return BASIC_CODE;
}

void Coderunner::run() {
    string lineofcode;
    while(true) {
        getline(cin, lineofcode, '\n');
        //analyse what kind of input it is.Order to the coderunner/BASIC code.
        InputType inputtype = processOrder(lineofcode);
        if(inputtype == QUIT_ORDER) return;
        else {
            if(inputtype == BASIC_CODE) {
                Linecode newcode = Linecode(lineofcode);

                if(newcode.lineno == INT32_MIN) {
                    try{runCode(lineofcode);}
                    catch(int x){cout<<"Execution error of single line of code.\n";}
                } else {
                    Rank r = codes.binarysearch(newcode);
                    if(r>=0&&newcode.lineno==codes[r].lineno) codes[r]=newcode;
                    else codes.insert(newcode,r+1);
                }

            }
        }
    }
}


void Coderunner::assignVar(const Linecode &code) {
    Rank i = 0;
    const string &expression = code.expression;
    string newvarname;

    if(expression.empty()) throw Exception("Syntax error:assigning expression lost.");//avoid "LET"

    while(i < expression.size() && isspace(expression[i])) i++;//skip space

    if(isLGVNS(expression[i]))
        newvarname = readVar(expression, i, &i);
    else throw Exception("Syntax error:assigning expression lost");//avoid "LET    "

    while(i < expression.size() && isspace(expression[i])) i++;
    if(i == expression.size()) throw Exception("Syntax error:assigned value lost.");//avoid "LET   A    "

    if(expression[i] == '=') i++;
    else throw Exception("Syntax error:invalid syntax.");//avoid strange expressions such as "LET A  !"

    Calcresult result = evaluate(string(expression, i), vars);//avoid illegal expression
    NamedVar var(newvarname);
    Rank r = vars.search(var);
    if(r != -1) {
        if(result.flag) vars[r].value = result.result;
        else throw Exception("Expression error.");
    } else {
        if(result.flag) vars.push_back(NamedVar(newvarname,result.result));
        else throw Exception("Expression error.");
    }
}


void Coderunner::jumpto(const Linecode &code) {
    int nextLineNumber = readNumber(code.expression, 0);
    Linecode lcode(nextLineNumber);
    currentLine = codes.search(lcode) - 1;
    if(currentLine < -1) {
        throw Exception("Line number not found.");
    }
}

void Coderunner::inputValue(const Linecode &code) noexcept {
    int input;
    cin >> input;
    cin.get();
    string varname = readVar(code.expression);
    NamedVar var(varname);
    Rank r = vars.search(var);
    if(r >= 0) vars[r].value = input;
    else {var.value=input;vars.push_back(var);}
}

void Coderunner::runCode(const Linecode &code) {
    try {
        switch(code.order) {
            case REM:
                break;
            case LET:
                assignVar(code);
                break;
            case PRINT:
                printExpression(code);
                break;
            case GOTO:
                jumpto(code);
                break;
            case INPUT:
                inputValue(code);
                break;
            case END:
                break;
            case IF:
                conditionCtrl(code);
                break;
            default:
                throw Exception("Undefined order.");
        }
    }
    catch(Exception ex){
        cout<<"\"\t"<<code.code<<"\t\""<<endl;
        cout<<ex.what()<<endl;
        throw -1;
    }
}

void Coderunner::runCodes() {
    vars.clear();
    currentLine = 0;
    while(currentLine < codes.size()) {
        //if(!runCode(codes[currentLine])) {cout<<"Process terminated with exit code -1.\n";return FAIL;}
        try{ runCode(codes[currentLine]); }
        catch(int x){cout<<"Execution terminated with exit code -1.\n";break; }
        currentLine++;
    }
    vars.clear();
    cout<<"Execution terminated with exit code 0.\n";
}

void Coderunner::runFile(const string &filename) {
    ifstream file;
    string lineofcode;
    file.open(filename);

    if(!file) throw Exception("Can not execute file '" + filename + "' !\n");

    codes.clear();
    while(!file.eof()) {
        getline(file, lineofcode, '\n');
        codes.push_back(Linecode(lineofcode));
    }
    file.close();
    try{
        runCodes();
    }
    catch(Exception &ex){
        cout<<ex.what()<<endl;
    }
    /*if(runCodes()) {
        codes.clear();

    }
    else {
        codes.clear();

    }*/
    codes.clear();
}

void Coderunner::loadfromFile(const string &filename) {
    ifstream file;
    string lineofcode;
    file.open(filename);
    if(!file) {
        cout << "Can not load from file '" << filename << "' !\n";
        return ;
    }
    codes.clear();
    while(!file.eof()) {
        getline(file, lineofcode, '\n');
        codes.push_back(Linecode(lineofcode));
    }
    file.close();
    cout<<"Successfully load codes from file '"<<filename<<"'.\n";
}

void Coderunner::writeToFile(const string &name) {
        ofstream file;
        file.open(name);
        if(!file) {
            throw Exception("Can not open file '"+name+"' !\n");
        }
        if(!codes.empty()) {
            for(int i = 0; i < codes.size(); i++) {
                file << codes[i].code << endl;
            }
            file << "END";
            cout << "Write to file \"" << name << "\" successfully.\n";
        }
        file.close();
}

void Coderunner::printExpression(const Linecode &code) {
    Calcresult calresult = evaluate(code.expression, vars);
    if(calresult.flag) cout << calresult.result << endl;
    else {throw Exception("Expression error.");}
}

void Coderunner::judgeCondition(const string &expression,Vector<NamedVar> &vars,bool &result) {
    Calcresult calresult=evaluate(expression,vars);
    if(calresult.flag) result=bool(calresult.result);
    else {result=false;throw Exception("Expression error in the if-clause.");}
}


void Coderunner::conditionCtrl(const Linecode &code){
        Rank hi = 0;
        string expression = code.expression;
        bool ctrlcondition;
        string conditionexp;

        while(!isTHEN(expression, hi)) {
            if(isLGEXP(expression[hi])) conditionexp += expression[hi];
            else throw Exception("Expression error in the if-clause.");
            hi++;
        }
        hi += 4;

        if(conditionexp.empty()) throw Exception("Empty if-clause.");

        judgeCondition(conditionexp, vars, ctrlcondition);


        /*
         * if the condition is false,python will not analyse the grammar of the "then" clause.
         */
        if(ctrlcondition) {Linecode exec(string(expression, hi));runCode(exec);}
}

void Coderunner::listCodes() noexcept {
    if(codes.empty()) {
        cout << "No line of code stored.\n";
        return ;
    }
    for(int i = 0; i < codes.size(); i++)
        cout << codes[i].code << endl;
}

