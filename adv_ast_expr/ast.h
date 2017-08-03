#ifndef _AST_H
#define _AST_H

#ifndef DEC_FORMAT
  #include "defs.h"
#endif

#include <list>
#include "stdio.h"

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

class AddExpr : public BinaryExpr{
public:
  AddExpr(Expr* LeftSide, Expr *RightSide): BinaryExpr(LeftSide, RightSide){}

  int eval();
};

class SubExpr : public BinaryExpr{
public:
  SubExpr(Expr* LeftSide, Expr *RightSide): BinaryExpr(LeftSide, RightSide){}

  int eval();
};

class MultExpr : public BinaryExpr{
public:
  MultExpr(Expr* LeftSide, Expr *RightSide): BinaryExpr(LeftSide, RightSide){}

  int eval();
};

class DivExpr : public BinaryExpr{
public:
  DivExpr(Expr* LeftSide, Expr *RightSide): BinaryExpr(LeftSide, RightSide){}

  int eval();
};

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

class EqualsOp : public BinaryExpr{
public:
  EqualsOp(Expr* LeftSide, Expr *RightSide): BinaryExpr(LeftSide, RightSide){}

  int eval();
};

class NotEqualsOp : public BinaryExpr{
public:
  NotEqualsOp(Expr* LeftSide, Expr *RightSide): BinaryExpr(LeftSide, RightSide){}

  int eval();
};

class LeqThanOp : public BinaryExpr{
public:
  LeqThanOp(Expr* LeftSide, Expr *RightSide): BinaryExpr(LeftSide, RightSide){}

  int eval();
};

class GreqThanOp : public BinaryExpr{
public:
  GreqThanOp(Expr* LeftSide, Expr *RightSide): BinaryExpr(LeftSide, RightSide){}

  int eval();
};

class LessThanOp : public BinaryExpr{
public:
  LessThanOp(Expr* LeftSide, Expr *RightSide): BinaryExpr(LeftSide, RightSide){}

  int eval();
};

class GreaterThanOp : public BinaryExpr{
public:
  GreaterThanOp(Expr* LeftSide, Expr *RightSide): BinaryExpr(LeftSide, RightSide){}

  int eval();
};

class Statement{
public:
  virtual void exec() = 0;
};

class AssignmentStatement: public Statement{
public:
  AssignmentStatement(int index, Expr* expression){
    this->expression = expression;
    this->index = index;
  }

  void exec();

  Expr* expression;
  int index = 0;
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
