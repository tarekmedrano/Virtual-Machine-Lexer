// Compiler Builder 15
// Austin Dragone
// Gabriela Fisher
// Jerasimos Strakosha
// Tarek Medrano
// Jonathan Lacombe

#include <stdio.h>
#include <stdlib.h>
#define MAX_CODE_LENGTH 500


typedef struct{
  char type[3]
  char val[12];
  } token;
  
typedef struct{
  int op;
  int r;
  int l;
  int m;
  }code_struct;
  
typedef struct{
  int type;
  char name[12];
  int val;
  int level;
  int addr;
  } symbol;

// Code array
code_struct code[MAX_CODE_LENGTH] 

// Code index
int cx = 0;

//Stack pointer
int sp;

//Max lexicographical level
int lvl;

//Current Lexicographical level
int curr_lvl;

// Current lex level
token current_token;

// keep track of symbols in symbol table?
int symbol_cnt;
