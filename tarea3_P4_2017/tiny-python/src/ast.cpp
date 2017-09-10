#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include "ast.h"

using namespace std;

map<string, int> vars;

int expt(int p, unsigned int q)
{
    int r = 1;

    while (q != 0) {
        if (q % 2 == 1) {    // q is odd
            r *= p;
            q--;
        }
        p *= p;
        q /= 2;
    }

    return r;
}

int InputExpr::evaluate()
{
	int result;

	cout << prompt;
	fflush(stdin);
	scanf("%d", &result);

	return result;
}

int CallExpr::evaluate()
{
    switch (fnId) {
        case FN_TIMECLOCK: return clock();
        case FN_RANDINT: {
            int start = arg0->evaluate();
            int end = arg1->evaluate();
            int range = end - start + 1;

            return (rand() % range) + start;
        }
        default:
            return 0;
    }
}

void BlockStatement::execute()
{
    list<Statement *>::iterator it = stList.begin();

    while (it != stList.end()) {
        Statement *st = *it;

        st->execute();
        it++;
    }
}

SynthMIPS BlockStatement::generateCode(Scope& scope){
  SynthMIPS result;
  list<Statement *>::iterator it = stList.begin();
  stringstream code;

  while (it != stList.end()) {
      Statement *st = *it;

      code << st->generateCode(scope).code;

      it++;
  }
  code << "\n";
  result.code = code.str();
  return result;
}

void PrintStatement::execute()
{
  list<Expr *>::iterator it = lexpr.begin();

  while (it != lexpr.end()) {
    Expr *expr = *it;

    if (expr->isA(STRING_EXPR)) {
      printf("%s", ((StringExpr*)expr)->str.c_str());
    } else {
      int result = expr->evaluate();
      printf("%d", result);
    }

    it++;
  }
  printf("\n");
}

SynthMIPS PrintStatement::generateCode(Scope& scope){
  SynthMIPS result;
  stringstream code;
  list<Expr *>::iterator it = lexpr.begin();

  while (it != lexpr.end()) {
    Expr *expr = *it;

    SynthMIPS expSynth = expr->generateCode(scope);
    string expCode = expSynth.code;
    string expLocation = expSynth.location;

    if (expr->isA(STRING_EXPR)) {
      //((StringExpr*)expr)->str.c_str() //TODO: save to label in the .data section
      code << "    move $a0, " << expLocation << "\n";
      code << "    jal puts \n";
    } else {
      //SynthMIPS exprCode = expr->generateCode(scope);
      //TODO: fix that
      code << "    move $a0, " << expLocation << "\n";
      code << "    jal put_udecimal \n";
    }

    it++;
  }
  result.code = code.str();
  return result;
}



void AssignStatement::execute()
{
    int result = expr->evaluate();
    vars[id] = result;
}

SynthMIPS AssignStatement::generateCode(Scope& scope){
  SynthMIPS result;
  stringstream code;

  SynthMIPS expSynth = expr->generateCode(scope);
  string expCode = expSynth.code;
  string expLocation = expSynth.location;
  //scope.releaseRegister(expLocation); //TODO: remove this comment
  code << expCode;
  code << "    sw " << expLocation <<", " << id << "\n";

  result.code = code.str();
  return result;
}

void IfStatement::execute()
{
    int result = cond->evaluate();

    if (result) {
        trueBlock->execute();
    } else if (falseBlock != 0) {
        falseBlock->execute();
    }
}

SynthMIPS IfStatement::generateCode(Scope& scope){
  SynthMIPS result;
  stringstream code;

  SynthMIPS expSynth = cond->generateCode(scope);
  string expCode = expSynth.code;
  string expLocation = expSynth.location;

  string ifLabelName = scope.getLabelFor("if");
  string endIfLabelName = scope.getLabelFor("endif");
  string jumpIfFalseLabel = endIfLabelName;
  string elseLabelName = " ";

  code << expCode;
  code << ifLabelName << ":\n";

  if(falseBlock != NULL){
    elseLabelName = scope.getLabelFor("else");
    jumpIfFalseLabel = elseLabelName;
  }

  code << "    beqz " << expLocation << ", " << jumpIfFalseLabel << "\n";
  code << "    nop\n";
  SynthMIPS trueCode = trueBlock->generateCode(scope);
  code << trueCode.code;

  if(falseBlock != NULL){
    code << elseLabelName << ":\n";
    code << falseBlock->generateCode(scope).code;
  }
  code << endIfLabelName << ":\n";

  result.code = code.str();
  return result;
}

void WhileStatement::execute()
{
  int result = cond->evaluate();

  while (result) {
    block->execute();

    result = cond->evaluate();
  }
}

