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
        cout<<"'"<<varname<<"' "<<"was not declared in this scope!\n";
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
    Stack<int> opnd;
    Stack<char> optr;
    optr.push('\0');
    Rank i = 0;
    //cout<<"expression:"<<s<<endl;
    while(!optr.empty()) {
        while(i<s.size()&&isspace(s[i])) i++;
        if(i<s.size()&&isdigit(s[i])) i = readNumber(s,i, opnd);
        else {
            if(i<s.size()&&isLGVNS(s[i])) i = readVar(s, i,opnd ,v);
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
                        Calcresult rslt=calculate(popnd1, popnd2, op);
                        if(rslt.flag) opnd.push(rslt.result);
                        else return EVALUATE_ERROR;
                        break;
                    }
                    case '?':
                        return EVALUATE_ERROR;
                }
            }
        }
        if(i==-1) return EVALUATE_ERROR;
    }
    if(!opnd.empty()) {/*cout<<"result:"<<opnd.top()<<endl;*/return opnd.pop();}
    else return EVALUATE_ERROR;
}

