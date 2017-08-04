%code requires {
  #include "ast.h"
}

%{
#include <stdio.h>
#ifndef DEC_FORMAT
  #include "defs.h"
#endif

int yylex();
extern int yylineno;
extern char* yyfilename;

void yyerror(const char *msg){
  printf("File: %s Line %d : %s \n",yyfilename, yylineno, msg);
}

#define YYERROR_VERBOSE 1

%}

%union {
  Statement* statement_t;
  Expr* expr_t;
  int int_t;
}

%type <statement_t> statement var_assignment print statements
%type <statement_t> if_statement optional_else block_or_statement
%type <expr_t> expr term factor add_expr
%type<int_t> pr_format

%token OP_ASSIGN OP_EQUALS OP_NEQ
%token OP_ADD OP_SUB OP_GREATER_THAN OP_LESS_THAN
%token OP_MUL OP_DIV OP_GREQ_THAN OP_LEQ_THAN
%token KW_PRINT KW_IF KW_ELSE
%token TK_LEFT_PAR TK_LEFT_BRACE
%token TK_RIGHT_PAR TK_RIGHT_BRACE
%token<int_t> TK_NUMBER TK_VAR_SIGN
%token TK_EOL TK_EOF TK_ERROR
%token<int_t> TK_FORMAT

%%

start: opt_eols statements opt_eols { ((BlockStatement*) $2)->exec(); }
    ;

statements: statements eols statement { $$ = $1; ((BlockStatement*)$$)->addStatement($3); }
    | statement { $$ = new BlockStatement; ((BlockStatement*)$$)->addStatement($1); }
    ;

eols: eols TK_EOL
    | TK_EOL
    ;

opt_eols: eols
    |
    ;

statement: var_assignment { $$ = $1; }
    | print { $$ = $1; }
    | if_statement { $$ = $1; }
    ;

var_assignment: TK_VAR_SIGN OP_ASSIGN expr { $$ = new AssignmentStatement($1, $3); }
    ;

print: KW_PRINT expr pr_format { $$ = new PrintStatement($2, $3); }
    ;

pr_format:  TK_FORMAT { $$ = $1; }
    |  { $$ = DEC_FORMAT; }
    ;

if_statement: KW_IF TK_LEFT_PAR expr TK_RIGHT_PAR block_or_statement optional_else { $$ = new IfStatement($3, $5, $6); }
    ;

block_or_statement: TK_LEFT_BRACE opt_eols statements opt_eols TK_RIGHT_BRACE { $$ = $3;}
    | statement { $$ = $1; }
    ;

optional_else: KW_ELSE block_or_statement { $$ = $2; }
    | { $$ = NULL; }
    ;

expr: expr OP_EQUALS add_expr { $$ = new EqualsOp($1, $3); }
    | expr OP_NEQ add_expr { $$ = new NotEqualsOp($1, $3); }
    | expr OP_LEQ_THAN add_expr { $$ = new LeqThanOp($1, $3); }
    | expr OP_GREQ_THAN add_expr { $$ = new GreqThanOp($1, $3); }
    | expr OP_LESS_THAN add_expr { $$ = new LessThanOp($1, $3); }
    | expr OP_GREATER_THAN add_expr { $$ = new GreaterThanOp($1, $3); }
    | add_expr { $$ = $1; }
    ;

add_expr: add_expr OP_ADD term { $$ = new AddExpr($1, $3); }
    | add_expr OP_SUB term { $$ = new SubExpr($1, $3); }
    | term { $$ = $1; }
    ;

term : term OP_MUL factor { $$ = new MultExpr($1, $3); }
    | term OP_DIV factor { $$ = new DivExpr($1, $3); }
    | factor { $$ = $1; }
    ;

factor: TK_NUMBER { $$ = new NumExpr( $1 ); }
    | TK_VAR_SIGN {$$ = new VarExpr($1); }
    | TK_LEFT_PAR expr TK_RIGHT_PAR { $$ = $2; }
    ;
