#include <cstdio>
#include "ast.h"
#include "tokens.h"
#include <sstream>


extern Statement *input;
extern FILE *yyin;
extern int yylineno;

void lexer_init();

int main(int argc, char *argv[])
{
	if (argc < 2) {
		fprintf(stderr, "Usage: %s <input file>\n", argv[0]);
		return 1;
	}
	bool exec = true;
	if(argc > 2){
		char* flag1 = argv[2];
		if(flag1[0] == '-' && (flag1[1] == 's' || flag1[1] == 'S')){
			exec = false;
		}else{
			fprintf(stderr, "Unsupported flag: %s\n", argv[2]);
			return 1;
		}
	}
	yyin  = fopen(argv[1], "rb");

	if (yyin == NULL) {
		fprintf(stderr, "Cannot open file %s\n", argv[1]);
		return 2;
	}

	yylineno = 1;
	lexer_init();
    input = NULL;
    yyparse();

    if (input != 0) {
			if(exec){
				input->execute();
			}else{
				Scope scope(NULL);

				stringstream output;

				SynthMIPS text = input->generateCode(scope);
				output << ".global main\n\n";
				output << ".data\n\n";
				output << "  msg: .asciz \"Hello World\\n\"\n";
				output << scope.getGlobals();
				output << ".text\n";

				output << "main:";
				output << "    li $a0, BRIGHT_WHITE\n";
		    output << "    li $a1, BLACK\n";
		    output << "    jal set_color\n";
		    output << "    jal clear_screen\n";
				output << text.code;
				printf("This is the source:\n%s", output.str().c_str());
			}
    }
}
