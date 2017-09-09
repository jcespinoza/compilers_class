#ifndef _AST_H_
#define _AST_H_

#include <string>
#include <list>
#include <map>

using namespace std;

extern map<string, int> vars;

int expt(int p, unsigned int q);

enum BuiltInFunct {
    FN_TIMECLOCK,
    FN_RANDSEED,
    FN_RANDINT
};

enum ExprKind {
  LT_EXPR,
  LTE_EXPR,
  GT_EXPR,
  GTE_EXPR,
  NE_EXPR,
  EQ_EXPR,
  ADD_EXPR,
  SUB_EXPR,
  MULT_EXPR,
  DIV_EXPR,
  MOD_EXPR,
  EXPT_EXPR,
  NUM_EXPR,
  ID_EXPR,
  STRING_EXPR,
  INPUT_EXPR,
  CALL_EXPR
};

enum LocationType{

};

class Scope {
public:
  Scope(Scope* parentScope);

  map<string, bool>* registers;
  Scope* parentScope;

  void releaseRegister(string name);

  string getFreeRegister();

private:
  void initRegisters();
};


class SynthMIPS {
public:
  string location;
  string code;
};

class Expr;
typedef list<Expr*> ExprList;

class Expr {
public:
    virtual int evaluate() = 0;
    virtual int getKind() = 0;
    bool isA(int kind) { return (getKind() == kind); }
};

class BinaryExpr: public Expr {
public:
    BinaryExpr(Expr *expr1, Expr *expr2) {
        this->expr1 = expr1;
        this->expr2 = expr2;
    }

    Expr *expr1;
    Expr *expr2;
};

class LTExpr: public BinaryExpr {
public:
    LTExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    int evaluate() { return expr1->evaluate() < expr2->evaluate(); }
    int getKind() { return LT_EXPR; }
};

class GTExpr: public BinaryExpr {
public:
    GTExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    int evaluate() { return expr1->evaluate() > expr2->evaluate(); }
    int getKind() { return GT_EXPR; }
};

class LTEExpr: public BinaryExpr {
public:
    LTEExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    int evaluate() { return expr1->evaluate() <= expr2->evaluate(); }
    int getKind() { return LTE_EXPR; }
};

class GTEExpr: public BinaryExpr {
public:
    GTEExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    int evaluate() { return expr1->evaluate() >= expr2->evaluate(); }
    int getKind() { return GTE_EXPR; }
};

class NEExpr: public BinaryExpr {
public:
    NEExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    int evaluate() { return expr1->evaluate() != expr2->evaluate(); }
    int getKind() { return NE_EXPR; }
};

class EQExpr: public BinaryExpr {
public:
    EQExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    int evaluate() { return expr1->evaluate() == expr2->evaluate(); }
    int getKind() { return NE_EXPR; }
};

class AddExpr: public BinaryExpr {
public:
    AddExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    int evaluate() { return expr1->evaluate() + expr2->evaluate(); }
    int getKind() { return ADD_EXPR; }
};

class SubExpr: public BinaryExpr {
public:
    SubExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    int evaluate() { return expr1->evaluate() - expr2->evaluate(); }
    int getKind() { return SUB_EXPR; }
};

class MultExpr: public BinaryExpr {
public:
    MultExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    int evaluate() { return expr1->evaluate() * expr2->evaluate(); }
    int getKind() { return MULT_EXPR; }
};

class DivExpr: public BinaryExpr {
public:
    DivExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    int evaluate() { return expr1->evaluate() / expr2->evaluate(); }
    int getKind() { return DIV_EXPR; }
};

class ModExpr: public BinaryExpr {
public:
    ModExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    int evaluate() { return expr1->evaluate() % expr2->evaluate(); }
    int getKind() { return EXPT_EXPR; }
};

class ExponentExpr: public BinaryExpr {
public:
    ExponentExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    int evaluate() { return expt(expr1->evaluate(), expr2->evaluate()); }
    int getKind() { return MOD_EXPR; }
};

class NumExpr: public Expr {
public:
    NumExpr(int value) { this->value = value; }
    int evaluate() { return value; }
    int getKind() { return NUM_EXPR; }

    int value;
};

class IdExpr: public Expr {
public:
    IdExpr(string id) { this->id = id; }
    int evaluate() { return vars[id]; }
    int getKind() { return ID_EXPR; }

    string id;
};

