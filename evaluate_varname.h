//
// Created by pc on 2019/10/23.
//

#ifndef BASIC_EVALUATE_EVALUATE_VARNAME_H
#define BASIC_EVALUATE_EVALUATE_VARNAME_H

#include "basic_calc_rule.h"
#include <string>
#include <cctype>
#include "Stack.h"
#include <stdexcept>
#include <cstdio>
using namespace std;
struct NamedVar{
    std::string name;
    int value;
    NamedVar(){}
    NamedVar(const string &name,int value=0):name(name),value(value){}
    NamedVar(const NamedVar &var):name(var.name),value(var.value){}
    bool operator==(const NamedVar &v){return name==v.name;}
};



//NamedVar varlist[3]={NamedVar("apple",1),NamedVar("banana",2),NamedVar("cat",3)};

int search(const string &name,NamedVar *v,int size){
    for(int i=0;i<size;i++) if(name==v[i].name) return i;
    return -1;
}

int add(NamedVar a,NamedVar b){return a.value+b.value;}
int sub(NamedVar a,NamedVar b){return a.value-b.value;}
int mult(NamedVar a,NamedVar b){return a.value*b.value;}
int div(NamedVar a,NamedVar b){return a.value/b.value;}

int calcu(NamedVar a,NamedVar b,char optr){
    switch(optr){
        case '+':return add(a,b);
        case '-':return sub(a,b);
        case '*':return mult(a,b);
        case '/':return div(a,b);
    }
}

Rank readVar(const string &s,Rank start,Stack<int> &opnd,Vector<NamedVar> &v){
    string varname;
    Rank i=start;
    while(i<s.size()&&(islower(s[i])||isupper(s[i]))){
        varname+=s[i];
        i++;
    }
    int rank=v.search(NamedVar(varname));
    if(rank==-1) {
        cout<<"'"<<varname<<"' "<<"was not declared in this scope!\n";
        return -1;
    }
    opnd.push(v[rank].value);
    return i;
}



#endif //BASIC_EVALUATE_EVALUATE_VARNAME_H
