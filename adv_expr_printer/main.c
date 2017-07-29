#include <stdio.h>

int push_file(char* filename);
int yylex();
void yyparse();
extern FILE* yyin;

int main(int argc, char* argv[]) {
  if(argc != 2){
    fprintf(stderr, "Usage: %s <input file 1>\n", argv[0]);
    return 1;
  }

  if(push_file(argv[1]) != 0){
    return 1;
  }

  yyparse();
}
