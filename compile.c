
// Compiler Builder 15
// Austin Dragone
// Gabriela Fisher
// Jerasimos Strakosha
// Tarek Medrano
// Jonathan Lacombe

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#define MAX_CODE_LENGTH 500
#define MAX_SYMBOL_TABLE_SIZE 10000



typedef enum {  nulsym = 1, identsym, numbersym, plussym, minussym,
	multsym, slashsym, oddsym, eqsym, neqsym, lessym, leqsym,
	gtrsym, geqsym, lparentsym, rparentsym, commasym, semicolonsym,
	periodsym, becomessym, beginsym, endsym, ifsym, thensym,
	whilesym, dosym, callsym, constsym, varsym, procsym, writesym,
	readsym , elsesym } token_type;

//if we put the operations here we wont get any more errors when emitting
typedef enum
{
    hlt,
    lit,
    opr,
    lod,
    sto,
    cal,
    inc,
    jmp,
    jpc,
    sio
} opcode;

typedef enum
{
    RET,
    NEG,
    ADD,
    SUB,
    MUL,
    DIV,
    ODD,
    MOD,
    EQL,
    NEQ,
    LSS,
    LEQ,
    GTR,
    GEQ
} oprcode;


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
  int name;			//name up to 12 chars
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
			
			if( current_token != eqsym ) ; // raise error
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
		statement();
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

	case eqsym : // '='
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
		err(20);
		return false;
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
					emit(lod, curr_lvl-symbol_table[i].level, symbol_table[i].addr);
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
// Gabriela, Terek, Austin
void err(int n){
	switch(n) {
		
		case 1:
			printf("Error: Use = instead of :=.\n");
			exit(0);
			
		case 2:
			printf("Error: = must be followed by a number.\n");
			exit(0);
			
		case 3: 
			printf("Error: Identifier should be followed by =");
			exit(0);
			
		case 4:
			printf("Error: const, var, procedure must be followed by identifier.\n");
			exit(0);
			
	    case 5:
			printf("Error: Semicolon or comma missing.\n");
			exit(0);
			
		case 6:
			printf("Error: Incorrect symbol after procedure declaration.");
			exit(0);
			
	    case 7:
			printf("Error: Statement expected");
			exit(0);
			
	    case 8:
			printf("Error: Incorrect symbol after statement part in block.\n");
			exit(0);
			
		case 9:
			printf("Error: Period expected.\n");
			exit(0);
			
	    case 10:
			printf("Error: Semicolon between statements missing.\n");
			exit(0);
			
		case 11:
			printf("Error: Undeclared identifier.\n");
			exit(0);
			
		case 12:
			printf("Error: Assignment to constant or procedure is not allowed. \n");
			exit(0);
			
	    case 13:
	        printf("Error: Assignment operater expected. \n");
			exit(0);
			
		case 14:
		    printf("Error: call must be followed by an identifier. \n");
			exit(0);
		
		case 15:
			printf("Error: Call of a constant or variable is meaningless.\n");
			exit(0);
			
		case 16:
			printf("Error: then expected \n");
			exit(0);
		
		case 17:
			printf("Error: Semicolon or } expected.\n");
			exit(0);
		
		case 18:
			printf("Error: do expected.\n");
			exit(0);
			
		case 19:
			printf("Error: Incorrect symbol following statement\n");
			exit(0);
		
		case 20:
			printf("Error: Relational operator expected.\n");
			exit(0);
		
		case 21:
			printf("Error: Relational operator expected.\n");
			exit(0);
		
		case 22:
			printf("Error: Right parenthesis missing. \n");
			exit(0);
		
		case 23:
			printf("The preceding factor cannot begin with this symbol.\n");
			exit(0);
		
		case 24:
			printf("An expression cannot begin with this symbol.\n");
			exit(0);
		
		case 25:
			printf("This number is too large.\n");
			exit(0);
		
		default:
			printf("Unknown error.\n");
			exit(0);
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
