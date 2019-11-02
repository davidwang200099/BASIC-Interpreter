#include "coderunner.h"


Coderunner::Coderunner() :currentLine(0) {
    expressWelcome();
}

InputType Coderunner::processOrder(const string &lineofcode) {
    Rank start=0;
    string ordertoRunner=readVar(lineofcode,start,&start);
    if(ordertoRunner=="QUIT") return QUIT_ORDER;
    if(ordertoRunner=="LIST") {listCodes();return NORMAL_ORDER;}
    if(ordertoRunner=="WRITE") {
        while(lineofcode[start]==' ')  start++;
        string filename=string(lineofcode,start);
        writeToFile(filename);
        return NORMAL_ORDER;
    }
    if(ordertoRunner=="CLEARBUFFER") {codes.clear();vars.clear();return NORMAL_ORDER;}
    if(ordertoRunner=="CLEAR") {system("clear");expressWelcome();return NORMAL_ORDER;}
    if(ordertoRunner=="RUN") {runCodes();return NORMAL_ORDER;}
    return BASIC_CODE;
}

void Coderunner::run() {
    string lineofcode;
    while(true){
        getline(cin,lineofcode,'\n');
        //analyse what kind of input it is.Order to the coderunner/BASIC code.
        InputType inputtype=processOrder(lineofcode);
        if(inputtype==QUIT_ORDER) break;
        else {
            if(inputtype == BASIC_CODE) {
                Linecode newcode= Linecode(lineofcode);
                if(newcode.lineno==INT32_MIN){
                    if(!runCode(newcode)) cout<<"Execution error!\n";
                }else{
                    Rank r=codes.search(newcode);
                    if(r==-1) codes.push_back(newcode);
                    else codes[r]=newcode;
                }
            }
        }
    }
}


Execflag Coderunner::assignVar(const Linecode &code) {
    //if(mode==STORATION) return true;
    Rank i=0;
    const string &expression=code.expression;
    string newvarname;

    if(expression.empty()) return false;//avoid "LET"

    while(i<expression.size()&&expression[i]==' ') i++;//skip space

    if(isLGVNS(expression[i]))
        newvarname=readVar(expression,i,&i);
    else return false;//avoid "LET    "

    while(i<expression.size()&&expression[i]==' ') i++;
    if(i==expression.size()) return false;//avoid "LET   A    "

    if(expression[i]=='=') i++;
    else return false;//avoid strange expressions such as "LET A  !"

    Calcresult result=evaluate(string(expression,i),vars);//avoid illegal expression
    Rank r=vars.search(NamedVar(newvarname));
    if(r!=-1) {
        if(result.flag) {vars[r].value=result.result;return true;}
        else {cout<<"Expression error!\n";return false;}
    }
    else {
        if(result.flag) {vars.push_back(NamedVar(newvarname, result.result));return true;}
        else {cout<<"Expression error!\n";return false;}
    }
}

Execflag Coderunner::printExpression(const Linecode &code) {

    Calcresult calresult = evaluate(code.expression, vars);
    if(calresult.flag) {
        cout << calresult.result << endl;
        return true;
    }
    else {
        cout << "Expression error!\n";
        return false;
    }
}

Execflag Coderunner::jumpto(const Linecode &code) {
    int nextLineNumber = readNumber(code.expression, 0);
    currentLine = codes.search(Linecode(nextLineNumber)) - 1;
    if(currentLine < -1) {
        cout << "Line NO" << nextLineNumber << " not found!\n";
        cout << "Execution terminated.\n";
        return false;
    }
    return true;

}

Execflag Coderunner::inputValue(const Linecode &code) {
    int input;
    cin>>input;
    cin.get();
    string varname=readVar(code.expression);
    Rank r=vars.search(NamedVar(varname));
    if(r>=0) vars[r].value=input;
    else vars.push_back(NamedVar(varname,input));
    return true;
}

Execflag Coderunner::runCode(const Linecode &code) {
    Execflag flag;
    switch(code.order){
        case REM:flag=true;break;
        case LET:flag=assignVar(code);break;
        case PRINT:flag=printExpression(code);break;
        case GOTO:flag=jumpto(code);break;
        case INPUT:flag=inputValue(code);break;
        case END:flag=true;break;
        default: cout<<"Undefined order!\n";flag=false;
    }
    return flag;
}

void Coderunner::runCodes() {
    vars.clear();
    currentLine=0;
    while(currentLine<codes.size()){
        if(!runCode(codes[currentLine])) break;
        currentLine++;
    }
}

Execflag Coderunner::writeToFile(const string &name) {
    ofstream file;
    file.open(name);
    if(!file) {cout<<"Wrong\n";return false;}
    if(!codes.empty()) {
        for(int i = 0; i < codes.size(); i++) {
            file << codes[i].code << endl;
        }
        file << "END";
        cout<<"Write to file \""<<name<<"\" successfully.\n";
    }
    file.close();
    return true;
}

/*Execflag Coderunner::conditionCtrl(const Linecode &code){
    Rank lo=0,hi=0;
    string condition=
    bool controllingCodition=
}*/

void Coderunner::listCodes() {
    if(codes.empty()) {cout<<"No line of code stored.\n";return;}
    for(int i=0;i<codes.size();i++)
        cout<<codes[i].code<<endl;
}