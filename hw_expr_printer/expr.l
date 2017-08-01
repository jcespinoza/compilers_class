%option noyywrap
%option yylineno
%x comment
%x include
%x includeDiamond
%{
#include "defs.h"
#include "lexer_code.c"

int process_include(char end_symbol);
%}

%%

[ \t]        { /* NADA */ }
^"#"[ \t]*"include"[ \t]*"<"  { BEGIN(includeDiamond); }
^"#"[ \t]*"include"[ \t]*\"  { BEGIN(include); }
"$"[0-7]     { yylval = yytext[1] - '0'; return TK_VAR_SIGN; }
"/*"         { BEGIN(comment); }
"//".*\n     { return TK_EOL; }
"="          { return OP_ASSIGN; }
"print"      { return KW_PRINT; }
"dec"|"hex"|"bin" {
              if(strcmp(yytext, "hex") == 0 ){
                yylval = HEX_FORMAT;
              }else if(strcmp(yytext, "bin") == 0){
                yylval = BIN_FORMAT;
              }else{
                yylval = DEC_FORMAT;
              }

              return TK_FORMAT;
             }
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
                    return process_include('"');
                  }
<includeDiamond>[^>\n]+ {
                    return process_include('>');
                  }
<comment>"*/" { BEGIN(INITIAL); }
<comment>.|\n { /* NADA */ }
<comment><<EOLF>> { printf("Line %d: Unmatched block comment\n", yylineno); return 0; }
%%

int process_include(char end_symbol){
  int cInput;
  char cBlank = ' ';
  //byte foundEnd = 0;
  while((cInput = input()) && cInput != '\n');

  yylineno++;

  if(push_file(yytext) != 0){
    yyterminate();
  }

  BEGIN(INITIAL);
}
