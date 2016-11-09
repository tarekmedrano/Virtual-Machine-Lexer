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



enum token{nulsym = 1, identsym = 2, numbersym = 3, plussym = 4, minussym = 5, multsym = 6,
slashsym = 7, oddsym = 8, eqlsym = 9, neqsym = 10, lessym = 11, leqsym = 12,
gtrsym = 13, geqsym = 14, lparentsym = 15, rparentsym = 16, commasym = 17,
semicolonsym = 18, periodsym = 19, becomessym = 20, beginsym = 21, endsym = 22,
ifsym = 23, thensym = 24, whilesym = 25, dosym = 26, callsym = 27, constsym = 28,
intsym = 29, procsym = 30, outsym = 31, insym = 32, elsesym = 33};





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
int symIndex= 0;
int symAddr = 3;
// keep track of symbols in symbol table?
int symbol_cnt;

// Code array
code_struct code[MAX_CODE_LENGTH] ;
int cx = 0;
int curr_lvl = 0;

int tokenArray[MAX_CODE_LENGTH];
int current_token;
int tokenIndex = 0;
int tokenPtr = 0;



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

// Austin
// Get token, call block(), end on period
void program(){
	
	get_next_t();
	block();
	if( current_token != periodsym ) ; // raise error
}

// Austin
// Check for constant declaration, variable declaration, procedure, then do statement
// Note: still needs errors
void block(){
	
	// Constant declaration
	// EBSF: const id equals num { comma id = num } semicolon
	if( current_token == constsym ) {
		
		// Loop until no more constants declared (when we see a comma)
		// Must see identifier, equals sign, then a number
		while( current_token == commasym ) {
			
			get_next_t();
			if( current_token != identsym ) ; // raise error
			get_next_t();
			
			if( current_token != eqlsym ) ; // raise error
			get_next_t();
			
			if( current_token != identsym ) ; // raise error
			get_next_t();
		}
		
		if( current_token != semicolonsym ) ; // raise error
		get_next_t();
	}
	
	// Variable declaration
	// EBSF: var id { comma id } semicolon
	if( current_token == varsym ) {
		
		// 
		while( current_token == commasym ) {
			get_next_t();
			if( current_token != identsym ) ; // raise error
			get_next_t();
		}
		
		if( current_token != semicolonsym ) ; // raise error
		get_next_t();
	}
	
	// Procedure (Note: unused in tiny PL/0)
	// EBSF: proc { identifier semicolon block semicolon }
	while( current_token == procsym ) {
		
		get_next_t();
		if( current_token != identsym ) ; // raise error
		get_next_t();
		if( current_token != semicolonsym ) ; // raise error
		get_next_t();
		
		block();
		
		if( current_token != semicolonsym ) ; // raise error
		get_next_t();
	}
	
	// Call statement
	statement();
}

void statement(){
	
}

void condition(){
	
}


int rel_op(){
	
	
	
}


void term(){

	
}

// Tarek
void factor(){
	int i;
	if(current_token == identsym){
		
		get_next_t();
		
		for(i = 0; i< symbol_cnt; i++){
			if(symbol_table[i].name == token){
				
				if(symbol_table[i].kind == 1){
					emit(lit, 0, symbol_table[i].val);
					break;
				}
				
				else{
					emit(lod, curr_lvl-symbol_table[i].level, symbol_table[j].addr);
					break;
				}		
			}	
		}
		
		get_next_t();
	}
	
	
	else if(current_token == numbersym){
		get_next_t();
		emit(lit, 0, current_token);
		token = get_next_t();
	}
	
	else if(current_token == lparentsym){
		get_next_t();
		current_token = expression(); // possible edit after experssion() finished
		if(current_token != rparentsym){
			
			// fill this in with an error?
		}
		
		get_next_t();
		
	}
	
	else{
		
		//print out an invalid factor error
	}
}



void expression(){
	
}
	
// "Current_token was already a global so I changed made a few adjustments
// to this and to factor()" - Austin
// 
// Gets the next token in the tokenArray and advances the pointer
void get_next_t() {
	current_token = tokenArray[tokenPtr];
	tokenPtr ++;
}

// Fill this function with all the different errors we can get
// use switches preferably?
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
