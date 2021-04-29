%{                         
// including the standard c libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <stdbool.h>
        
// Symbol table size 
#define SYMTABLESIZE 6174 
// last index used to update the symboltypetable updation 
int last_used_index = 0;

// necessary declarations
int yylex(void);
void yyerror(char*msg);
extern char *yytext;
extern int line_number;

//custom data structure to store identifiers and numerical values. 1 for int and 2 for real values & -1 as default
struct entry{
	char *identifier_symbol;
	char *type;
    short val;
    struct entry* next;
};

//this table keeps track of all the identifiers and numerical values
struct entry symbolTable[SYMTABLESIZE]; 
int SymbolTableType[SYMTABLESIZE];


//hash function which takes the lexmes and gives a hash_value which is used to insert in hashtable
int gettingHashValue(char *sym,int offset)
{
    unsigned long hash_value = 2473;
    int c ;

    for(int temp=0;temp < strlen(sym);temp++){
        c = sym[temp];
		hash_value = ((hash_value * 193) + 841) * 211 + c;
		hash_value %= SYMTABLESIZE;
    }

    return ((hash_value+offset)%SYMTABLESIZE);
}

//searching in hashtable for any specific entry
int searchingHashTable(char *sym, int ifInt) {
	int hash_index =gettingHashValue(sym,0);
     
	//at max SYMTABLESIZE tries can be done if all tries are finished as there is no entry in hashtable

    if(sym != NULL){
        for(int offset=0;offset < SYMTABLESIZE; offset++){
            
            if(!(strcmp(symbolTable[hash_index].identifier_symbol,"NOTDEFINED"))){
                break;
            }
            
            if(!strcmp(symbolTable[hash_index].identifier_symbol,sym)) {
                return hash_index ;
            }
            hash_index = gettingHashValue(sym,offset);
        }
    }

	return -1;
}



//inserting in hash table if not present in hashtable
int hashTableInsertion(char *sym , char *type) {
	if(searchingHashTable(sym, 0)==-1) {
		int offset= -1;
		//at max SYMTABLESIZE tries can be done if all tries are finished then no entry is empty in hashtable
		
        do{
            ++offset;
            int hash_index=gettingHashValue(sym,offset);
            if(!strcmp(symbolTable[hash_index].identifier_symbol,"NOTDEFINED")) {

                // assigning values to the new node
                symbolTable[hash_index].type = (char *)malloc((strlen(type)+1)*sizeof(char));
                strncpy(symbolTable[hash_index].type, type, strlen(type));

				symbolTable[hash_index].val = -1;

                symbolTable[hash_index].identifier_symbol = (char *) malloc((strlen(sym)+1)*sizeof(char));
                strncpy(symbolTable[hash_index].identifier_symbol, sym, strlen(sym));

				return hash_index%SYMTABLESIZE;
			}
        } while(offset < SYMTABLESIZE);
	}
	return -1;
}

// check existence of identifier in symbol table
bool checkExistence(char *sym){
    int exist = searchingHashTable(sym, 0);
    if( exist != -1){
        return true;
    }
    return false;
}

// adding indentifier to the symboltable
void install_id(char *sym){
   
    if(!checkExistence(sym)){
        int temp;
	    temp = hashTableInsertion(sym,"identifier");
        SymbolTableType[last_used_index] = temp;
        last_used_index++;
    }
    else{
       yyerror("semantic error");
    }
}

// error function print out the syntax or seemantic errors
void yyerror(char* msg){
    fprintf(stderr,"%s at line : %d\n",msg,line_number);
}

// store type of identifier in corresponding of table SymbolTableType
void IdentifierTypeDeclaration(int type){
    int temp = last_used_index -1;
    int curr_index = SymbolTableType[temp];

    for(;temp >=0;){
        temp--;
        if(symbolTable[curr_index].val != -1){
            break;
        }
        symbolTable[curr_index].val = type;
        curr_index = SymbolTableType[temp];
    }
}

%}

%start prog

%union{
    int expressionValueType;
    char *symbol;
}

// keywords
%token PROGRAM 
%token VAR 
%token BEGINN 
%token END
%token ENDL
%token FOR 
%token READ 
%token WRITE
%token TO 
%token DO

