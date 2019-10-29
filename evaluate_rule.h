//
// Created by pc on 2019/10/23.
//

#ifndef BASIC_EVALUATE_BASIC_CALC_RULE_H
#define BASIC_EVALUATE_BASIC_CALC_RULE_H

#define N_OPTR 8

#ifndef OPND
#define OPND int
#endif

#define ERROR Calcresult(INT32_MIN,false)

const char pri[N_OPTR][N_OPTR] = {
	/*                   |     current   operator     |  */
	/*                   +   -   *   /   (   )  \0   =  */
	/* stack   +     */ '>','>','<','<','<','>','>','?',
	/* top     -     */ '>','>','<','<','<','>','>','?',
	/*operator *     */ '>','>','>','>','<','>','>','?',
	/*         /     */ '>','>','>','>','<','>','>','?',
	/*         (     */ '<','<','<','<','<','=','?','?',
	/*         )     */ '?','?','?','?','?','?','?','?',
	/*         \0    */ '<','<','<','<','<','?','=','<',
	/*         =     */ '<','<','<','<','<','>','>','>'
};

typedef enum { ADD, SUB, MUL, DIV, LP, RP, EOE, EQ} Operator;

char orderbetween(char a, char b) {
	int a1, b1;
	switch (a) {
	case '+':a1 = ADD; break;
	case '-':a1 = SUB; break;
	case '*':a1 = MUL; break;
	case '/':a1 = DIV; break;
	case '(':a1 = LP; break;
	case ')':a1 = RP; break;
	case '\0':a1 = EOE; break;
	case '=':a1 = EQ;
	}
	switch (b) {
	case '+':b1 = ADD; break;
	case '-':b1 = SUB; break;
	case '*':b1 = MUL; break;
	case '/':b1 = DIV; break;
	case '(':b1 = LP; break;
	case ')':b1 = RP; break;
	case '\0':b1 = EOE; break;
	case '=':b1=EQ;
	}
	return pri[a1][b1];
}

#ifndef CALCRESULT
#define CALCRESULT
struct Calcresult{
	bool flag;
	int result;
	Calcresult(int result,bool flag=true):result(result),flag(flag){}
};

#endif
Calcresult calculate(int a, int b, char optr) {
	switch (optr) {
	case '+':return a + b;
	case '-':return a - b;
	case '*':return a * b;
	case '/':return b==0?ERROR:a/b;
	case '=':return a == b;
	}
}

#endif //BASIC_EVALUATE_BASIC_CALC_RULE_H
