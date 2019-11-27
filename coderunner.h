#ifndef BASIC_INTERPRETER_CODERUNNER_H
#define BASIC_INTERPRETER_CODERUNNER_H

#include "linecode.h"
#include "Vector.h"
#include <fstream>
#include <QObject>
class Console;

typedef enum{QUIT_ORDER,NORMAL_ORDER,BASIC_CODE} InputType;
typedef enum{SeqFinished,Pause4UsrInput,Terminate4Error} RunResult;




#ifndef Execflag
#define Execflag bool
#endif

#ifndef SUCCEED
#define SUCCEED true
#endif

#ifndef FAIL
#define FAIL false
#endif


typedef enum{INPUTCODE,INPUTVALUE,SUCCESSIVERUN} Runnermode;

class Coderunner
{
        friend class Console;
    private:
        Vector<Linecode> codes;
        Vector<NamedVar> vars;
        Vector<int> runtimeVars;
        Rank currentLine;
        Console *console;
        Runnermode runnermode;
        Stack<int> Runtime_stack;
        //Registers regs;
    protected:
        void writeToFile(const string &name);
        void runCode(const Linecode &code);
        void assignVar(const Linecode &code);
        void printExpression(const Linecode &code);
        void inputValue(const Linecode &code) noexcept;
        void jumpto(const Linecode &code);
        void conditionCtrl(const Linecode &code);
        void judgeCondition(const string &expression,Vector<NamedVar> &vars,bool &result);
        void endProg() noexcept;
        void functionCall(const Linecode &code);
        void endFunctionCall();
        void skipFunctionBody();
        //void readassignValue();
    public:
        Coderunner(Console *console);
        ~Coderunner(){}
        //void run();
        RunResult runCodes();
        void listCodes() noexcept ;
        void runFile(const string &filename);
        //void expressWelcome()noexcept ;
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
