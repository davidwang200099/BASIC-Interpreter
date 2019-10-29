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


#ifndef DEFAULT_EXP_LENGTH
#define DEFAULT_EXP_LENGTH 100;
#endif

typedef enum {REM,LET,PRINT,INPUT,GOTO,END} Order;

int readNumber(char *s){
    int result=0;
    Rank i=0;
    while(!isdigit(s[i])) i++;
    while(isdigit(s[i])){
        result*=10;
        result+=(s[i])-'0';
        i++;
    }
    return result;
}


#include <string>
using namespace std;

class Linecode {
    public:
        Rank lineno;
        string code;
        Order order;
        string expression;

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
        void printExpression(Vector<NamedVar> &v);
        void printSomething(Vector<NamedVar> &v);
        void jump();
};


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
    char *expression=new char[100]{0};

    strcpy(expression,code.data()+ptr);

    //cout<<lineno<<" "<<order<<" "<<expression<<endl;

    this->expression=expression;

    //execOrder()
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

    int newvalue=readNumber(const_cast<char *>(expression.data()+i));

    v.push_back(NamedVar(newvarname,newvalue));

    //for(int i=0;i<v.size();i++) cout<<v[i].name<<" "<<v[i].value<<endl;
}

void Linecode::printVar(Vector<NamedVar> &v) {
    /*string name(expression);
    for(int i=0;i<v.size();i++)
        if(v[i].name==name) {cout<<v[i].value<<endl;return;}
    cout<<"'"<<name<<"'"<<" was not declared.\n";*/
    //cout<<expression<<endl;
    Calcresult calresult=evaluate(expression,v);
    if(calresult.flag) cout<<calresult.result<<endl;
    else cout << "Expression error!\n";
}

#endif //BASIC_INTERPRETER_LINECODE_H
