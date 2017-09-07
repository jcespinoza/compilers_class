#include "ast.h"

string Root::GenerateCode(Scope& scope){
  return statementList->GenerateCode(scope).Code;
}

SynthMIPS StatementList::GenerateCode(Scope& scope){
  list<Statement*>::iterator it = statements.begin();
  SynthMIPS synth;
  for(; it != statements.end(); it++){
    SynthMIPS synth = (*it)->GenerateCode(scope);
    cout << synth.Code << "\n";
  }
  return synth;
}

SynthMIPS NumExpr::GenerateCode(Scope& scope){
  return SynthMIPS{
    "t0",
    "Here is a number\n"
  };
}

SynthMIPS IdExpr::GenerateCode(Scope& scope){
  return SynthMIPS{
    "t0",
    "Here's an identifier\n"
  };
}

SynthMIPS ExprStatement::GenerateCode(Scope& scope){
  return SynthMIPS{
    "t0",
    "Here's an Expr Statement\n"
  };
}

SynthMIPS AddExpr::GenerateCode(Scope& scope){
  return SynthMIPS{
    "t0",
    "Here's an AddExpr\n"
  };
}

SynthMIPS SubExpr::GenerateCode(Scope& scope){
  return SynthMIPS{
    "t0",
    "Here's an SubExpr\n"
  };
}

SynthMIPS MultExpr::GenerateCode(Scope& scope){
  return SynthMIPS{
    "t0",
    "Here's an MultExpr\n"
  };
}

SynthMIPS DivExpr::GenerateCode(Scope& scope){
  return SynthMIPS{
    "t0",
    "Here's an DivExpr\n"
  };
}
