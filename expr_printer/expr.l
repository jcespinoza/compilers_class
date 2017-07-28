%option noyywrap
%option yylineno
%x comment
%{
#include <stdio.h>
#include "tokens.h"
%}

%%

[ \t]        { /* NADA */ }
"$"[0-7]     { yylval = yytext[1] - '0'; return TK_VAR_SIGN; }
"/*"         { BEGIN(comment); }
"//".*\n     { return TK_EOL; }
"="          { return OP_ASSIGN; }
"print"      { return KW_PRINT; }
"+"          { return OP_ADD; }
"-"          { return OP_SUB; }
"*"          { return OP_MUL; }
"/"          { return OP_DIV; }
"("          { return TK_LEFT_PAR; }
")"          { return TK_RIGHT_PAR; }
[0-9]+       { yylval =  atoi(yytext); return TK_NUMBER; }
"\n"         { return TK_EOL; }
.            { return TK_ERROR; }
<comment>"*/" { BEGIN(INITIAL); }
<comment>.|\n { /* NADA */ }
<comment><<EOLF>> { printf("Line %d: Unmatched block comment\n", yylineno); return 0; }
%%