SynthMIPS WhileStatement::generateCode(Scope& scope){
  SynthMIPS result;
  stringstream code;

  SynthMIPS expSynth = cond->generateCode(scope);
  string expCode = expSynth.code;
  string expLocation = expSynth.location;

  string whileLabelName = scope.getLabelFor("while");
  string endWhileLabel = scope.getLabelFor("endwhile");
  code << whileLabelName << ":\n";
  code << expCode;
  code << "    beqz " << expLocation << ", " << endWhileLabel << "\n";
  SynthMIPS blockCode = block->generateCode(scope);

  code << blockCode.code;
  code << "j " << whileLabelName << "\n";
  code << endWhileLabel << ": \n";

  result.code = code.str();
  return result;
}

void ForStatement::execute()
{
	int val = startExpr->evaluate();
  	vars[id] = val;

	val = endExpr->evaluate();
	while (vars[id] < val) {
		block->execute();
		vars[id] = vars[id] + 1;
	}
}

SynthMIPS ForStatement::generateCode(Scope& scope){
  SynthMIPS result;
  stringstream code;

  SynthMIPS stExpSynth = startExpr->generateCode(scope);
  string stExpCode = stExpSynth.code;
  string stExpLocation = stExpSynth.location;

  code << stExpCode;
  code << "    sw " << stExpLocation << ", " << id << "\n";

  SynthMIPS endExpSynth = endExpr->generateCode(scope);
  string endExpCode = endExpSynth.code;
  string endExpLocation = endExpSynth.location;

  code << endExpCode;
  string tempVarReg = scope.getFreeRegister();
  code << "    move " << tempVarReg << ", " << endExpLocation << ", " << id << "\n";

  string forLabelName = scope.getLabelFor("for");
  string endForLabel = scope.getLabelFor("endfor");
  code << forLabelName << ":\n";
  string varValueReg = scope.getFreeRegister();
  code << "    lw " << varValueReg << ", " << id << "\n";
  code << "    sltu "  << varValueReg << ", " << tempVarReg << "\n";
  code << "    beqz " << varValueReg << ", " << endForLabel << "\n";

  SynthMIPS blockCode = block->generateCode(scope);
  code << blockCode.code;

  code << "j " << forLabelName << "\n";
  code << endForLabel << ":\n";

  result.code = code.str();
  return result;
}

void CallStatement::execute()
{
    switch (fnId) {
        case FN_RANDSEED: {
            int arg = arg0->evaluate();
            srand(arg);
        }
        default: {

        }
    }
}

SynthMIPS CallStatement::generateCode(Scope& scope){
  SynthMIPS result;
  stringstream code;

  switch (fnId) {
      case FN_RANDSEED: {
          return generateCallToRandSeed(scope);
      }
      default: {

      }
  }

  result.code = code.str();
  return result;
}

SynthMIPS CallStatement::generateCallToRandSeed(Scope& scope){
  SynthMIPS result;
  stringstream code;

  result.code = code.str();
  return result;
}

Scope::Scope(Scope* parentScope){
  if(parentScope != NULL){
    this->parentScope = parentScope;
    this->registers = parentScope->registers;
  }else{
    initRegisters();
    initLabels();
  }
}

void Scope::initRegisters(){
  registers = new map<string, bool>();

  map<string, bool> regs = *registers;
  regs["$t0"] = 0;
  regs["$t1"] = 0;
  regs["$t2"] = 0;
  regs["$t3"] = 0;
  regs["$t4"] = 0;
  regs["$t5"] = 0;
  regs["$t6"] = 0;
  regs["$t7"] = 0;
  regs["$t8"] = 0;
  regs["$t9"] = 0;
}

void Scope::initLabels(){
  labels = new map<string, int>();

  map<string, int> lab = *labels;
  lab["if"] = 0;
  lab["else"] = 0;
  lab["endif"] = 0;
  lab["for"] = 0;
  lab["endfor"] = 0;
  lab["while"] = 0;
  lab["endwhile"] = 0;
}

void Scope::releaseRegister(string name){
  (*registers)[name] = 0;
}

string Scope::getFreeRegister(){
  map<string, bool>::iterator it = (*registers).begin();
  for(; it != (*registers).end(); it++){
    if(it->second == false){
      it->second = true;
      return it->first;
    }
  }
}

string Scope::getLabelFor(string kind){
  map<string, int> lCounters = (*labels);
  map<string, int>::iterator it = lCounters.begin();
  kind.insert(0, ".");
  kind.append("_");
  kind.append( to_string(lCounters[kind]++) );
  return kind;
}


string Scope::getGlobals(){
  return "";
}
