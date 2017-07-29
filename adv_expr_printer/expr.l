%option noyywrap
%option yylineno
%x comment
%x include
%{
#include <stdio.h>
#include "tokens.h"

typedef struct include_stack{
  struct include_stack* prevState;
  FILE* file;
  char* filename;
  int lineNumber;
  YY_BUFFER_STATE bufferState;
} include_stack_t;

include_stack_t* global_inc_stack = NULL;
char* yyfilename;

int push_file(char* filename){
  FILE* file = fopen(filename, "r");

  if(file == NULL){
    fprintf(stderr, "File %s not found\n", filename);
    return 1;
  }

  include_stack_t* incStack = malloc(sizeof(include_stack_t));

  if(incStack == NULL){
    fprintf(stderr, "Out of memory.");
    return 1;
  }

  incStack->file = file;
  incStack->filename = yyfilename;
  incStack->lineNumber = yylineno;
  incStack->bufferState = yy_create_buffer(file, YY_BUF_SIZE);
  incStack->prevState = global_inc_stack;
  global_inc_stack  = incStack;
  yyfilename = filename;
  yylineno = 1;

  yy_switch_to_buffer(incStack->bufferState);

  return 0;
}

int pop_file(){
  include_stack_t* prev_incStack = global_inc_stack->prevState;
  fclose(global_inc_stack->file);
  yy_delete_buffer(global_inc_stack->bufferState);

  yylineno = global_inc_stack->lineNumber;
  yyfilename = global_inc_stack->filename;

  free(global_inc_stack);

  if(prev_incStack == NULL) return 1;

  yy_switch_to_buffer(prev_incStack->bufferState);

  global_inc_stack = prev_incStack;

  return 0;
}

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
