#include "coderunner.h"
#include "Console.h"
#include <QDebug>
string readOrder(const string &s, int start = 0, int *p = NULL) {
    string order;
    Rank i = start;
    while(i < s.size() && isspace(s[i])) i++;
    while(i < s.size() && isLGVN(s[i])) order += s[i++];
    if(p) *p = i;
    return order;
}

bool isTHEN(const string &s,Rank start){
    if(start<0) return false;
    if(start+4>s.size()) return false;
    return s[start]=='T'&&s[start+1]=='H'&&s[start+2]=='E'&&s[start+3]=='N';
}

Coderunner::Coderunner(Console *console):
    currentLine(0),console(console),runnermode(INPUTCODE) {}


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
        else writeToFile("OUT.VB");
        return NORMAL_ORDER;
    }
    if(ordertoRunner == "CLEAR") {
        codes.clear();
        vars.clear();
        return NORMAL_ORDER;
    }
    if(ordertoRunner == "CLS") {
        //system("cls");
        console->clear();
        console->expressWelcome();
        return NORMAL_ORDER;
    }
    if(ordertoRunner == "RUN") {
        runnermode=SUCCESSIVERUN;
        currentLine=0;
        vars.clear();
        if(runCodes()!=Pause4UsrInput) runnermode=INPUTCODE;
        //else qDebug()<<"Pause for user input!\n";
        return NORMAL_ORDER;
    }
    if(ordertoRunner == "LOAD") {
            while(isspace(lineofcode[start])) start++;
            string filename = string(lineofcode, start);
            if(!filename.empty()) loadfromFile(filename);
            else console->write("File name empty!\n");
            return NORMAL_ORDER;
    }
    return BASIC_CODE;
}

void Coderunner::assignVar(const Linecode &code) {
    Rank i = 0;
    const string &expression = code.expression;
    string newvarname;

    if(expression.empty())
        throw Exception("Syntax error:assigning expression lost.\n");//avoid "LET"

    while(i < expression.size() && isspace(expression[i])) i++;//skip space

    if(isLGVNS(expression[i]))
        newvarname = readVar(expression, i, &i);
    else throw Exception("Syntax error:assigning expression lost.\n");//avoid "LET    "

    while(i < expression.size() && isspace(expression[i])) i++;
    if(i == expression.size())
        throw Exception("Syntax error:assigned value lost.\n");//avoid "LET   A    "

    if(expression[i] == '=') i++;
    else throw Exception("Syntax error:invalid syntax.\n");//avoid strange expressions such as "LET A  !"

    Calcresult result = evaluate(string(expression, i), vars);//avoid illegal expression
    NamedVar var(newvarname);
    Rank r = vars.search(var);
    if(r != -1) {
        if(result.flag) vars[r].value = result.result;
        else throw Exception("Expression error.\n");
    } else {
        if(result.flag) vars.push_back(NamedVar(newvarname,result.result));
        else throw Exception("Expression error.\n");
    }
}


void Coderunner::jumpto(const Linecode &code) {
    int nextLineNumber = readNumber(code.expression, 0);
    Linecode lcode(nextLineNumber);
    currentLine = codes.search(lcode) - 1;
    if(currentLine <= -1) {
        throw Exception("Line number not found.\n");
    }
}

void Coderunner::inputValue(const Linecode &code) noexcept {
     runnermode=INPUTVALUE;
     string varname=readVar(code.expression);
     NamedVar var(varname);
     Rank r = vars.search(var);//create a new var
     if(r >= 0) currentVar=r;//set currentVar to r for user input
     else {vars.push_back(var);currentVar=vars.size()-1;}

}

void Coderunner::functionCall(const Linecode &code){
    Runtime_stack.push(currentLine);//push %rip into stack as return address
    string functionName=code.expression;//search for the function
    for(int i=0;i<codes.size();i++){
        if(codes[i].order==SUB&&codes[i].expression==functionName){
            currentLine=i;//set %rip to the entry of the function
            return;
        }
    }
    throw Exception("Can not find function \""+code.expression+"\" !\n");
}

void Coderunner::endFunctionCall(){
    currentLine=Runtime_stack.pop();//set %rip to the return address
}

void Coderunner::skipFunctionBody(){
    while(codes[currentLine].order!=ENDSUB) currentLine++;
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
                endProg();
                break;
            case IF:
                conditionCtrl(code);
                break;
            case CALL:
                functionCall(code);
                break;
            case ENDSUB:
                endFunctionCall();
                break;
            case SUB:
                skipFunctionBody();
                break;
            default:
                throw Exception("Undefined order.\n");
        }
    }
    catch(Exception ex){
        string str="\"\t"+code.code+"\t\"\n";
        console->write(str);
        console->write(ex.what());
        throw -1;
    }
}

RunResult Coderunner::runCodes() {
    while(currentLine>=0 &&currentLine<codes.size()) {
        try{
            runCode(codes[currentLine]);
            currentLine+=1;
            // pause the execution of program and wait for user input
            if(runnermode==INPUTVALUE) return Pause4UsrInput;
        }catch(int x){
            console->write("Execution terminated with exit code -1.\n");
            return Terminate4Error;
        }
    }
    return SeqFinished;
}
/*
 * assisting function,please just ignore it.
 */
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
        console->write(ex.what());
    }
    codes.clear();
}

void Coderunner::loadfromFile(const string &filename) {
    ifstream file;
    string lineofcode;
    file.open(filename);
    if(!file) {//if file with the name of "filename" not found
        console->write("Can not load from file '" + filename +"' !\n");
        return ;
    }
    codes.clear();//clear the buffer for the new codes
    while(!file.eof()) {
        getline(file, lineofcode, '\n');
        codes.push_back(Linecode(lineofcode));
    }
    file.close();
    console->write("Successfully load codes from file '"+filename+"'.\n");
}

void Coderunner::writeToFile(const string &name) {
        ofstream file;
        file.open(name);
        if(!file) {//actually this will hardly happen.
            throw Exception("Can not open file '"+name+"' !\n");
        }
        if(!codes.empty()) {
            for(int i = 0; i < codes.size(); i++) {
                file << codes[i].code << endl;
            }
            console->write( "Write to file \"" + name + "\" successfully.\n");
        }
        file.close();
}

void Coderunner::printExpression(const Linecode &code) {
    Calcresult calresult = evaluate(code.expression, vars);
    if(calresult.flag)
        console->write(QString::number(calresult.result).toStdString());
    else {throw Exception("Expression error.");}
}

void Coderunner::judgeCondition(const string &expression,Vector<NamedVar> &vars,bool &result) {
    Calcresult calresult=evaluate(expression,vars);
    if(calresult.flag) result=bool(calresult.result);
    else {result=false;throw Exception("Expression error in the if-clause.\n");}
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
         * if the condition is true,THEN-clause will be executed
         */
        if(ctrlcondition) {Linecode exec(string(expression, hi));runCode(exec);}
}

void Coderunner::listCodes() noexcept {
    if(codes.empty()) {
        console->write("No line of code stored.\n");
        return ;
    }
    for(int i = 0; i < codes.size(); i++)
        console->write(codes[i].code);
}

void Coderunner::endProg() noexcept{
    vars.clear();//clear the memory but don't clear the buffer of codes
    console->write("Execution terminated with exit code 0.\n");
    currentLine=-2;

}

