#include "coderunner.h"

typedef union{
    int d;
    char c;
} data_T;

int main(){
    Coderunner runner;
    runner.run();
    /*auto *p=new data_T[5];
    for(int i=0;i<5;i++) p[i].d=i;
    for(int i=0)*/
    return 0;
}