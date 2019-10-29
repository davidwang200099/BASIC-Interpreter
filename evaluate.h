//
// Created by pc on 2019/10/24.
//

#ifndef BASIC_EVALUATE_EVALUATE_H
#define BASIC_EVALUATE_EVALUATE_H


#include "evaluate_number.h"
#include "evaluate_varname.h"

struct Calcresult{
    bool flag;
    int result;
    Calcresult(bool flag,int result):result(result),flag(flag){}
};

Calcresult evaluate(string &s,Vector<NamedVar> &v) {
    Stack<int> opnd;
    Stack<char> optr;
    optr.push('\0');
    Rank i = 0;
    while(!optr.empty()) {
        //cout<<i<<" "<<int(s[i])<<endl;
        if(i<s.size()&&isdigit(s[i])) i = readNumber(s, opnd);
        else {
            if(i<s.size()&&(islower(s[i]) || isupper(s[i]))) i = readVar(s, i,opnd ,v);
            else {
                auto curr=(i<s.size())?s[i]:'\0';
                switch(orderbetween(optr.top(), curr)) {
                    case '<':
                        optr.push(curr);
                        ++i;
                        break;
                    case '=':
                        optr.pop();
                        ++i;
                        break;
                    case '>': {
                        char op = optr.pop();
                        int popnd2 = opnd.pop();
                        int popnd1 = opnd.pop();
                        opnd.push(calculate(popnd1, popnd2, op));
                        break;
                    }
                    case '?':
                        //cout << "Expression error!\n";
                        return Calcresult(false,INT32_MIN);
                        //return INT32_MIN;
                        //exit(-1);
                }
            }
        }
        if(i==-1) return Calcresult(false,INT32_MIN);
    }
    return Calcresult(true,opnd.pop());
}

#endif //BASIC_EVALUATE_EVALUATE_H
