// Compiler Builder 15
// Austin Dragone
// Gabriela Fisher
// Jerasimos Strakosha
// Tarek Medrano
// Jonathan Lacombe


// 99% of this is taken from the example lexer, and we just need to add in the rest of the tokens
// and format it so that the output matches what is expected.

// nulsym, identsym, numbersym, plussym, multsym, lparentsym,
// rparentsym are completed already.
// our job is to split the other 26.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// List of the tokens we are responsible for handling
typedef enum {  nulsym = 1, identsym, numbersym, plussym, minussym,
	multsym, slashsym, oddsym, eqsym, neqsym, lessym, leqsym,
	gtrsym, geqsym, lparentsym, rparentsym, commasym, semicolonsym,
	periodsym, becomessym, beginsym, endsym, ifsym, thensym,
	whilesym, dosym, callsym, constsym, varsym, procsym, writesym,
	readsym , elsesym} token_type;

union lval {
	char *id;
	int num; 
} lval; 

char *tokens;
int t_pointer;
char *input;
int pointer;

token_type lex();
int startComment();
void tokenSym( token_type t );
void tokenIllegal();
void inputChar( char c );


//Lex the input and output it before comments are removed
token_type lex() {
	
	//Check for comment start
	char c, d;
	if( (c = getchar()) == '/' ) {
		if( (d = getchar()) == '*' ) {
			printf("\ncomment started\n");
			if( !startComment() ) {
				return nulsym; //EOF while in comment
			}
		} else {
		ungetc(d, stdin);
		ungetc(c, stdin);
		}
	} else {
		ungetc(c, stdin);
	}
	
	//ignore space, tab, newline
	while ((c=getchar()) == ' ' || c == '\t' || c == '\n') {
		
		if( c == ' ' ) {
			printf(" ");
			input[pointer] = c;
			pointer++;
		}
		else if( c == '\t' ) {
			printf("\t");
			input[pointer] = '\t';
			pointer++;
		}
		else if( c == '\n' ) {
			printf("\n");
			input[pointer] = c;
			pointer++;
		}
	}
	
	//End of the file, stop lexxing
	if (c == EOF) return nulsym;

	// identifier or reserved word
	if (isalpha(c)) {
		
		char sbuf[100], *p = sbuf;
		
		do {
			
    	printf("%c", c);
		input[pointer] = c;
		pointer++;
		*p++ = c;

		} while ((c=getchar()) != EOF && isalnum(c));

		ungetc(c, stdin);

		*p = '\0';

		// check if any of these identifiers were actually meant
		// to be reserved words instead. 
		// -Tarek Medrano

		if(strcmp(sbuf, "begin") == 0) return beginsym;

		if(strcmp(sbuf, "null") == 0) return nulsym;

		if(strcmp(sbuf, "call") == 0) return callsym;

		if(strcmp(sbuf, "const") == 0) return constsym;

		if(strcmp(sbuf, "do") == 0) return dosym;

		if(strcmp(sbuf, "else") == 0) return elsesym;

		if(strcmp(sbuf, "end") == 0) return endsym;

		if(strcmp(sbuf, "if") == 0) return ifsym;

		if(strcmp(sbuf, "odd") == 0) return oddsym;

		if(strcmp(sbuf, "procedure") == 0) return procsym;

		if(strcmp(sbuf, "read") == 0) return readsym;

		if(strcmp(sbuf, "then") == 0) return thensym;

		if(strcmp(sbuf, "var") == 0) return varsym;

		if(strcmp(sbuf, "while") == 0) return whilesym;

		if(strcmp(sbuf, "write") == 0) return writesym;

		lval.id = sbuf;
		
		return identsym;
	}
	
	// digits
	if (isdigit(c)){
  		
		char sbuf[100], *p = sbuf;

		do {
			
			printf("%c", c);
			input[pointer] = c;
			pointer++;
			*p++ = c;

		} while ((c=getchar()) != EOF && isdigit(c));

		ungetc(c, stdin);

		*p = '\0';
		
		return numbersym;  	
	}
	
	//Reserved symbol or illegal token
	printf("%c", c);
	switch (c) {
 	 	
    	case '+' :
		 input[pointer] = '+';
		 pointer++;
		 
		 return plussym;
     		
    	case '*' :
		 input[pointer] = '*';
		 pointer++;
		 
		 return multsym;
   	    	
    	case '(' :
		 input[pointer] = '(';
		 pointer++;
		 
		 return lparentsym;
      		
    	case ')' :
		 input[pointer] = ')';
		 pointer++;
		 
		 return rparentsym;
      	
		// Completed these operator tokens
		// Tarek Medrano  
		  	
    	case '/' :
		 input[pointer] = '/';
		 pointer++;
		 
		 return slashsym;
     		
    	case '-' :
		 input[pointer] = '-';
		 pointer++;
		 
    	 return minussym;
   	    	
    	case ',' :
		 input[pointer] = ',';
		 pointer++;
		 
		 return commasym;
      		
    	case ';' :
		 input[pointer] = ';';
		 pointer++;
		 
		 return semicolonsym;
      		
    	case '.' :
		 input[pointer] = '.';
		 pointer++;
		 
		 return periodsym;
     		
    	case '<' :
		 input[pointer] = '<';
		 pointer++;
		 
		 if((c=getchar()) == '=') {
			 printf("%c", c);
			 input[pointer] = '=';
			 pointer++;
			 
			 return leqsym;
		 }
		 ungetc(c, stdin);
		 return lessym;     	    	
   	    	
    	case '>' :
		 input[pointer] = '>';
		 pointer++;
		 
		 if((c=getchar()) == '=') {
			 printf("%c", c);
			 input[pointer] = '=';
			 pointer++;
			 
			 return geqsym;
		 }
		 ungetc(c, stdin);
		 
		 return gtrsym;
			
    	case '!' :
		 input[pointer] = '!';
		 pointer++;
		 
		 if((c=getchar()) == '=')  {
			 printf("%c", c);
			 input[pointer] = '=';
			 pointer++;
			 
			 return neqsym;
		 }
		 ungetc(c, stdin);
		 //printf("illegal token \n");
		 break;
		  
    	case ':' :
		 input[pointer] = ':';
		 pointer++;
		 
		 if((c=getchar()) == '=') {
			 printf("%c", c);
			 input[pointer] = '=';
			 pointer++;
			 
			 return becomessym;
		 }
		 ungetc(c, stdin);
		 //printf("illegal token \n");
		 break; 		  
				
    	case '=' :
		 input[pointer] = '=';
		 pointer++;
		 
		 return eqsym;
		 
		 
		 
		default  :
			input[pointer] = ' ';
			pointer++;
			//printf("illegal token\n");
	}
	
}


