
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

void get_next_t(); // turned this to a void, was int
void add_symbol(int k, char name[], int val, int addr);
void err(int n);

//Generates the PL/0 code line w/ the input fields.
void emit(int op, int l, int m);

//procs for the grammar
void program();
void statement();
void block();
void condition();
bool rel_op();
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
	// EBSF: "const" <id> "=" <num> { "," <id> "=" <num> } ";"
	if( current_token == constsym ) {
		
		// Loop until no more constants declared (so until semicolon)
		// Must see identifier, equals sign, then a number
		while( current_token == commasym ) {
			
			get_next_t();
			if( current_token != identsym ) ; // raise error
			get_next_t();
			
			if( current_token != eqlsym ) ; // raise error
			get_next_t();
			
			if( current_token != numbersym ) ; // raise error
			get_next_t();
		}
		
		if( current_token != semicolonsym ) ; // raise error
		get_next_t();
	}
	
	// Variable declaration
	// EBSF: "var" <id> { "," <id> } ";"
	if( current_token == varsym ) {
		
		while( current_token == commasym ) {
			get_next_t();
			if( current_token != identsym ) ; // raise error
			get_next_t();
		}
		
		if( current_token != semicolonsym ) ; // raise error
		get_next_t();
	}
	
	// Procedure (Note: unused in tiny PL/0)
	// EBSF: "proc" { <id> ";" <block> ";" }
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

// jerasimos
void statement(){
	
	if(current_token == identsym){
		get_next_t();
		if( current_token != becomessym ) ; // raise error
		get_next_t();
		expression();
		return;
		
	} else if( current_token == callsym ) {
		get_next_t();
		if( current_token != identsym ) ; // raise error
		get_next_t();
		return;
		
	} else if(current_token == beginsym){
		get_next_t();
		statement();
		while(current_token == semicolonsym) {
			get_next_t();
			statement();
		}
		if(current_token != endsym) ; // raise error
		get_next_t();
		return;
		
	} else if(current_token == ifsym ){
		get_next_t();
		condition();
		if(current_token != thensym) ; // raise error
		get_next_t();
		statement()
		return;
		
	} else if(current_token == whilesym) {
		get_next_t();
		condition();
		if( current_token != dosym ) ; // raise error
		get_next_t();
		statement();
		return;
		
	} else if(current_token == readsym ){
		get_next_t();
		if(	current_token != oddsym) ; // raise error
		get_next_t();
		return;
		
	} else if(current_token == writesym){
		get_next_t();
		if(	current_token != oddsym) ; // raise error
		get_next_t();
		return;
	} 
}

// jerasimos 
void condition(){
	if(	current_token == oddsym){
		get_next_t();
		expression();
		return;
	}else{
		expression();
		if( !rel_op() ) ; // error raised in rel_op()
		get_next_t();
		expression();
		return;
	}

}

// jerasimos
// Check that current_token is a relation symbol. If not, raise an error.
bool rel_op(){
	
	switch(current_token){

	case eqlsym : // '='
		return true;
	
	case neqsym : //'<>' "This is correct" - Austin
		return true;
	
	case lessym : // '<'
		return true;
	
	case leqsym : // '<='
		return true;

	case gtrsym : // '>'
		return true;
	
	case geqsym : // '>='
		return true;

	default :
		printf("error sysntax error, need Relational Operators");
		//error sysntax error, need Relational Operators
		// error msg and exit() code need here!!
	}
}

// jerasimos
void term(){
	int mulop;
	factor();
	while(current_token == multsym || current_token == slashsym){
		mulop = current_token;
		get_next_t();
		factor();
		if(mulop == multsym)
			emit(opr, 0, MUL); // correct me if im wrong but we needed to emit these?
								// -Tarek
		else
			emit(opr, 0, DIV);
	}
	
}

