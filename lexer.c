// Compiler Builder 15
// Austin Dragone
// Gabriela Fisher
// Jerasimos Strakosha
// Tarek Medrano
// JonaThan Lacombe


// 99% of this is taken from the example lexer, and we just need to add in the rest of the tokens
// and format it so that the output matches what is expected.

// nulsym, identsym, numbersym, plussym, multsym, lparentsym,
// rparentsym are completed already.
// our job is to split the other 26.


#include <stdio.h>
#include <stdlib.h>
#include <string.h>



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


token_type lex() {
	char c;
  
	// ignore space, tab, newline
	while ((c=getchar()) == ' ' || c== '\t' || c == '\n')
	;

	if (c == EOF) return nulsym;

	// identifier
	if (isalpha(c)) {
		char sbuf[100], *p = sbuf;
    	
		do {
 
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
  
	if (isdigit(c)){
  		
		char sbuf[100], *p = sbuf;
    	
		do {
		
		*p++ = c;

		} while ((c=getchar()) != EOF && isdigit(c));
    	
    	ungetc(c, stdin);
    	
    	*p = '\0';
    	
    	return numbersym;  	
	}

	switch (c) {
 	 	
    	case '+' :
		 return plussym;
     		
		case '*' :
		 return multsym;
   	    	
    	case '(' :
		 return lparentsym;
      		
    	case ')' :
		 return rparentsym;
      		
    	case '/' :
		 return slashsym;
     		
		case '-' :
		 return minussym;
   	    	
    	case ',' :
		 return commasym;
      		
    	case ';' :
		 return semicolonsym;
      		
    	case '.' :
		 return periodsym;
     		
		case '<' :
		 if((c=getchar()) == '=') return leqsym;
		 ungetc(c, stdin);
		 return lessym;     	    	
   	    	
    	case '>' :
		 if((c=getchar()) == '=') return geqsym;
		 ungetc(c, stdin);  
		 return gtrsym;
			
		case '!' :
		 if((c=getchar()) == '=') return neqsym;
		 ungetc(c, stdin);  
				
    	case '=' :
		 return eqsym;				  	      		

	
	  
    default  :
    	printf("illegal token\n");
  }
  
}

main() {
  	printf("example lexer\n");
  	
	token_type tok;
  	
	while ((tok=lex()) != nulsym) {
    
		printf("token type: %d", tok);
    
		if (tok == identsym) printf("%s", lval.id);
    
		printf("\n"); 
  }
}
