#include "linecode.h"

Order toOrder(string &str) {
    if(str=="REM") return REM;
    if(str=="LET") return LET;
    if(str=="PRINT") return PRINT;
    if(str=="INPUT") return INPUT;
    if(str=="GOTO") return GOTO;
    if(str=="END") return END;
    if(str=="IF") return IF;
    return UNDEFINED;
}

Rank Linecode::readLineno(Rank start)  {
    int no=0;
    //while(start<code.size()&&code[start]==' ') start++;
    while(start<code.size()&&isdigit(code[start])){
        no*=10;
        no+=(code[start++])-'0';
    }
    lineno=no;
    return start;
}

Rank Linecode::readOrder(Rank start,Order &order)  {
    string str;
    //while(start<code.size()&&code[start]==' ') start++;
    while(start<code.size()&&isupper(code[start]))
        str += code[start++];

    order=toOrder(str);
    return start;
}

void Linecode::analyseCode() {
    Rank ptr=0;

    while(ptr<code.size()&&code[ptr]==' ') ptr++;
    if(ptr<code.size()&&isdigit(code[ptr])) ptr=readLineno(ptr);
    else lineno=INT32_MIN;

    while(ptr<code.size()&&code[ptr]==' ') ptr++;
    if(ptr<code.size()&&isupper(code[ptr])) ptr=readOrder(ptr,order);

    while(ptr<code.size()&&code[ptr]==' ') ptr++;

    this->expression=string(code,ptr);

}