class StringExpr: public Expr {
public:
    StringExpr(string str) { this->str = str; }
    int evaluate() { return 0; }
    int getKind() { return STRING_EXPR; }

    string str;
};

class InputExpr: public Expr {
public:
    InputExpr(string prompt) { this->prompt = prompt; }
    int evaluate();
    int getKind() { return INPUT_EXPR; }

    string prompt;
};

class CallExpr: public Expr {
public:
    CallExpr(BuiltInFunct fnId) {
        this->fnId = fnId;
    }
    CallExpr(BuiltInFunct fnId, Expr *arg0, Expr *arg1) {
        this->fnId = fnId;
        this->arg0 = arg0;
        this->arg1 = arg1;
    }
    int evaluate();
    int getKind() { return CALL_EXPR; }

    BuiltInFunct fnId;
    Expr *arg0, *arg1;
};

enum StatementKind {
    BLOCK_STATEMENT,
    PRINT_STATEMENT,
    ASSIGN_STATEMENT,
    IF_STATEMENT,
    WHILE_STATEMENT,
	FOR_STATEMENT,
	PASS_STATEMENT,
    CALL_STATEMENT
};

class Statement {
public:
    virtual void execute() = 0;
    virtual StatementKind getKind() = 0;
    virtual SynthMIPS generateCode(Scope& scope) = 0;
};

class BlockStatement: public Statement {
public:
    BlockStatement() {}
    void execute();
    SynthMIPS generateCode(Scope& scope);
    StatementKind getKind() { return BLOCK_STATEMENT; }
    void add(Statement *st) { stList.push_back(st); }

    list<Statement *> stList;
};


class AssignStatement: public Statement {
public:
    AssignStatement(string id, Expr *expr) {
        this->id = id;
        this->expr = expr;
    }
    void execute();
    SynthMIPS generateCode(Scope& scope);
    StatementKind getKind() { return ASSIGN_STATEMENT; }

    string id;
    Expr *expr;
};

class PrintStatement: public Statement {
public:
    PrintStatement(ExprList lexpr) {
        this->lexpr = lexpr;
    }
    void execute();
    SynthMIPS generateCode(Scope& scope);
    StatementKind getKind() { return PRINT_STATEMENT; }

    ExprList lexpr;
};

class IfStatement: public Statement {
public:
    IfStatement(Expr *cond, Statement *trueBlock, Statement *falseBlock) {
        this->cond = cond;
        this->trueBlock = trueBlock;
        this->falseBlock = falseBlock;
    }
    void execute();
    SynthMIPS generateCode(Scope& scope);
    StatementKind getKind() { return IF_STATEMENT; }

    Expr *cond;
    Statement *trueBlock;
    Statement *falseBlock;
};

class PassStatement: public Statement {
public:
    PassStatement() {
    }
    void execute() {} ;
    SynthMIPS generateCode(Scope& scope){ SynthMIPS result; return result; }
    StatementKind getKind() { return PASS_STATEMENT; }
};

class WhileStatement: public Statement {
public:
    WhileStatement(Expr *cond, Statement *block) {
        this->cond = cond;
        this->block = block;
    }
    void execute();
    SynthMIPS generateCode(Scope& scope);
    StatementKind getKind() { return WHILE_STATEMENT; }

    Expr *cond;
    Statement *block;
};

class ForStatement: public Statement {
public:
    ForStatement(string id, Expr *startExpr, Expr *endExpr, Statement *block) {
        this->id = id;
        this->startExpr = startExpr;
        this->endExpr = endExpr;
        this->block = block;
    }
    void execute();
    SynthMIPS generateCode(Scope& scope);
    StatementKind getKind() { return FOR_STATEMENT; }

    string id;
    Expr *startExpr;
    Expr *endExpr;
    Statement *block;
};

class CallStatement: public Statement {
public:
    CallStatement(BuiltInFunct fnId, Expr *arg0) {
        this->fnId = fnId;
        this->arg0 = arg0;
        this->arg1 = NULL;
    }
    CallStatement(BuiltInFunct fnId, Expr *arg0, Expr *arg1): CallStatement(fnId, arg0) {
        this->arg1 = arg1;
    }
    void execute();
    SynthMIPS generateCode(Scope& scope);
    StatementKind getKind() { return CALL_STATEMENT; }

    BuiltInFunct fnId;
    Expr *arg0, *arg1;
};

#endif
