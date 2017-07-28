%{
#include <stdio.h>

int yylex();
extern int yylineno;
void yyerror(const char *msg){
  printf("Line %d : %s \n",yylineno,msg);
}

#define YYERROR_VERBOSE 1

int variables[10];

%}

%token OP_ASSIGN
%token OP_ADD OP_SUB
%token OP_MUL OP_DIV
%token KW_PRINT
%token TK_LEFT_PAR
%token TK_RIGHT_PAR
%token TK_NUMBER TK_VAR_SIGN
%token TK_EOL TK_EOF TK_ERROR

%%

start: opt_eols statements opt_eols
    ;

statements: statements eols statement
    | statement
    ;

eols: eols TK_EOL
    | TK_EOL
    ;

opt_eols: eols
    |
    ;

statement: var_assignment
    | print
    ;

var_assignment: TK_VAR_SIGN OP_ASSIGN expr { variables[$1] = $3; }
    ;

print: KW_PRINT expr { printf("%d\n", $2); }
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
    | TK_VAR_SIGN {$$ = variables[$1]; }
    | TK_LEFT_PAR expr TK_RIGHT_PAR { $$ = $2; }
    ;
