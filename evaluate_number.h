//
// Created by pc on 2019/10/23.
//

#ifndef BASIC_EVALUATE_RPN_H
#define BASIC_EVALUATE_RPN_H

#include "basic_calc_rule.h"
#include <string>
#include "Stack.h"
#include <cctype>
#include <limits.h>

using namespace std;




int Evaluate_num(const string &s){
    Stack<int> opnd;
    Stack<char> optr;
    optr.push('\0');
    Rank i=0;
    while(!optr.empty()){
        if(isdigit(s[i])){
            i=readNumber(s,opnd);
        }else{
            switch (orderbetween(optr.top(),s[i])){
                case '<':
                    optr.push(s[i]);
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
                default:
                    cout<<"Expression error!\n";
                    return INT_MIN;
                    //exit(-1);
            }
        }
    }
    return opnd.pop();
}



#endif //BASIC_EVALUATE_RPN_H
