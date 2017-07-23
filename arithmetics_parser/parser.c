#include <stdio.h>
//#include "tokens.h" //comment out once makefile is fixed
int token;

int expr();
int term();
int factor();
int yylex();
extern char *yytext;

int expr() {
	int result = term();
	switch(token){
		case OP_SUB:
		case OP_ADD: {
			while ((token == OP_ADD) || (token == OP_SUB)) {
				int op = token;
				token = yylex();
				if(op == OP_ADD){
					result += term();
				} else {
					result -= term();
				}
			}
			break;
		}
		default:
			break;
	}
	return result;
}

int term() {
	int result = factor();
	switch(token){
		case OP_DIV:
		case OP_MUL: {
			while ((token == OP_MUL) || (token == OP_DIV)) {
				int op = token;
				token = yylex();
				if(op == OP_MUL) {
					result *= factor();
				} else {
					result /= factor();
				}
			}
			break;
		}
		default:
			break;
	}
	return result;
}

int factor() {
	switch (token) {
		case NUMBER: {
		  int result = atoi(yytext);
			token = yylex();
			return result;
		}
		case LEFT_PAR: {
			token = yylex();
			int result = expr();
			if(token == RIGHT_PAR) {
				token = yylex();
			} else {
				printf("Error: expected ')'\n");
			}
			return result;
		}
		default:
			printf("Error: expected number or '('\n");
			return 0;
	}
}
