
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

#include "lexer.h"

#define MAX_CODE_LENGTH 500
#define MAX_SYMBOL_TABLE_SIZE 10000

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
  char name[12];	// name up to 12 chars
  int val;			// number (ascii val)
  int level;		// L level 
  int addr;			// M address
} symbol;

// All our global Vars

symbol symbol_table[MAX_SYMBOL_TABLE_SIZE];
int symIndex= 0;
int symAddr = 3;
// keep track of symbols in symbol table?
int num_symbols = 0;

// Code array
code_struct code[MAX_CODE_LENGTH] ;
int cx = 0;
int curr_lvl = 0;

int tokenArray[MAX_CODE_LENGTH];
Token* current_token;
int tokenIndex = 0;
int tokenPtr = 0;

FILE* Parserin;

void get_next_t(); // turned this to a void, was int
void add_symbol(int k, char *name, int num, int level, int modifier);
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
char* tokenToString(int token);

// "I don't understand what parser is doing. The while loop is scanning integers from a file,
// but the file we read in is PL/0 code. Also it should start at program() not block()" -Austin
//
// our main function for the parser/code generator
// - Tarek
void Parser(char* inputFile, char* outputFile){
	int i, temp;
	 // not sure how to define files to take in variable
	 // file names, someone fix this please. -Tarek
	
	initalizeLexer(inputFile);
	
	FILE* output = fopen(outputFile, "w");
	
	program();
	
	// Generate PM/0 code
	for( i = 0; i < cx; i++ ) {
		fprintf(output, "%d %d %d\n", code[i].op, code[i].l, code[i].m );
	}
	
	//fclose(input);
	fclose(output);

}

// Austin
// Get token, call block(), end on period
void program(){
	
	get_next_t();
	block();
	if( current_token->type != periodsym )
		err(9); // raise error
}

// Austin
// Check for constant declaration, variable declaration, procedure, then do statement
void block(){
	
	// Constant declaration
	// EBSF: "const" <id> "=" <num> { "," <id> "=" <num> } ";"
	if( current_token->type == constsym ) {
		// Loop until no more constants declared (so until semicolon)
		// Must see identifier, equals sign, then a number
		while (1) {
			char* ident;
			int val;
			
			get_next_t();
			if( current_token->type != identsym ) err(4);
			strcpy(ident,current_token->string);
			
			get_next_t();
			if( current_token->type != eqsym ) err(3);
			
			get_next_t();
			if( current_token->type != numbersym ) err(2);
			
			val = atoi(current_token->string);
			add_symbol(1, ident, val, 0, 0);
			
			get_next_t();
			if( current_token->type != commasym ) break;
		}
		
		if( current_token->type != semicolonsym ) err(5);
		get_next_t();
	}
	
	// Variable declaration
	// EBSF: "var" <id> { "," <id> } ";"
	if( current_token->type == varsym ) {
		
		get_next_t();
		if( current_token->type != identsym ) err(4);
		
		add_symbol(2, current_token->string, 0, curr_lvl, 0);
		get_next_t();
		
		while( current_token->type == commasym ) {
			get_next_t();
			if( current_token->type != identsym ) err(4);
			
			add_symbol(2, current_token->string, 0, curr_lvl, 0);
			get_next_t();
		}
		
		emit( INC, 0, 4 + num_symbols );
		
		if( current_token->type != semicolonsym ) err(5);
		get_next_t();
	}
	
	// Procedure
	// EBSF: "proc" { <id> ";" <block> ";" }
	while( current_token->type == procsym ) {
		
		get_next_t();
		if( current_token->type != identsym ) err(4);
		get_next_t();
		if( current_token->type != semicolonsym ) err(5);
		get_next_t();
		
		emit( jmp, 0, 0 );
		
		int proc_start = cx;
		code_struct temp;
		temp.op = 0;
		temp.l = curr_lvl;
		temp.m = cx;
		
		block();
		
		// Go back and write the modifier
		temp.op = code[proc_start].op;
		code[proc_start] = temp;
		
		if( current_token->type != semicolonsym ) err(5);
		get_next_t();
	}
	
	// Call statement
	statement();
}

// jerasimos
void statement() {
	
	if(current_token->type == identsym){
		get_next_t();
		if( current_token->type != becomessym ) err(19) ; // raise error
		get_next_t();
		expression();
		return;
		
	} else if( current_token->type == callsym ) {
		get_next_t();
		if( current_token->type != identsym )err(14) ; // raise error
		get_next_t();
		return;
		
	} else if(current_token->type == beginsym){
		get_next_t();
		statement();
		while(current_token->type == semicolonsym) {
			get_next_t();
			statement();
		}
		if(current_token->type != endsym) err(27); // raise error
		get_next_t();
		return;
		
	} else if(current_token->type == ifsym ){
		get_next_t();
		condition();
		if(current_token->type != thensym) err(16); // raise error
		get_next_t();
		statement();
		//get_next_t(); //semi colon?
		if(current_token->type == elsesym) {
			get_next_t();
			statement();
		}
		return;
		
	} else if(current_token->type == whilesym) {
		get_next_t();
		condition();
		if( current_token->type != dosym ) err(18) ; // raise error
		get_next_t();
		statement();
		return;
		
	} else if(current_token->type == readsym ){
		get_next_t();
		if(	current_token->type != identsym) err(26); // raise error
		get_next_t();
		return;
		
	} else if(current_token->type == writesym){
		get_next_t();
		if(	current_token->type != identsym) err(26); // raise error
		get_next_t();
		return;
	} else {
		
	}
}

