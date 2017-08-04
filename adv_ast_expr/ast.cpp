#include "ast.h"

#define IMPL_BIN_EXPR_EVAL(name, op)    \
  int name##Expr::eval(){               \
    int leftValue = LeftSide->eval();   \
    int rightValue = RightSide->eval(); \
    return leftValue op rightValue;     \
  }

int variables[10];

IMPL_BIN_EXPR_EVAL(Add, +)

IMPL_BIN_EXPR_EVAL(Sub, -)

IMPL_BIN_EXPR_EVAL(Mult, *)

IMPL_BIN_EXPR_EVAL(Div, /)

IMPL_BIN_EXPR_EVAL(Equals, ==)

IMPL_BIN_EXPR_EVAL(NotEquals, !=)

IMPL_BIN_EXPR_EVAL(LeqThan, <=)

IMPL_BIN_EXPR_EVAL(GreqThan, >=)

IMPL_BIN_EXPR_EVAL(LessThan, <)

IMPL_BIN_EXPR_EVAL(GreaterThan, >)

int VarExpr::eval(){
  return variables[index];
}

void AssignmentStatement::exec(){
  variables[index] = expression->eval();
}

void BlockStatement::exec(){
  list<Statement*>::const_iterator iterator, endOfList;
  iterator = listOfStatements.begin();
  endOfList = listOfStatements.end();

  for (; iterator != endOfList; ++iterator) {
    Statement* currentStatement = (Statement*)(*iterator);
    currentStatement->exec();
  }
}

void PrintStatement::exec(){
  int exprValue = expression->eval();
  if(format == HEX_FORMAT){
    printf("0x%032X\n", exprValue);
  }else if(format == BIN_FORMAT){
    printBits( (void const* const)(&exprValue) );
  }else{
    printf("%d\n", exprValue);
  }
}

void PrintStatement::printBits(void const* const pointer ){
  unsigned char *bits = (unsigned char*)pointer;
  unsigned char oneByte;
  int index, jindex;

  printf("0b");
  for(index = sizeof(int) - 1; index >= 0; index--){
    for(jindex = 7; jindex >= 0; jindex--){
      oneByte = (bits[index] >> jindex) & 1;
      printf("%u", oneByte);
    }
  }
  printf("\n");
}

void IfStatement::exec(){
  if(condition->eval()){
    truePart->exec();
  }else{
    if(falsePart != NULL){
      falsePart->exec();
    }
  }
}
