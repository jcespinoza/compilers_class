%option noyywrap
%option yylineno
%x comment
%x include
%x includeDiamond
%{
#include "defs.h"
#include "lexer_code.cpp"

int process_include(char end_symbol);
%}

%%

[ \t]        { /* NADA */ }
^"#"[ \t]*"include"[ \t]*"<"  { BEGIN(includeDiamond); }
^"#"[ \t]*"include"[ \t]*\"  { BEGIN(include); }
"$"[0-7]     { yylval.int_t = yytext[1] - '0'; return TK_VAR_SIGN; }
"/*"         { BEGIN(comment); }
"//".*\n     { return TK_EOL; }

"!="         { return OP_NEQ; }
"=="         { return OP_EQUALS; }
"="          { return OP_ASSIGN; }
"<="         { return OP_LEQ_THAN; }
">="         { return OP_GREQ_THAN; }
"<"          { return OP_LESS_THAN; }
">"          { return OP_GREATER_THAN; }
"if"         { return KW_IF; }
"else"       { return KW_ELSE; }
"print"      { return KW_PRINT; }
"dec"|"hex"|"bin" {
              if(strcmp(yytext, "hex") == 0 ){
                yylval.int_t = HEX_FORMAT;
              }else if(strcmp(yytext, "bin") == 0){
                yylval.int_t = BIN_FORMAT;
              }else{
                yylval.int_t = DEC_FORMAT;
              }

              return TK_FORMAT;
             }
"+"          { return OP_ADD; }
"-"          { return OP_SUB; }
"*"          { return OP_MUL; }
"/"          { return OP_DIV; }
"("          { return TK_LEFT_PAR; }
")"          { return TK_RIGHT_PAR; }
"{"          { return TK_LEFT_BRACE; }
"}"          { return TK_RIGHT_BRACE; }
[0-9]+       { yylval.int_t =  atoi(yytext); return TK_NUMBER; }
"\n"         { return TK_EOL; }
<INITIAL><<EOF>>       { if(pop_file() != 0) yyterminate(); }
.            { return TK_ERROR; }

<include>[^\"\n]+ {
                    process_include('"');
                  }
<includeDiamond>[^>\n]+ {
                    process_include('>');
                  }
<comment>"*/" { BEGIN(INITIAL); }
<comment>.|\n { /* NADA */ }
<comment><<EOLF>> { printf("Line %d: Unmatched block comment\n", yylineno); return 0; }
%%

int process_include(char end_symbol){
  int cInput;
  char cBlank = ' ';
  //byte foundEnd = 0;
  while((cInput = yyinput()) && cInput != '\n');

  yylineno++;

  if(push_file(yytext) != 0){
    yyterminate();
  }

  BEGIN(INITIAL);
}
