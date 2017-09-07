#include <cstdlib>
#include <cstdio>
#include "tokens.h"

void* EXParseAlloc(void* (*allocProc)(size_t));
void EXParse(void*, int, void*);
void* EXParseFree(void*, void(*freeProc)(void*));
extern char* yytext;
int yylex();

int main(){
  // Set up the parser
  void* shellParser = EXParseAlloc(malloc);

  // Do it!
  int lexCode;
  while( (lexCode = yylex()) != 0){
    EXParse(shellParser, lexCode, yytext);
  }

  EXParse(shellParser, 0, NULL);

  if (-1 == lexCode) {
      fprintf(stderr, "The scanner encountered an error.\n");
  }

  // Cleanup the scanner and parser
  EXParseFree(shellParser, free);
  return 0;
}
