// Compiler Builder 15
// Austin Dragone
// Gabriela Fisher
// Jerasimos Strakosha
// Tarek Medrano
// Jonathan Lacombe

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_CODE_LENGTH 500
#define MAX_SYMBOL_TABLE_SIZE 10000


typedef struct{
  char type[3];
  char val[12];
  } token;
  
typedef struct{
  int op;
  int l;
  int m;
  }code_struct;
  
typedef struct{
  int kind;			// const = 1, var = 2, proc = 3
  char name[13];	//name up to 12 chars
  int val;			// number (ascii val)
  int level;		// L level 
  int addr;			// M address
  } symbol;

// All our global Vars

symbol symbol_table[MAX_SYMBOL_TABLE_SIZE];
int symIndex=0;
int symAddr = 3;


// Code array
code_struct code[MAX_CODE_LENGTH] ;
int cx = 0;
int curr_lvl = 0;

int tokenArray[MAX_CODE_LENGTH];
int current_token;
int tokenIndex =0; int tokenPtr =0;

// keep track of symbols in symbol table?
int symbol_cnt;

FILE* Parserin;

// Function prototypes

int get_next_t();

void add_symbol(int k, char name[], int val, int addr);

void err(int n);

//Generates the PL/0 code line w/ the input fields.
void emit(int op, int l, int m);

//procs for the grammar
void program();
void statement();
void block();
void condition();
int rel_op();
void term();
void factor();
void expression();

// our main function for the parser/code generator
void Parser(int flag){
	
}

void program(){
	
}

void block(){
	
}

void statement(){
	
}

void condition(){
	
}


int rel_op(){
	
	
	
}


void term(){

	
}


void factor(){
	
}



void expression(){

		
	}
	

int get_next_t(){
	
}


void err(int n){
	
}

// Tarek
void emit(int op, int l, int m){
	
	// send an error saying too large
	if(cx > MAX_CODE_LENGTH)
		err(25);
	else
	{
		code[cx].op = op; 	// opcode
		code[cx].l = l;		// lexicographical lvl 
		code[cx].m = m;		// modifier
		cx++;
	}
	
}

void add_symbol(int k, char name[], int val, int addr){
	
}
