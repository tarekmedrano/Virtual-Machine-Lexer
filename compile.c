// Compiler Builder 15
// Austin Dragone
// Gabriela Fisher
// Jerasimos Strakosha
// Tarek Medrano
// Jonathan Lacombe

#include <stdio.h>
#include <stdlib.h>
#define MAX_CODE_LENGTH 500
#define MAX_SYMBOL_TABLE_SIZE 10000

typedef struct{
  char type[3]
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
  } symbol_t;

symbol_t symbol_table[MAX_SYMBOL_TABLE_SIZE];


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

FILE *Parserin;

int get_next_t();

void add_symbol(int k, char name[], int val, int addr);

void err(int n);

//Generates the PL/0 code line w/ the input fields.
void emit(int op, int r, int l, int m);

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

// Tarek
void term(){
	int mulop;
	factor();
	while(token == multsym || token == slashsym){
		mulop = token;
		get_next_t();
		factor();
		if(mulop == multsym)
			emit(OPR, 0, OPR_MUL); //multiplication
		else
			emit(OPR, 0, OPR_DIV); //division
		
		
	}
	
}


void factor(){
	
}


// Tarek
void expression(){
	int addop;
	if (token == plussym || token == minussym){
		addop = token;
		get_next_t();
		term();
		if(addop == minussym) emit(OPR, 0, OPR_NEG); // negate
	}
	else term();
	
	while (token == plussym || token == minussym){
		addop = token;
		get_next_t();
		term();
		if(addop == plussym)
			emit(OPR, 0, OPR_ADD); //addition
		else
			emit(OPR, 0, OPR_SUB); // subtraction
		
		
		
	}
	
}

int get_next_t(){
	
}


void err(int n){
	
}

// Tarek
void emit(int op, int r, int l, int m){
	if(cx > CODE_SIZE)
		error(25);
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
