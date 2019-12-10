#include "evaluate.h"
#include "evaluate_rule.h"
int readNumber(const string &s,Rank start,Rank *p){
    int result=0;
    Rank i=start;
    while(i<s.size()&&isdigit(s[i])){
        result*=10;
        result+=(s[i])-'0';
        i++;
    }
    if(p) *p=i;
    return result;
}

bool isOperator(const char s){
    return s=='+'||
           s=='-'||
           s=='*'||
           s=='/'||
           s=='('||
           s==')'||
           s=='='||
           s=='>'||
           s=='<';
}

bool isSingleMinus(const string &s,Rank i){
    if(s[i]!='-') return false;
    if(i==0) return true;
    while(i-1>=0&&isspace(s[i-1])) i--;
    return s[i-1]=='(';
}

bool isSinglePlus(const string &s,Rank i){
    if(s[i]!='+') return false;
    if(i==0) return true;
    while(i-1>=0&&isspace(s[i-1])) i--;
    return s[i-1]=='(';
}

bool isLGVNS(const char s){return isupper(s)||islower(s)||s=='_';}//is legal Varname Start

bool isLGVN(const char s){return isupper(s)||islower(s)||isdigit(s)||s=='_';}//is legal Varname

bool isLGEXP(const char s){return isupper(s)||islower(s)||isdigit(s)||isOperator(s)||isspace(s);}
//is legal expression

Rank readNumber(const string &s,Rank start,Stack<int> &opnd){
    int result=0;
    Rank i=start;
    while(i<s.size()&&isdigit(s[i])){
        result*=10;
        result+=(s[i])-'0';
        i++;
    }
    opnd.push(result);
    return i;
}


Rank readVar(const string &s,Rank start,Stack<int> &opnd,Vector<NamedVar> &v){
    string varname;
    Rank i=start;
    if(i<s.size()&&isLGVNS(s[i])) varname+=s[i++];
    while(i<s.size()&&isLGVN(s[i]))
        varname+=s[i++];
    NamedVar var(varname);
    int rank=v.search(var);
    if(rank==-1) {
        return -1;
    }
    opnd.push(v[rank].value);
    return i;
}
/*
 *From Rank start,read a varname from s,and store next start in Rank *p
 */
string readVar(const string &s,Rank start,Rank *p){
    string varname;
    Rank i=start;
    while(i<s.size()&&isspace(s[i])) i++;
    if(isLGVNS(s[i])) varname+=s[i++];
    while(i<s.size()&&isLGVN(s[i]))
        varname+=s[i++];
    if(p) *p=i;
    return varname;
}
//It can cope with expressions filled with Space.
Calcresult evaluate(const string &s,Vector<NamedVar> &v) {
    Stack<int> opnd;//stack of operated numbers and operator
    Stack<char> optr;
    optr.push('\0');
    Rank i = 0;
    //cout<<"expression:"<<s<<endl;
    while(!optr.empty()) {//process every char before optr turns empty
        while(i<s.size()&&isspace(s[i])) i++;//skip space 
        if(i<s.size()&&isdigit(s[i])) i = readNumber(s,i, opnd);//if current char is a number,then read it
        else {
            if(i<s.size()&&isLGVNS(s[i])) i = readVar(s, i,opnd ,v);//if current char is a named variable
            else {
                auto curr=(i<s.size())?s[i]:'\0';//if current char is an operator
                if(i<s.size()){
                    if(isSingleMinus(s,i)||isSinglePlus(s,i)) //if operator is single +/-
                        opnd.push(0);//-2==0-2
                }
                switch(orderbetween(optr.top(), curr)) {
                    case '<'://current operator is prior to stack-top operator
                        optr.push(curr);//evaluation delayed.Current operator pushed into the stack
                        ++i;
                        break;
                    case '='://the same priority :curr is ')' or '\0'
                        optr.pop();//omit bracket and accept next char
                        ++i;
                        break;
                    case '>': {//stack-top operator is prior to current operator
                        char op = optr.pop();
                        if(opnd.empty()) return EVALUATE_ERROR;
                        int popnd2 = opnd.pop();
                        if(opnd.empty()) return EVALUATE_ERROR;
                        int popnd1 = opnd.pop();//evaluation can be done
                        Calcresult rslt=calculate(popnd1, popnd2, op);
                        if(rslt.flag) opnd.push(rslt.result);//if evaluation is valid,push it into stack
                        else return EVALUATE_ERROR;//else grammatical error.
                        break;
                    }
                    case '?'://grammatical error.
                        return EVALUATE_ERROR;
                }
            }
        }
        if(i==-1) return EVALUATE_ERROR;
    }
    if(!opnd.empty()) return opnd.pop();
    else return EVALUATE_ERROR;//return the result
}

