//
// Created by pc on 2019/10/25.
//

#ifndef BASIC_INTERPRETER_CODERUNNER_H
#define BASIC_INTERPRETER_CODERUNNER_H

#include "linecode.h"
#include "Vector.h"
#include "evaluate.h"

class Coderunner{
    private:
        Vector<Linecode> codes;
        Vector<NamedVar> vars;
    public:
        Coderunner();
        void run();
        void execOrder();
};

Coderunner::Coderunner() {
    cout<<"Welcome to BASIC!\n";
}

void Coderunner::run() {
    string lineofcode;
    while(true){
        getline(cin,lineofcode,'\n');
        if(lineofcode=="QUIT") break;
        codes.push_back(Linecode(lineofcode));
        execOrder();
    }
}

void Coderunner::execOrder() {
    codes[codes.size()-1].execOrder(vars);
}

#endif //BASIC_INTERPRETER_CODERUNNER_H