// Tarek
void factor(){
	int i;
	if(current_token == identsym){
		
		get_next_t();
		
		// use this to fetch the value of the identifier from the 
		// symbol table
		for(i = 0; i< symbol_cnt; i++){
			if(symbol_table[i].name == current_token){
				
				// if its a constant we emit a lit
				if(symbol_table[i].kind == 1){
					emit(lit, 0, symbol_table[i].val);
					break;
				}
				
				// otherwise we load the address of the factor and emit it in the virtual
				// mach.
				else{
					emit(lod, curr_lvl-symbol_table[i].level, symbol_table[j].addr);
					break;
				}		
			}	
		}
		
		get_next_t();
	}
	
	// otherwise if we find a numbersym, emit the number literal
	else if(current_token == numbersym){
		get_next_t();
		emit(lit, 0, current_token);
		get_next_t();
	}
	
	
	// or if we find a left paren, evaluate it as a new expression
	else if(current_token == lparentsym){
		get_next_t();
		expression(); // possible edit after experssion() finished
		
		if(current_token != rparentsym){
			err(13);
		}
		
		get_next_t();	
	}
	
	// otherwise we have an error
	else{
		//print out an invalid factor error

	}
}


// jerasimos
void expression(){
	if(current_token == plussym || current_token == minussym)
		get_next_t();
	term();
	while(current_token == plussym || current_token == minussym){
		get_next_t();
		term();
	}
}

// Gets the next token in the tokenArray and advances the pointer
void get_next_t() {
	current_token = tokenArray[tokenPtr];
	tokenPtr ++;
}

// Print an error statement out and exit the program.
// "Page 5 of the following link should include all of the errors we need" - Austin
// http://www.cs.ucf.edu/~wocjan/Teaching/2016_Fall/cop3402/2_homeworks/IntermediateCodeGeneration.pdf
// I'm filling in the errors that I see are possible, will need editing for usability after. - Gabriela
// Went ahead and finished up all 25 of the errors -Tarek
void err(int n){
	switch(n) {
		
		case 1:
			printf("Error: Use = instead of :=.\n");
			break;
			
		case 2:
			printf("Error: = must be followed by a number.\n");
			break;
			
		case 3: 
			printf("Error: Identifier should be followed by =");
			break;
			
		case 4:
			printf("Error: const, var, procedure must be followed by identifier.\n");
			break;
			
	    	case 5:
			printf("Error: Semicolon or comma missing.\n");
			break;
			
		case 6:
			printf("Error: Incorrect symbol after procedure declaration.");
			break;
			
	    	case 7:
			printf("Error: Statement expected");
			break;
			
	    	case 8:
			printf("Error: Incorrect symbol after statement part in block.\n");
			break;
			
		case 9:
			printf("Error: Period expected.\n");
			break;
			
	    	case 10:
			printf("Error: Semicolon between statements missing.\n");
			break;
			
		case 11:
			printf("Error: Undeclared identifier.\n");
			break;
			
		case 12:
			printf("Error: Assignment to constant or procedure is not allowed. \n");
			break;
			
	    	case 13:
	        	printf("Error: Assignment operater expected. \n");
		    	break;
			
		case 14:
		    	printf("Error: call must be followed by an identifier. \n");
			break;
			
		case 15:
			printf("Error: Call of a constant or variable is meaningless.\n");
			break;
			
		case 16:
			printf("Error: then expected \n");
			break;
			
		case 17:
			printf("Error: Semicolon or } expected.\n");
			break;
			
		case 18:
			printf("Error: do expected.\n");
			break;
			
		case 19:
			printf("Error: Incorrect symbol following statement\n");
			break;
			
		case 20:
			printf("Error: Relational operator expected.\n");
			break;
			
		case 21:
			printf("Error: Relational operator expected.\n");
			break;
			
		case 22:
			printf("Error: Right parenthesis missing. \n");
			break;
			
		case 23:
			printf("The preceding factor cannot begin with this symbol.\n");
			break;
			
		case 24:
			printf("An expression cannot begin with this symbol.\n");
			break;
			
		case 25:
			printf("This number is too large.\n");
			break;
	}
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
