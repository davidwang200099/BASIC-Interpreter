#include "linecode.h"

Order toOrder(string &str) {
    if(str=="REM") return REM;
    if(str=="LET") return LET;
    if(str=="PRINT") return PRINT;
    if(str=="INPUT") return INPUT;
    if(str=="GOTO") return GOTO;
    if(str=="END") return END;
    if(str=="IF") return IF;
    if(str=="CALL") return CALL;
    if(str=="Sub") return SUB;
    if(str=="End") return ENDSUB;
    return UNDEFINED;
}

Rank Linecode::readLineno(Rank start,Rank &lineno)  {
    lineno=readNumber(code,start,&start);
    return start;
}
/*
 * read order from code and return the next position of the end of the order
 */
Rank Linecode::readOrder(Rank start,Order &order)  {
    string str;
    while(start<code.size()&&(isupper(code[start])||islower(code[start])))
        str += code[start++];

    order=toOrder(str);
    return start;
}
/*
 * After the analysis,it can be guaranteed that:
 * the expression will start with letters.
 * After the analysis,the type of the code can be detected.
 */
void Linecode::analyseCode() {
    Rank ptr=0;

    while(ptr<code.size()&&isspace(code[ptr])) ptr++;
    if(ptr<code.size()&&isdigit(code[ptr])) ptr=readLineno(ptr,lineno);
    else lineno=INT32_MIN;

    while(ptr<code.size()&&isspace(code[ptr])) ptr++;
    if(ptr<code.size()&&(isupper(code[ptr])||islower(code[ptr]))) ptr=readOrder(ptr,order);

    while(ptr<code.size()&&isspace(code[ptr])) ptr++;

    this->expression=string(code,ptr);

}
