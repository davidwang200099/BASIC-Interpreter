#include "evaluate.h"
#include "evaluate_rule.h"
int readNumber(const string &s){
    int result=0;
    Rank i=0;
    while(i<s.size()&&isdigit(s[i])){
        result*=10;
        result+=(s[i])-'0';
        i++;
    }
    return result;
}

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
    while(i<s.size()&&(islower(s[i])||isupper(s[i]))){
        varname+=s[i];
        i++;
    }
    int rank=v.search(NamedVar(varname));
    if(rank==v.size()) {
        cout<<"'"<<varname<<"' "<<"was not declared in this scope!\n";
        return -1;
    }
    opnd.push(v[rank].value);
    return i;
}


Calcresult evaluate(const string &s,Vector<NamedVar> &v) {
    Stack<int> opnd;
    Stack<char> optr;
    optr.push('\0');
    Rank i = 0;
    while(!optr.empty()) {
        while(s[i]==' ') i++;
        //cout<<i<<" "<<int(s[i])<<endl;
        if(i<s.size()&&isdigit(s[i])) i = readNumber(s,i, opnd);
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
                        Calcresult rslt=calculate(popnd1, popnd2, op);
                        if(rslt.flag) opnd.push(rslt.result);
                        else return ERROR;
                        break;
                    }
                    case '?':
                        //cout << "Expression error!\n";
                        return ERROR;
                        //return INT32_MIN;
                        //exit(-1);
                }
            }
        }
        if(i==-1) return ERROR;
    }
    return opnd.pop();
}

