%option noyywrap
%option yylineno
%x comment
%x include
%x includeDiamond
%x eols
%x block_comment_eols
%{
#include "defs.h"
#include "lexer_code.cpp"
void yyerror(const char* message);
int process_include(char end_symbol);
%}

%%

[ \t]        { /* NADA */ }
^"#"[ \t]*"include"[ \t]*"<"  { BEGIN(includeDiamond); }
^"#"[ \t]*"include"[ \t]*\"  { BEGIN(include); }
"$"[0-7]     { yylval.int_t = yytext[1] - '0'; return TK_VAR_SIGN; }
"/*"         { BEGIN(comment); }
"//".*\n     { BEGIN(eols); }

"!="         { return OP_NEQ; }
"=="         { return OP_EQUALS; }
"="          { return OP_ASSIGN; }
"<="         { return OP_LEQ_THAN; }
">="         { return OP_GREQ_THAN; }
"<"          { return OP_LESS_THAN; }
">"          { return OP_GREATER_THAN; }
"if"         { return KW_IF; }
"else"       { yyerror("Else keyword was not expected here."); return KW_ELSE; }
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
[_a-zA-Z][_a-zA-Z0-9]* {
                char* varName = (char*)malloc(strlen(yytext));
                strcpy(varName, yytext);

                yylval.string_t = varName;
                return TK_IDENTIFIER;
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
"\n"         { BEGIN(eols); }
<INITIAL><<EOF>>       { if(pop_file() != 0) yyterminate(); }
.            { return TK_ERROR; }

<eols>\n      { /*Nothing */ }
<eols>"//"    { /*Nothing */  }
<eols>[ \t]   { /* Nothing */ }
<eols>"/*"    { BEGIN(block_comment_eols); }
<eols>"else"  { BEGIN(INITIAL); return KW_ELSE; }
<eols><<EOF>> { BEGIN(INITIAL); return TK_EOL; }
<eols>.       { BEGIN(INITIAL); unput(yytext[0]); return TK_EOL; }

<block_comment_eols>"*/" { BEGIN(eols); }
<block_comment_eols>.|\n { /* NADA */ }
<block_comment_eols><<EOLF>> { printf("Line %d: Unmatched block comment\n", yylineno); return 0; }

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
