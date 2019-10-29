
#ifndef BASIC_INTERPRETER_NAMEDVARIABLE_H
#define BASIC_INTERPRETER_NAMEDVARIABLE_H

#include <string>
using namespace std;

#ifndef Rank
#define Rank int
#endif

#include <iostream>
#include "Vector.h"
#include "Stack.h"

struct NamedVar{
    std::string name;
    int value;
    NamedVar(){}
    NamedVar(const string &name,int value=0):name(name),value(value){}
    NamedVar(const NamedVar &var):name(var.name),value(var.value){}
    ~NamedVar(){}
    bool operator==(const NamedVar &v){return name==v.name;}
};



#endif //BASIC_INTERPRETER_NAMEDVARIABLE_H
