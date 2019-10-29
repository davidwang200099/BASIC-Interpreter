#include "coderunner.h"

Coderunner::Coderunner() {
    cout<<"Welcome to BASIC!\n";
}

void Coderunner::run() {
    string lineofcode;
    while(true){
        getline(cin,lineofcode,'\n');
        if(lineofcode=="QUIT") break;
        if(lineofcode=="WRITE") {writeToFile("out.vb");continue;}
        codes.push_back(Linecode(lineofcode));
        execOrder();
    }
}

void Coderunner::execOrder() {
    codes[codes.size()-1].execOrder(vars);
}

void Coderunner::writeToFile(const string &name) {
    ofstream file;
    file.open(name);
    if(!file) {cout<<"Wrong\n";return ;}
    for(int i=0;i<codes.size();i++){
        file<<codes[i].code<<endl;
    }
    file<<"END";
    file.close();
}