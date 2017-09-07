#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string.h>
#include "defs.h"
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
    if(lexCode == TK_IDENTIFIER){
      char* iChar = new char[strlen((char*)yytext)];
      strcpy(iChar, (char*)yytext);

      //token.char_t = iChar;
      EXParse(shellParser, lexCode, iChar);
    }else if(lexCode == LIT_INTEGER){
      char* nChar = new char[strlen((char*)yytext)];
      strcpy(nChar, (char*)yytext);
      EXParse(shellParser, lexCode, yytext);
    }else{
      EXParse(shellParser, lexCode, yytext);
    }
//    EXParse(shellParser, lexCode, &token);
  }

  EXParse(shellParser, 0, NULL);

  if (-1 == lexCode) {
      fprintf(stderr, "The scanner encountered an error.\n");
  }

  // Cleanup the scanner and parser
  EXParseFree(shellParser, free);
  return 0;
}
