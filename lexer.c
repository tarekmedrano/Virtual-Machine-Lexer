// Compiler Builder 15
// Austin Dragone
// Gabriela Fisher
// Jerasimos Strakosha
// Tarek Medrano
// Jonathan Lacombe

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
	readsym , elsesym } token_type;

union lval {
	char *id;
	int num; 
} lval; 

char *tokens;
int t_pointer;
char *input;
int pointer;
int *input_guide;

token_type lex();
int startComment();
void tokenSym( token_type t );
void inputChar( char c );
int wordPrint( /*char c,*/ int p, int *ill );
void inputGuider( int i );


//Lex the input and output it before comments are removed
token_type lex() {
	
	char c, d;
	int saw_comment;
	
	//ignore space, tab, newline
	while ((c=getchar()) == ' ' || c == '\t' || c == '\n') {
		
		inputGuider( 0 );
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
	
	//Check for comment start
	saw_comment = 0;
	if( c == '/' ) {
		if( (d = getchar()) == '*' ) {
			saw_comment = 1;
			if( !startComment() ) {
				return nulsym; //EOF while in comment
			}
			c = getchar();
		} else
			ungetc(d, stdin);
	}
	
	//Check for white space again after comment 
	if( saw_comment ) {
		ungetc(c, stdin);
		while ( (c=getchar()) == ' ' || c == '\t' || c == '\n') {
			
			inputGuider( 0 );
			if( c == ' ' ) {
				printf(" ");
				input[pointer] = c;
				pointer++;
			}
			else if( c == '\t' ) {
				printf("\t");
				input[pointer] = c;
				pointer++;
			}
			else if( c == '\n' ) {
				printf("\n");
				input[pointer] = c;
				pointer++;
			}
		}
	}
	
	//End of the file, stop lexxing
	if (c == EOF) return nulsym;

	// Identifier or reserved word
	if (isalpha(c)) {
		
		char sbuf[100], *p = sbuf;
		
		do {
			
			printf("%c", c);
			inputGuider( 3 );
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
		
		return identsym;
	}
	
	// Digits
	if (isdigit(c)){
  		
		char sbuf[100], *p = sbuf;

		do {
			
			printf("%c", c);
			inputGuider( 3 );
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
		 inputGuider( 1 );
		 input[pointer] = '+';
		 pointer++;
		 
		 return plussym;
			
		case '*' :
		 inputGuider( 1 );
		 input[pointer] = '*';
		 pointer++;
		 
		 return multsym;
			
		case '(' :
		 inputGuider( 1 );
		 input[pointer] = '(';
		 pointer++;
		 
		 return lparentsym;
			
		case ')' :
		 inputGuider( 1 );
		 input[pointer] = ')';
		 pointer++;
		 
		 return rparentsym;
		
		// Completed these operator tokens
		// Tarek Medrano  
			
		case '/' :
		 inputGuider( 1 );
		 input[pointer] = '/';
		 pointer++;
		 
		 return slashsym;
			
		case '-' :
		 inputGuider( 1 );
		 input[pointer] = '-';
		 pointer++;
		 
		 return minussym;
			
		case ',' :
		 inputGuider( 1 );
		 input[pointer] = ',';
		 pointer++;
		 
		 return commasym;
			
		case ';' :
		 inputGuider( 1 );
		 input[pointer] = ';';
		 pointer++;
	
		 return semicolonsym;
			
		case '.' :
		 inputGuider( 1 );
		 input[pointer] = '.';
		 pointer++;
		 
		 return periodsym;
			
		case '<' :
		 inputGuider( 2 );
		 input[pointer] = '<';
		 pointer++;
		 
		 if((c=getchar()) == '=') {
			 printf("%c", c);
			 inputGuider( 2 );
			 input[pointer] = '=';
			 pointer++;
			 
			 return leqsym;
		 }
		 
		 //Undo 22 in inputGuide to 10
		 pointer -= 2; inputGuider( 1 );
		 pointer += 1; inputGuider( 0 );
		 
		 ungetc(c, stdin);
		 return lessym;     	    	
			
		case '>' :
		 inputGuider( 2 );
		 input[pointer] = '>';
		 pointer++;
		 
		 if((c=getchar()) == '=') {
			 printf("%c", c);
			 inputGuider( 2 );
			 input[pointer] = '=';
			 pointer++;
			 
			 return geqsym;
		 }
		 //Undo 22 in inputGuide to 10
		 pointer -= 2; inputGuider( 1 );
		 pointer += 1; inputGuider( 0 );
		 
		 ungetc(c, stdin);
		 return gtrsym;
			
		case '!' :
		 inputGuider( 2 );
		 input[pointer] = '!';
		 pointer++;
		 
		 if((c=getchar()) == '=')  {
			 printf("%c", c);
			 inputGuider( 2 );
			 input[pointer] = '=';
			 pointer++;
			 
			 return neqsym;
		 }
		 //Undo 22 in inputGuide to 40
		 pointer -= 2; inputGuider( 4 );
		 pointer += 1; inputGuider( 0 );
		 
		 ungetc(c, stdin);
		 //printf("illegal token \n");
		 break;
		  
		case ':' :
		 inputGuider( 2 );
		 input[pointer] = ':';
		 pointer++;
		 
		 if((c=getchar()) == '=') {
			 printf("%c", c);
			 inputGuider( 2 );
			 input[pointer] = '=';
			 pointer++;
			 
			 return becomessym;
		 }
		 //Undo 22 in inputGuide to 40
		 pointer -= 2; inputGuider( 4 );
		 pointer += 1; inputGuider( 0 );
		 
		 ungetc(c, stdin);
		 //printf("illegal token\n");
		 break; 		  
				
		case '=' :
		 inputGuider( 1 );
		 input[pointer] = '=';
		 pointer++;
		 
		 return eqsym;
		 
		 
		 
		default  :
			inputGuider( 4 );
			input[pointer] = c;
			pointer++;
			//printf("illegal token\n");
	}
}


//Add white space to the input string to maintain spacing
//Exit if we see */
//-Austin
int startComment(){
	
	char c, d;
	
	//To make up for the start of the comment /* 
	printf("/"); input[pointer] = ' '; pointer++;
	printf("*"); input[pointer] = ' '; pointer++;
	
	//Continue unless EOF or if we find */
	
	while( (c=getchar()) != EOF ) {
		
		//Exit comment if we find */
		if( c  == '*' ) {
			if( (d=getchar()) == '/' ) {
				printf("*"); input[pointer] = ' '; pointer++;
				printf("/"); input[pointer] = ' '; pointer++;
				return 1;
			} else {
				ungetc(d, stdin);
			}
		}
		
		//Tab is different since it takes multiple spaces
		if( c == '\t' ) {
			printf("\t"); 
			input[pointer] = '\t';
			pointer++;
		}
		
		//I don't think we need this, but is here just incase
		else if( c == '\n' )
			printf("\n");
		
		//Otherwise, we just copy a space to input and continue
		else {
			printf("%c", c); 
			input[pointer] = ' ';
			pointer++;
		}
		
	}
	
	//Quit lex since EOF
	return 0;
}


//This occured a lot so I made a function for it to make things look cleaner
void tokenSym( token_type t ) {
	tokens[t_pointer] = t;
	t_pointer++;
}


//Like tokenSym, but for input[] and pointer
//Replaces the following two lines in lex():
//	input[pointer] = ___;
//	pointer++;
void inputChar( char c ){
	
	input[pointer] = c;
	pointer++;
	
}


// when printing out the tokens this decides which is a full word so 
// it will print out var instead v a r on different lines
// -------------
// We want this to print any input over again and then place the token type next to it.
// -Tarek
int wordPrint(/*char c,*/ int p, int *ill) {
	
	int j = p;
	int is_three = 0;
	
	// Skip white space
	while( input_guide[j] == 0 || input[j] == '\r' )
		j++;
	
	// Print a single char operator
	if( input_guide[j] == 1 ) {
		printf("%c\t\t", input[j] );
		return j + 1;
	}
	
	// Print a two-char operator
	else if( input_guide[j] == 2 ) {
		printf("%c%c\t\t", input[j], input[j+1] );
		return j + 2;
	}
	
	// Print an identifier or number
	if( input_guide[j] == 3 )
		is_three = 1;
	while( input_guide[j] == 3 && input_guide[j] != '\0' ) {
		printf("%c", input[j] );
		j++;
	}
	if( is_three ) {
		printf("\t\t");
		return j;
	}
	
	while( input[j] == '\n' )
		j++;
	
	// Print "illegal token"
	if( input_guide[j] == 4 ) {
		printf("illegal token %c\n", tokens[j] );
		*ill = 1;
		return j + 1;
	}
}


// i=0 skip, i=1 print single char, i=2 print 2 char, i=3 print string or identifier
// until i changes, i=4 illegal token
// Example:
//		input: var x, dog;
//		guide: 22202102221
void inputGuider( int i ) {
	input_guide[pointer] = i;
}
	


int main() {
  	int i = 0;
	char c;
	token_type tok;

	tokens = malloc( sizeof(char)*2500 );
	input = malloc( sizeof(char)*2500 );
	input_guide = malloc( sizeof(int)*2500 );
	t_pointer = 0;
	pointer = 0;

  	
	//Lex the input and print the input
  	printf("\nsource code:\n-----------\n");
	while ((tok=lex()) != nulsym) {
		
		tokenSym( tok );

	}
	printf("\n\n");
	input[pointer] = '\0';
	
	
	//PRINT WITHOUT COMMENTS
  	printf("source code without comments:\n-----------------------------\n");
	i = 0;
	c = input[0];
	while( c != '\0') {
		
		printf("%c", c);
		i++;
		c = input[i];
	}
	
	
	//PRINT TOKENS
	printf("\n\ntokens:\n-------\n");
	int j;
	i = 0;
	int illegal = 0;
	for( j = 0; j < pointer; /*intentionally blank*/ ) {
		
		j = wordPrint( j, &illegal );
		if( !illegal )
			printf("%-2d\n", tokens[i] );
		illegal = 0;
		i++;
	}

	free(input);
	free(tokens);
	free(input_guide);
	
	return 0;
}
