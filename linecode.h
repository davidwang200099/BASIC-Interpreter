#ifndef BASIC_INTERPRETER_LINECODE_H
#define BASIC_INTERPRETER_LINECODE_H

#include "Vector.h"
#include "evaluate.h"
#ifndef Rank
#define Rank int
#endif

typedef enum {REM,LET,PRINT,INPUT,IF,GOTO,END,UNDEFINED} Order;

#include <string>
using namespace std;


struct Linecode {

        string code;//110 LET A=10
        Rank lineno;//110
        Order order;//LET
        string expression;//A=10

        Linecode(const string &code) : code(code) { analyseCode(); }

        Linecode(){}

        Linecode(Rank lineno):lineno(lineno){}

        Linecode(const Linecode &l) : code(l.code), order(l.order), lineno(l.lineno)  {}

        ~Linecode(){}

        Rank readLineno(Rank start);

        Rank readOrder(Rank start, Order &order);

        void analyseCode();

        bool operator==(const Linecode &e) const {return lineno==e.lineno;}

        bool operator<(const Linecode &e) const {return lineno<e.lineno;}

        Linecode& operator=(const Linecode &e) {
            code=e.code;
            lineno=e.lineno;
            order=e.order;
            expression=e.expression;
            return *this;
        }
};




#endif //BASIC_INTERPRETER_LINECODE_H
