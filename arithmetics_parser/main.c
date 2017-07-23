#include <stdio.h>
#include "tokens.h"
int expr();
int yylex();
extern int token;

int main() {
	token = yylex();
  while(token != TK_EOF){
  	int r = expr();
    printf("Last token: %d\n", token);
  	printf("Result =  %d\n",r);
    token = yylex();
  }
}
