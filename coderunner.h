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

#ifndef SUCCEED
#define SUCCEED true
#endif

#ifndef FAIL
#define FAIL false
#endif

class Coderunner{
    private:
        Vector<Linecode> codes;
        Vector<NamedVar> vars;
        Rank currentLine;
    protected:
        void writeToFile(const string &name);
        void  runCode(const Linecode &code);
        void assignVar(const Linecode &code);
        void printExpression(const Linecode &code);
        void inputValue(const Linecode &code) noexcept;
        void jumpto(const Linecode &code);
        void conditionCtrl(const Linecode &code);
        void judgeCondition(const string &expression,Vector<NamedVar> &vars,bool &result);
    public:
        Coderunner();
        ~Coderunner(){}
        void run();
        void runCodes();
        void listCodes() noexcept ;
        void runFile(const string &filename);
        void expressWelcome()noexcept{cout<<"Welcome to BASIC!\n";}
        void loadfromFile(const string &filename);
        InputType processOrder(const string &lineofcode);
};

//typedef enum {} ExceptType;

struct Exception{
    //ExceptType type;
    string message;
    Exception(const string &s):message(s){}
    string what() const {return message;}
};


#endif //BASIC_INTERPRETER_CODERUNNER_H
