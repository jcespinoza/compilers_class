# Compilers II class exercises files

Exercise files for the Compilers II class CCC502, 2017 Quarter 3

### Week One
- Learning to use FLEX
- Writing an arithmetic expressions lexicon tokenizer
- Writing Make files
- Writing the expressions grammar and parser
- Compiling and linking with GCC
- Learning to not freak out with the compiler's complains

## Week Two
- Fix bug which causes includes to accept anything after the filename
- Support double quotes for included files and also <> symbols
- Support format for expression printing: decimal, hexadecimal or binary:

```
Usage: print <expr> [dec | hex | bin] 32 bit integers
```

```c
{
  if(cInput == end_symbol){
    foundEnd = 1;
  }
  if(foundEnd && cInput != cBlank){
    fprintf(stderr, "Line %d: Invalid characters after filename\n", yylineno);
    return 0;
  }
  if(cInput != cBlank && cInput != end_symbol){
    foundEnd = 1;
  }else if(cInput == cBlank){

  }
}
```

- Fix problem with mixed-syntax variable assignnments


## Week Four
- Add support for if and else keywords
- Enhance AST with If statement
- Resolve the dangling else problem

## Week Five
- Add support for while loop statements
- Modify the project to use Lemon instead of Bison
