#include "ast.h"

int variables[10];

int AddExpr::eval(){
  int leftValue = LeftSide->eval();
  int rightValue = RightSide->eval();

  return leftValue + rightValue;
}

int SubExpr::eval(){
  int leftValue = LeftSide->eval();
  int rightValue = RightSide->eval();

  return leftValue - rightValue;
}

int MultExpr::eval(){
  int leftValue = LeftSide->eval();
  int rightValue = RightSide->eval();

  return leftValue * rightValue;
}

int DivExpr::eval(){
  int leftValue = LeftSide->eval();
  int rightValue = RightSide->eval();

  return leftValue / rightValue;
}

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
