%option noyywrap
%option yylineno

%{

#include "tokens.h"
%}

%%

[ \t\n]                 { /* Nothing */ }
"//".*\n                { /* Nothing */ }
";"                     { return PNT_END_OF_STATEMENT; }
"("                     { return PNT_LEFT_PAREN; }
")"                     { return PNT_RIGHT_PAREN; }
"+"                     { return OP_PLUS; }
"-"                     { return OP_SUB; }
"*"                     { return OP_ASTERISK; }
"/"                     { return OP_DIV; }
"%"                     { return OP_MOD; }
[a-zA-Z_][a-zA-Z0-9_]*  { return TK_IDENTIFIER; }
[0-9]+                  { return LIT_INTEGER; }
.                       { fprintf(stderr, "Line %d: Invalid character\n", yylineno); return 0; }
