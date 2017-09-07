#ifndef _AST_H
#define _AST_H

#define DEF_OPERATOR_EXPR(name) \
  class name##Expr : public BinaryExpr{\
  public:\
    name##Expr(Expr* LeftSide, Expr *RightSide): BinaryExpr(LeftSide, RightSide){} \
    SynthMIPS GenerateCode(Scope& scope); \
  }

#include <list>
#include "stdio.h"
#include <iostream>
#include <string>

using namespace std;

typedef struct SynthMIPS {
  string Place;
  string Code;
} SynthMIPS;

typedef struct Scope {

} Scope;

class Expr{
public:
  virtual SynthMIPS GenerateCode(Scope& scope) = 0;
};

class BinaryExpr: public Expr{
protected:
  BinaryExpr(Expr* LeftSide, Expr *RightSide){
    this->LeftSide = LeftSide;
    this->RightSide = RightSide;
  }
public:
  Expr* LeftSide, *RightSide;
};

DEF_OPERATOR_EXPR(Add);
DEF_OPERATOR_EXPR(Sub);
DEF_OPERATOR_EXPR(Mult);
DEF_OPERATOR_EXPR(Div);

class NumExpr : public Expr{
public:
  NumExpr(int value){
    this->value = value;
  }
  SynthMIPS GenerateCode(Scope& scope);
  int value;
};

class IdExpr: public Expr{
public:
  IdExpr(char* varName){ this->varName.append( varName ); }

  string varName;
  SynthMIPS GenerateCode(Scope& scope);
};

class Statement{
public:
  virtual SynthMIPS GenerateCode(Scope& scope) = 0;
};

class StatementList : public Statement{
public:
  StatementList(){

  }
  SynthMIPS GenerateCode(Scope& scope);
  void addStatement(Statement* st){ statements.push_back(st); }

  list<Statement*> statements;
};

class ExprStatement : public Statement{
public:
  ExprStatement(Expr* expression){
    this->expression = expression;
  }
  SynthMIPS GenerateCode(Scope& scope);
  Expr* expression;
};

class AssignmentStatement: public Statement{
public:
  AssignmentStatement(Expr* expression){
    this->expression = expression;
  }
  SynthMIPS GenerateCode(Scope& scope);
  Expr* expression;
};

class IdAssignmentSt: public AssignmentStatement{
public:
  IdAssignmentSt(char* varName, Expr* expression): AssignmentStatement(expression){
    this->expression = expression;
    this->varName.append( varName );
  }

  SynthMIPS GenerateCode(Scope& scope);

  string varName;
};

class Root {
public:
  Root(StatementList* statements){
    this->statementList = statements;
  }
  string GenerateCode(Scope& scope);
  StatementList* statementList;
};

#endif //_AST_H
