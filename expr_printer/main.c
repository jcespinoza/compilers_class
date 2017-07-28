#include <stdio.h>

int expr();
int yylex();
void yyparse();
extern FILE* yyin;

int main(int argc, char* argv[]) {
  if(argc <= 2){
    fprintf(stderr, "Usage: %s <input file 1> <input file 2>...\n", argv[0]);
    return 1;
  }

  argc--;
  argv++;
  while(argc > 0){
    yyin = fopen(argv[0], "r");

    if(yyin == NULL){
      fprintf(stderr, "Cannot open file %s\n", argv[0]);
      return 1;
    }

  	yyparse();

    fclose(yyin);
    argc--;
    argv++;
  }
}
