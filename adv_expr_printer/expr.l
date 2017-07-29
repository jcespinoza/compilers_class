%option noyywrap
%option yylineno
%x comment
%x include
%{
#include "lexer_code.c"

%}

%%


[ \t]        { /* NADA */ }
^"#"[ \t]*"include"[ \t]*\"  { BEGIN(include); }
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
<INITIAL><<EOF>>       { if(pop_file() != 0) yyterminate(); }
.            { return TK_ERROR; }

<include>[^\"\n]+ {
                {
                  int c;
                  while((c = input()) && c != '\n');
                  yylineno++;

                  if(push_file(yytext) != 0){
                    yyterminate();
                  }

                  BEGIN(INITIAL);
                }
              }
<comment>"*/" { BEGIN(INITIAL); }
<comment>.|\n { /* NADA */ }
<comment><<EOLF>> { printf("Line %d: Unmatched block comment\n", yylineno); return 0; }
%%