// symbols
%left MULTIPLY DIV
%left PLUS MINUS

%token<symbol> identifier
%token<expressionValueType> INT_VALUE
%token<expressionValueType> REAL_NUMBER
%token<expressionValueType> INTEGER
%token<expressionValueType> REAL
%type<expressionValueType> factor
%type<expressionValueType> term
%type<expressionValueType> expression

// symbols
%token SEMICOLON 
%token COLON
%token COMMA
%token ASSIGN
%token OBRACKET
%token CBRACKET

// pascal grammer declaration
%%
prog :      PROGRAM prog_name VAR dec_list BEGINN statements ENDL
            ;
prog_name:  identifier   
            ;
dec_list:   declaration  {;}
            | dec_list COMMA declaration {;}
            ;
declaration:    idList COLON type     {;}
                ;
type:       INTEGER                {int int_type = $1; IdentifierTypeDeclaration(int_type);}        
            | REAL                 {int real_type = $1; IdentifierTypeDeclaration(real_type);}
            ;

for:        FOR indexexpression DO body {;}
          ;
indexexpression:    assign TO expression {;}
            ;
assign:     identifier ASSIGN expression    {   int hashTableIndex = searchingHashTable($1,0);
                                                // identifier not found
                                                if(hashTableIndex == -1){
                                                    yyerror("Semantic Error - variable not declared");
                                                }
                                                // identifier and expression type mis-match
                                                else if(symbolTable[hashTableIndex].val != $3){
                                                    yyerror("Semantic Error - different data types assigned to each other");
                                                };}       
            ;
idList:     idList COMMA identifier        {char *cur_token = yytext; install_id(cur_token); }
            |identifier                    {char *cur_token = yytext; install_id(cur_token);}
            ;

statements: statement {;}
            | statements SEMICOLON statement  {;}
            ;
statement:  assign {;}
            | read {;}
            | write {;}
            | for {;}
            ;

expression: term                    {$$ = $1;}
            | expression PLUS term  {int type1 = $1, type3 = $3;
                                    if(type1 != type3) {
                                        yyerror("Semantic Error - different data types assigned to each other");
                                    };
                                    $$ = type1;}
            | expression MINUS term {int type1 = $1, type3 = $3;
                                    if(type1 != type3) {
                                        yyerror("Semantic Error - different data types assigned to each other");
                                    };
                                    $$ = type1;}
            ;
term:       factor                  {$$ = $1;} 
            | term MULTIPLY factor  {int type1 = $1, type3 = $3;
                                    if(type1 != type3) {
                                        yyerror("Semantic Error - different data types assigned to each other");
                                    };
                                    $$ = type1;}
            | term DIV factor       {int type1 = $1, type3 = $3;
                                    if(type1 != type3) {
                                        yyerror("Semantic Error - different data types assigned to each other");
                                    };
                                    $$ = type1;}
            ;

identifier_list: identifier_list COMMA identifier      {if(searchingHashTable($3,0)==-1) {
                                                yyerror("semantic error - Variable not declared");
                                            }; }
            | identifier                    {if(searchingHashTable($1,0)==-1) {
                                                yyerror("semantic error - Variable not declared");
                                            }; }
          ;

factor:     identifier                    { int hashTableIndex = searchingHashTable($1,0); 
                                            if(hashTableIndex != -1){
                                                $$ = symbolTable[hashTableIndex].val;
                                            }
                                            else {
                                                yyerror("semantic error - Variable not declared");
                                            }; }       
            | INT_VALUE                 { $$ = $1;}            
            | REAL_NUMBER             { $$ = $1;}            
            | OBRACKET expression CBRACKET { $$ = $2;}
            ;
read:       READ OBRACKET identifier_list CBRACKET {;}
            ;
write:      WRITE OBRACKET identifier_list CBRACKET {;}
            ;
body:       BEGINN statements END
            | statement
            ;
          
%%

int main(int argc ,char*argv[]){
    
	//initializing the hashtable 
	for(int i=0;i<SYMTABLESIZE;i++){
        symbolTable[i].identifier_symbol = "NOTDEFINED";
	}
    yyparse();
    return 0;
}
