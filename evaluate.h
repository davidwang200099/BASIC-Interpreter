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

int readNumber(const string &s);

Rank readNumber(const string &S,Stack<int> &opnd);

Rank readVar(const string &s,Rank start,Stack<int> &opnd,Vector<NamedVar> &v);

Calcresult evaluate(const string &s,Vector<NamedVar> &v);


#endif //BASIC_EVALUATE_EVALUATE_H