//Add white space to the input string to maintain spacing
//Exit if we see */
//-Austin
int startComment(){
	
	//To make up for the start of the comment /* 
	printf("/"); input[pointer] = ' '; pointer++;
	printf("*"); input[pointer] = ' '; pointer++;
	
	//Continue unless EOF
	char c, d;
	while( (c=getchar()) != EOF ) {
		
		//Exit comment if we find */
		if( c  == '*' ) {
			if( (d=getchar()) == '/' ) {
				printf("*"); input[pointer] = ' '; pointer++;
				printf("/"); input[pointer] = ' '; pointer++;
				return 1;
			}
		}
		ungetc(d, stdin);
		
		//Tab is different since it takes multiple spaces
		if( c == '\t' ) {
			printf("\t"); 
			input[pointer] == '\t';
			pointer++;
		}
		
		/*//I don't think we need this, but is here just incase
		else if( c == '\n' ) {
			input[pointer] = '\n';
			pointer++;
		}
		*/
		
		//Otherwise, we just copy a space to input and continue
		printf("%c", c); 
		input[pointer] = ' ';
		pointer++;
		
	}
	
	//Quit lex since EOF
	return 0;
}


//This occured a lot so I made a function for it to make things look cleaner
void tokenSym( token_type t ) {
	tokens[t_pointer] = (char) t;
	t_pointer++;
}


//Replace all "printf("illegal token\n");" in lex() with this function call 
void tokenIllegal() {
	
	//loop through the string "illegal token\n" and put each char in token[]
	//(including '\n') and increment t_pointer
	
}


//Like tokenSym, but for input[] and pointer
//Replaces the following two lines in lex():
//	input[pointer] = ___;
//	pointer++;
void inputChar( char c ){
	
	//put c in input[] and increment pointer variable
	
}


int main() {
  	
	tokens = malloc( sizeof(char)*2500 );
	input = malloc( sizeof(char)*2500 );
	t_pointer = 0;
	pointer = 0;
	token_type tok;
  	
	//Lex the input and print the input
  	printf("\nsource code:\n-----------\n");
	while ((tok=lex()) != nulsym) {
		
		tokenSym( tok );
		
		//Print tokens later
		tokens[t_pointer] = (char) tok;
		t_pointer++;
    
		if (tok == identsym) 
			; //printf(" %s", lval.id);
    
		tokens[t_pointer] = '\n';
		t_pointer++;
	}
	printf("\n\n");
	input[pointer] = '\0';
	
	
	//PRINT WITHOUT COMMENTS
  	printf("source code without comments:\n-----------------------------\n");
	int i = 0;
	char c = input[0];
	while( c != '\0') {
		
		printf("%c", c);
		i++;
		c = input[i];
	}
	
	
	//PRINT TOKENS
	//needs fixing, causes segmentation fault
	printf("\n\ntokens:\n-------\n");
	i = 0;
	/*c = tokens[0];
	while( c != '\0' ) {
		
		printf("%c", c);
		i++;
		c = tokens[i];
	}*/
	
}







