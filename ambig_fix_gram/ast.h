#ifndef _AST_H
#define _AST_H

#ifndef DEC_FORMAT
  #include "defs.h"
#endif

#include <list>
#include "stdio.h"
#include <string>

#define DEF_OPERATOR_EXPR(name) \
  class name##Expr : public BinaryExpr{\
  public:\
    name##Expr(Expr* LeftSide, Expr *RightSide): BinaryExpr(LeftSide, RightSide){} \
    int eval(); \
  }

using namespace std;

class Expr{
public:
  virtual int eval() = 0;
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
  NumExpr(int value){ this->value = value; }

  int eval(){ return this->value; }

  int value;
};

class VarExpr : public Expr{
public:
  VarExpr(int index){ this->index = index; }

  int eval();

  int index;
};

class IdExpr: public Expr{
public:
  IdExpr(char* varName){ this->varName.append( varName ); }

  int eval();

  string varName;
};

DEF_OPERATOR_EXPR(Equals);
DEF_OPERATOR_EXPR(NotEquals);
DEF_OPERATOR_EXPR(LeqThan);
DEF_OPERATOR_EXPR(GreqThan);
DEF_OPERATOR_EXPR(LessThan);
DEF_OPERATOR_EXPR(GreaterThan);

class Statement{
public:
  virtual void exec() = 0;
};

class AssignmentStatement: public Statement{
public:
  AssignmentStatement(Expr* expression){
    this->expression = expression;
  }

  Expr* expression;
};

class DollarAssignmentSt: public AssignmentStatement{
public:
  DollarAssignmentSt(int index, Expr* expression): AssignmentStatement(expression){
    this->expression = expression;
    this->index = index;
  }

  void exec();

  int index = 0;
};

class IdAssignmentSt: public AssignmentStatement{
public:
  IdAssignmentSt(char* varName, Expr* expression): AssignmentStatement(expression){
    this->expression = expression;
    this->varName.append( varName );
  }

  void exec();

  string varName;
};


class BlockStatement: public Statement{
public:
  BlockStatement(){}

  void addStatement(Statement* statement){
    listOfStatements.push_back(statement);
  }

  void exec();

  list<Statement*> listOfStatements;
};

class PrintStatement: public Statement{
public:
  PrintStatement(Expr* expression, int format){
    this->expression = expression;
    this->format = format;
  }

  void exec();

  int format = DEC_FORMAT;
  Expr* expression;
private:
  void printBits(void const* const pointer );
};

class IfStatement: public Statement{
public:
  IfStatement(Expr* condition, Statement* truePart, Statement* falsePart){
    this->condition = condition;
    this->truePart = truePart;
    this->falsePart = falsePart;
  }

  void exec();

  Expr* condition;
  Statement* truePart;
  Statement* falsePart;
};

#endif
