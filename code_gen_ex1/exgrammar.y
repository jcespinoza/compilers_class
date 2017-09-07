%include {
  #include <cstdio>
  #include <string>
  #include <iostream>
  #include <assert.h>
  #include "ast.h"

  using namespace std;

  extern int yylineno;

  int yylex();

  void yyerror(const char *str)
  {
      printf("Line %d: %s\n", yylineno, str);
  }

  #define YYERROR_VERBOSE 1
  #define YYDEBUG 1
}

%syntax_error {
  fprintf(stderr, "Syntax error!\n");
  int n = sizeof(yyTokenName) / sizeof(yyTokenName[0]);
  for (int i = 0; i < n; ++i) {
          int a = yy_find_shift_action(yypParser, (YYCODETYPE)i);
          if (a < YYNSTATE + YYNRULE) {
                  printf("possible token: %s\n", yyTokenName[i]);
          }
  }
}

%default_type {int}

%type LIT_INTEGER { int }
%type TK_IDENTIFIER {char*}
%type expression { Expr* }
%type addi_expr { Expr* }
%type term { Expr* }
%type factor { Expr* }
%type statement { Statement*  }
%type statement_list { StatementList* }

%start_symbol input
%name EXParse

input ::= statement_list(SL) .
    {
      Root ast(SL);
      Scope scope;
      string code = ast.GenerateCode(scope);
      cout << code << "\n";
    }

statement_list(SL) ::= statement_list(L) statement(S) . { ((StatementList*)S)->addStatement(S); SL = L; }
statement_list(SL) ::= statement(S) . { SL = new StatementList(); SL->addStatement(S); }

//statement ::= assignment .
statement(S) ::= expression(E) PNT_END_OF_STATEMENT . { S = new ExprStatement(E); }

//assignment ::= TK_IDENTIFIER OP_ASSIGN expression PNT_END_OF_STATEMENT .

expression(E) ::= addi_expr(A) . { E = A; }

addi_expr(AX) ::= addi_expr(A) OP_PLUS term(T) . { AX = new AddExpr(A, T); }
addi_expr(AX) ::= addi_expr(A) OP_SUB term(T) . { AX = new SubExpr(A, T); }
addi_expr(A) ::= term(T) . { A = T; }

term(TE) ::= term(T) OP_ASTERISK factor(F) . { TE = new MultExpr(T, F); }
term(TE) ::= term(T) OP_DIV factor(F) . { TE = new DivExpr(T, F); }
term ::= term OP_MOD factor .
term(T) ::= factor(F) . { T = F; }

factor(F) ::= LIT_INTEGER(NUM) . { F = new NumExpr((int)(*(int*)NUM)); }
factor(F) ::= TK_IDENTIFIER(id) . { F = new IdExpr((char*)id); }
factor(F) ::= PNT_LEFT_PAREN expression(ex) PNT_RIGHT_PAREN . { F = ex; }
factor ::= OP_SUB factor .
