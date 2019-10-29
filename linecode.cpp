#include "linecode.h"

Order toOrder(string &str) {
    if(str=="REM") return REM;
    if(str=="LET") return LET;
    if(str=="PRINT") return PRINT;
    if(str=="INPUT") return INPUT;
    if(str=="GOTO") return GOTO;
    if(str=="END") return END;
}

Rank Linecode::readLineno(Rank start){
    int no=0;
    while(isdigit(code[start])){
        no*=10;
        no+=(code[start])-'0';
        start++;
    }
    lineno=no;
    return start;
}

Rank Linecode::readOrder(Rank start,Order &order) {
    string str;
    while(isupper(code[start])) {
        str += code[start];
        start++;
    }
    order=toOrder(str);
    return start;
}

void Linecode::analyseCode()  {
    Rank ptr=0;

    while(code[ptr]==' '&&ptr<code.size()) ptr++;
    if(isdigit(code[ptr])) ptr=readLineno(ptr);

    while(code[ptr]==' '&&ptr<code.size()) ptr++;
    if(isupper(code[ptr])) ptr=readOrder(ptr,order);

    while(code[ptr]==' '&&ptr<code.size()) ptr++;

    this->expression=string(code,ptr);

}

void Linecode::execOrder(Vector<NamedVar> &v) {
    switch(order){
        case LET:initVar(v);break;
        case PRINT:printVar(v);break;
    }

}

void Linecode::initVar(Vector<NamedVar> &v) {
    Rank i=0;
    //cout<<ptr<<endl;
    string newvarname;
    while((isupper(expression[i])||islower(expression[i])))
    {newvarname+=expression[i];i++;}

    if(expression[i]=='=') i++;

    Calcresult result=evaluate(string(expression,i),v);
    Rank r=v.search(NamedVar(newvarname));
    if(r<v.size()) {
        if(result.flag) v[r].value=result.result;
        else cout<<"Expression error!\n";
    }
    else {
        if(result.flag) v.push_back(NamedVar(newvarname, result.result));
        else cout<<"Expression error!\n";
    }

}

void Linecode::printVar(Vector<NamedVar> &v) {
    Calcresult calresult=evaluate(expression,v);
    if(calresult.flag) cout<<calresult.result<<endl;
    else cout << "Expression error!\n";
}