// jerasimos 
void condition(){
	if(	current_token->type == oddsym){
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
	
	switch(current_token->type){

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
	while(current_token->type == multsym || current_token->type == slashsym){
		mulop = current_token->type;
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
	if(current_token->type == identsym){
		
		get_next_t();
		
		// use this to fetch the value of the identifier from the 
		// symbol table
		for(i = 0; i< num_symbols; i++){
			if(symbol_table[i].name == current_token->string){
				
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
		
		//get_next_t();
	}
	
	// otherwise if we find a numbersym, emit the number literal
	else if(current_token->type == numbersym){
		get_next_t();
		emit(lit, 0, current_token->type);
		//get_next_t();
	}
	
	
	// or if we find a left paren, evaluate it as a new expression
	else if(current_token->type == lparentsym){
		get_next_t();
		expression(); // possible edit after experssion() finished
		
		if(current_token->type != rparentsym){
			err(13);
		}
		
		//get_next_t();	
	}
	
	// otherwise we have an error
	else{
		err(30);
	}
}


// jerasimos
// Added an emit here - Gabriela  
void expression(){
	int plusop;
	if(current_token->type == plussym || current_token->type == minussym)
		get_next_t();
	term();
	while(current_token->type == plussym || current_token->type == minussym){
		plusop = current_token->type;
		get_next_t();
		term();
		if(plusop == plussym)  
			emit(opr, 0, ADD); 
		else
			emit(opr, 0, SUB);
	}
}

// Gets the next token in the tokenArray and advances the pointer
void get_next_t() {
	current_token = lex();
	printf("%s \n",current_token->string);
}

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
			printf("Error: const, var, procedure, read, write must be followed by identifier.\n");
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
			
		case 26:
			printf("Error: read, write must be followed by identifier.\n");
			exit(0);
			
		case 27:
			printf("Error: end expected.\n");
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

// Add constants or variables to the symbol table (procedures not in tiny PL/0)
// If the symbol already exists, raise error
// jonathan
void add_symbol(int k, char *name, int num, int level, int modifier){
	int i = 0;
	for ( i=0; i<num_symbols; i++) {
		symbol s = symbol_table[i];
		
		// Check name and level
		if (strcmp(s.name,name) == 0) {
			if(s.level == curr_lvl) {
				printf("Symbol name already in use\n");
				exit(0);
			}
		}
	}
	
	symbol *s = malloc(sizeof(symbol));
	s->kind = k;
	strcpy(s->name,name);
	s->val = num;
	s->level = level;
	s->addr = modifier;

	symbol_table[num_symbols] = *s;
	num_symbols++;
}



// we can use a tostring method to make the tokens into the names needed for parser func
// -Tarek
char* tokenToString(int token){
    switch (token)
    {
        case (int) plussym:
            return "plussym";
            
        case (int) minussym:
            return "minussym";
            
        case (int) numbersym:
            return "numbersym"; 
            
        case (int) multsym:
            return "multsym";
            
        case (int) slashsym:
            return "slashsym";
            
        case (int) lessym:
            return "lesssym";
            
        case(int) eqsym:
            return "eqsym";
            
        case (int) gtrsym:
            return "gtrsym";
            
        case(int) geqsym:
            return "geqsym";
            
        case(int) leqsym:
            return "leqsym";
            
        case(int) becomessym:
            return "becomessym";
            
        case(int) neqsym:
            return "neqsym";
            
        case (int) lparentsym:
            return "lparentsym";
            
        case (int) rparentsym:
            return "rparentsym";
            
        case (int) commasym:
            return "commasym";
            
        case (int) semicolonsym:
            return "semicolonsym";
            
        case (int) periodsym:
            return "periodsym";
            
        case (int) varsym:
            return "varsym";
            
        case (int) beginsym:
            return "beginsym";
            
        case (int) identsym:
            return "identsym";
            
        case (int) endsym:
            return "endsym";
            
        case (int) ifsym:
            return "ifsym";
            
        case (int) thensym:
            return "thensym";
            
        case (int) whilesym:
            return "whilesym";
            
        case (int) dosym:
            return "dosym";
            
        case (int) callsym:
            return "callsym";
            
        case (int) constsym:
            return "constsym";
            
        case (int) elsesym:
            return "elsesym";
            
        case (int) procsym:
            return "procsym";
            
        case (int) readsym:
            return "readsym";
            
        case (int) writesym:
            return "writesym";
            
        case (int) nulsym:
            return "NULL";
            
        default:
            return "ERR";
    }
}

// Main
int main( int argc, char** argv ) {

	// Austin - "Working on this at the moment"
	
	argc--; *argv++; //dont need first argument (executable name)
	char* inputFile = *argv++;
	char* outputFile = *argv++;
	
	Parser(inputFile, outputFile);

}
