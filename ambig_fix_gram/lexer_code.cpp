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

  include_stack_t* incStack = (include_stack_t*) malloc(sizeof(include_stack_t));

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
