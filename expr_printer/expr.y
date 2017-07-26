%{
#include <stdio.h>

int yylex();
void yyerror(const char* msg){
  printf("%s\n", msg);
}

#define YYERROR_VERBOSE

int variables[10];

int counter = 0;
for(counter = 0; counter < 10; counter++){
  variables[counter] = 0;
}

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

input:  input input_line
    |
    ;

input_line:  var_assignment TK_EOL { $$ = $1; }
    |   print TK_EOL { $$ = $1; }
    ;

var_assignment: TK_VAR_SIGN TK_NUMBER OP_ASSIGN expr { variables[$2] = $3; }
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
    | TK_LEFT_PAR expr TK_RIGHT_PAR { $$ = $2; }
    ;

var_name: TK_VAR_SIGN TK_NUMBER {  }
