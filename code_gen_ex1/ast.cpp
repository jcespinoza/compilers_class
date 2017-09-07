#include <sstream>
#include "ast.h"

string Root::GenerateCode(Scope& scope){
  return statementList->GenerateCode(scope).Code;
}

void Scope::releaseRegister(string name){
  registers[name] = 0;
}

string Scope::getFreeRegister(){
  map<string, bool>::iterator it = registers.begin();
  for(; it != registers.end(); it++){
    if(it->second == false){
      it->second = true;
      return it->first;
    }
  }
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
  stringstream stream;
  string place = scope.getFreeRegister();
  stream << "addi " << place <<", " << "$zero, " <<  value << " \n";

  return SynthMIPS{
    place,
    stream.str()
  };
}

SynthMIPS IdExpr::GenerateCode(Scope& scope){
  stringstream stream;
  string place = scope.getFreeRegister();
  stream << "lw " << place <<", " << varName << " \n";
  return SynthMIPS{
    place,
    stream.str()
  };
}

SynthMIPS ExprStatement::GenerateCode(Scope& scope){
  SynthMIPS expSynth = expression->GenerateCode(scope);
  scope.releaseRegister(expSynth.Place);

  return SynthMIPS{
    expSynth.Place,
    expSynth.Code
  };
}

SynthMIPS AddExpr::GenerateCode(Scope& scope){
  stringstream stream;
  SynthMIPS leftSynth = LeftSide->GenerateCode(scope);
  SynthMIPS rightSynth = RightSide->GenerateCode(scope);
  stream << leftSynth.Code;
  stream << rightSynth.Code;

  string lPlace = leftSynth.Place;
  string rPlace = rightSynth.Place;
  scope.releaseRegister(lPlace);
  scope.releaseRegister(rPlace);

  string rTarget = scope.getFreeRegister();

  stream << "add " << rTarget << ", " << lPlace << ", " <<  rPlace << " \n";

  return SynthMIPS{
    rTarget,
    stream.str()
  };
}

SynthMIPS SubExpr::GenerateCode(Scope& scope){
  stringstream stream;
  SynthMIPS leftSynth = LeftSide->GenerateCode(scope);
  SynthMIPS rightSynth = RightSide->GenerateCode(scope);
  stream << leftSynth.Code;
  stream << rightSynth.Code;

  string lPlace = leftSynth.Place;
  string rPlace = rightSynth.Place;
  scope.releaseRegister(lPlace);
  scope.releaseRegister(rPlace);

  string rTarget = scope.getFreeRegister();

  stream << "sub " << rTarget << ", " << lPlace << ", " <<  rPlace << " \n";

  return SynthMIPS{
    rTarget,
    stream.str()
  };
}

SynthMIPS MultExpr::GenerateCode(Scope& scope){
  stringstream stream;
  SynthMIPS leftSynth = LeftSide->GenerateCode(scope);
  SynthMIPS rightSynth = RightSide->GenerateCode(scope);
  stream << leftSynth.Code;
  stream << rightSynth.Code;

  string lPlace = leftSynth.Place;
  string rPlace = rightSynth.Place;
  scope.releaseRegister(lPlace);
  scope.releaseRegister(rPlace);

  stream << "mult " << lPlace << ", " <<  rPlace << " \n";
  string rTarget = scope.getFreeRegister();
  stream << "mflo " << rTarget << "\n";

  return SynthMIPS{
    rTarget,
    stream.str()
  };
}

SynthMIPS DivExpr::GenerateCode(Scope& scope){
  stringstream stream;
  SynthMIPS leftSynth = LeftSide->GenerateCode(scope);
  SynthMIPS rightSynth = RightSide->GenerateCode(scope);
  stream << leftSynth.Code;
  stream << rightSynth.Code;

  string lPlace = leftSynth.Place;
  string rPlace = rightSynth.Place;
  scope.releaseRegister(lPlace);
  scope.releaseRegister(rPlace);

  stream << "div " << lPlace << ", " <<  rPlace << " \n";
  string rTarget = scope.getFreeRegister();
  stream << "mflo " << rTarget << "\n";

  return SynthMIPS{
    rTarget,
    stream.str()
  };
}
