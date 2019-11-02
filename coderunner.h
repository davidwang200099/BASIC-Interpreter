#ifndef BASIC_INTERPRETER_CODERUNNER_H
#define BASIC_INTERPRETER_CODERUNNER_H

#include "linecode.h"
#include "Vector.h"
#include <fstream>

//typedef enum{INTERACTION,STORATION,EXECUTION} Mode;

typedef enum{QUIT_ORDER,NORMAL_ORDER,BASIC_CODE} InputType;

#ifndef Execflag
#define Execflag bool
#endif

class Coderunner{
    private:
        Vector<Linecode> codes;
        Vector<NamedVar> vars;
        Rank currentLine;
    protected:
        Execflag writeToFile(const string &name);
        Execflag runCode(const Linecode &code);
        Execflag assignVar(const Linecode &code);
        Execflag printExpression(const Linecode &code);
        Execflag inputValue(const Linecode &code);
        Execflag jumpto(const Linecode &code);
        Execflag conditionCtrl(const Linecode &code);
    public:
        Coderunner();
        ~Coderunner(){}
        void run();
        void runCodes();
        void listCodes();
        void expressWelcome(){cout<<"Welcome to BASIC!\n";}
        InputType processOrder(const string &lineofcode);
};


#endif //BASIC_INTERPRETER_CODERUNNER_H
