%option noyywrap

%{
#include <stdio.h>
#include "tokens.h"
%}

%%

[ \t] { /* NADA */ }
"+" { return OP_ADD; }
"-" { return OP_SUB; }
"*" { return OP_MUL; }
"/" { return OP_DIV; }
"(" { return LEFT_PAR; }
")" { return RIGHT_PAR; }
[0-9]+ { return NUMBER; }
"\n" { return TK_EOL; }
<<EOF>> { return TK_EOF; }
. { return TK_ERROR; }

%%

/*int main()
{
  int token;

  while ( (token = yylex()) != TK_EOF) {
    printf("%d: %s\n",token,yytext);
  }
}*/
