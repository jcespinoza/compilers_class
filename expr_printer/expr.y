%{
#include <stdio.h>

int yylex();
void yyerror(const char* msg){
  printf("%s\n", msg);
}

#define YYERROR_VERBOSE
%}

%token OP_ADD OP_SUB
%token OP_MUL OP_DIV
%token TK_LEFT_PAR
%token TK_RIGHT_PAR
%token TK_NUMBER
%token TK_EOL TK_EOF TK_ERROR

%%

input: expr { printf("%d\n", $1); }
    ;
expr : expr OP_ADD term { $$ = $1 + $3; }
    | expr OP_SUB term { $$ = $1 - $3; }
    | term { $$ = $1; }
    ;

term : term OP_MUL factor { $$ = $1 * $3; }
    | term OP_DIV factor { $$ = $1 / $3; }
    | factor { $$ = $1; }
    ;

factor: TK_NUMBER { $$ = $1; }
    | TK_LEFT_PAR expr TK_RIGHT_PAR { $$ = $2; }
    ;
