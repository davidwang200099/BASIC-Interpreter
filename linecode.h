//
// Created by pc on 2019/10/24.
//

#ifndef BASIC_INTERPRETER_LINECODE_H
#define BASIC_INTERPRETER_LINECODE_H

#include "Vector.h"
#include "evaluate.h"
#ifndef Rank
#define Rank int
#endif

typedef enum {REM,LET,PRINT,INPUT,GOTO,END} Order;

#include <string>
using namespace std;


class Linecode {
    public:
        string code;//110 LET A=10
        Rank lineno;//110
        Order order;//LET
        string expression;//A=10

        Linecode(const string &code) : code(code) { analyseCode(); }

        Linecode(){}

        Linecode(const Linecode &l) : code(l.code), order(l.order), lineno(l.lineno)  {}

        ~Linecode(){}

        Rank readLineno(Rank start);

        Rank readOrder(Rank start, Order &order);

        void analyseCode();

        void execOrder(Vector<NamedVar> &v);

    protected:
        void initVar(Vector<NamedVar> &v);
        void printVar(Vector<NamedVar> &v);
        //void writeToFile(const string &name);
        void jump();
};




#endif //BASIC_INTERPRETER_LINECODE_H
