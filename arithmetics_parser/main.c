#include <stdio.h>

int expr();
int yylex();
extern int token;

int main() {
	token = yylex();
	int r = expr();
	printf("Result =  %d\n",r);
}
