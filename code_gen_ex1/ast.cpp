#include <sstream>
#include "ast.h"

string Root::GenerateCode(Scope& scope){
  return statementList->GenerateCode(scope).Code;
}

SynthMIPS StatementList::GenerateCode(Scope& scope){
  list<Statement*>::iterator it = statements.begin();
  SynthMIPS synth;
  for(; it != statements.end(); it++){
    SynthMIPS synth = (*it)->GenerateCode(scope);
    cout << synth.Code;
  }
  return synth;
}

SynthMIPS NumExpr::GenerateCode(Scope& scope){
  stringstream stream;
  stream << "addi " << "$t0, " << "$zero, " <<  value << " \n";
  return SynthMIPS{
    "t0",
    stream.str()
  };
}

SynthMIPS IdExpr::GenerateCode(Scope& scope){
  stringstream stream;
  stream << "lw " << "$t0, " << varName << " \n";
  return SynthMIPS{
    "t0",
    stream.str()
  };
}

SynthMIPS ExprStatement::GenerateCode(Scope& scope){
  SynthMIPS expSynth = expression->GenerateCode(scope);
  return SynthMIPS{
    "t0",
    expSynth.Code
  };
}

SynthMIPS AddExpr::GenerateCode(Scope& scope){
  stringstream stream;
  SynthMIPS leftSynth = LeftSide->GenerateCode(scope);
  SynthMIPS rightSynth = RightSide->GenerateCode(scope);
  stream << leftSynth.Code;
  stream << rightSynth.Code;
  stream << "add " << "$t0" << ", " << "$t0, " <<  "$t1" << " \n";

  return SynthMIPS{
    "t0",
    stream.str()
  };
}

SynthMIPS SubExpr::GenerateCode(Scope& scope){
  stringstream stream;
  stream << "sub " << "$t1" << ", " << "$t0, " <<  "$t1" << " \n";
  return SynthMIPS{
    "t0",
    stream.str()
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
