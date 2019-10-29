//
// Created by pc on 2019/10/25.
//

#ifndef BASIC_INTERPRETER_CODERUNNER_H
#define BASIC_INTERPRETER_CODERUNNER_H

#include "linecode.h"
#include "Vector.h"
#include <fstream>
class Coderunner{
    private:
        Vector<Linecode> codes;
        Vector<NamedVar> vars;
    protected:
        void writeToFile(const string &name);
    public:
        Coderunner();
        void run();
        void execOrder();
};


#endif //BASIC_INTERPRETER_CODERUNNER_H
