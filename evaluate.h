//
// Created by pc on 2019/10/24.
//

#ifndef BASIC_EVALUATE_EVALUATE_H
#define BASIC_EVALUATE_EVALUATE_H

//why move "basic_calc_rule.h"?
#include "namedvariable.h"
#include "Stack.h"
#include "Vector.h"

#ifndef CALCRESULT
#define CALCRESULT
struct Calcresult{
    bool flag;
    int result;
    Calcresult(int result,bool flag=true):result(result),flag(flag){}
};
#endif

bool isOperator(const char s);

bool isLGVNS(const char s);//is legal Varname Start

bool isLGVN(const char s);//is legal Varname

bool isLGEXP(const char s);//is legal expression

int readNumber(const string &s,Rank start=0,Rank *p=NULL);

Rank readNumber(const string &s,Stack<int> &opnd);

Rank readVar(const string &s,Rank start,Stack<int> &opnd,Vector<NamedVar> &v);

string readVar(const string &s,Rank start=0,Rank *p=NULL);

Calcresult evaluate(const string &s,Vector<NamedVar> &v);


#endif //BASIC_EVALUATE_EVALUATE_H
