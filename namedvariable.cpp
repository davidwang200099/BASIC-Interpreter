#include "namedvariable.h"

int search(const string &name,NamedVar *v,int size){
    for(int i=0;i<size;i++) if(name==v[i].name) return i;
    return -1;
}

int add(NamedVar a,NamedVar b){return a.value+b.value;}
int sub(NamedVar a,NamedVar b){return a.value-b.value;}
int mult(NamedVar a,NamedVar b){return a.value*b.value;}
int div(NamedVar a,NamedVar b){return a.value/b.value;}

