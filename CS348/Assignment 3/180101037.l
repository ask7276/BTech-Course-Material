%{
// including the standard c libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <stdbool.h>

// defining token names and their values for latter use in simplified form as provided in assignment problem statement.
#define PROG_NAME                  1
#define VARIABLE                   2
#define START                      3
#define END_WITHOUT_DOT            4
#define END_WITH_DOT               5
#define MY_INTEGER                 6
#define MY_FOR                     7
#define MY_READ                    8
#define MY_WRITE                   9
#define KEYWORD_TO                10
#define KEYWORD_DO                11
#define SEMI_COLON_SYMBOL         12
#define COLON_SYMBOL              13
#define COMMA_SYMBOL              14
#define ASSIGNMENT_SYMBOL         15
#define PLUS_SYMBOL               16
#define SUB_SYMBOL                17
#define MUL_SYMBOL                18
#define MY_DIV                    19
#define OPEN_BRACKET_SYMBOL       20
#define CLOSE_BRACKET_SYMBOL      21
#define INDENTIFIERS              22
#define INTEGER_VALUES            23 

// value with respect to which hash table is generated
#define VALUE_HASH                31 

// special value of new line to return
#define NEW_LINE                  99

// line number currently getting parsed
int my_line_number = 1;

%}

%%

 /* Predefined keywords in Pascal in the scope of Assignment. */ 

"PROGRAM"                   return PROG_NAME;
"VAR"                       return VARIABLE;
"BEGIN"                     return START;
"END"                       return END_WITHOUT_DOT;
"END."                      return END_WITH_DOT;
"INTEGER"                   return MY_INTEGER;
"FOR"                       return MY_FOR;
"READ"                      return MY_READ;
"WRITE"                     return MY_WRITE;
"TO"                        return KEYWORD_TO;
"DO"                        return KEYWORD_DO;
"DIV"                       return MY_DIV;


 /* Symboles in Pascal in scope of Assignment 3. */

";"                         return SEMI_COLON_SYMBOL;
":"                         return COLON_SYMBOL;
","                         return COMMA_SYMBOL;
":="                        return ASSIGNMENT_SYMBOL;
"+"                         return PLUS_SYMBOL;
"-"                         return SUB_SYMBOL;
"*"                         return MUL_SYMBOL;
"("                         return OPEN_BRACKET_SYMBOL;
")"                         return CLOSE_BRACKET_SYMBOL;


 /* Defining the regular expressions for the identifiers and special characters. */

 /* regular expression for identifier. */
[_a-zA-Z][_a-zA-Z0-9]*      return INDENTIFIERS;

 /* regular expression for integer values. */
0                           return INTEGER_VALUES;
[1-9][0-9]*                 return INTEGER_VALUES;

 /* actions for special characters. */
[ \t]                       ;
"\n"                        my_line_number++;return NEW_LINE;
.                           printf("Syntactic Error\n"); return 1;

%%

/*****************************************************************************************************/

// Defining the symbol table struct to be used store the identifiers.
struct identifier
{
    int token_number;
    bool is_integer;
    bool is_variable;
    int integer_value;
    char *character_string_value;
    struct identifier *next;
};

// Hash table initialization of the symbol table
struct identifier *SymbolHashTable[VALUE_HASH];

/*
Functions related to query and insertion of identifiers in SymbolHashTable.
*/

// get the hash function value for the identifier using the custome defined hash function.
int getVariableHashValue(char *character_string_value)
{
    int temp = 0;
    int temp_iter = 0;
    while (temp_iter < strlen(character_string_value))
    {
        temp += character_string_value[temp_iter];
        temp_iter++;
    }
    return temp % VALUE_HASH;
}

// check if the identifier queried already exists in symbol table or not
bool queryVariableExistence(char *character_string_value)
{
    int hash_value = getVariableHashValue(character_string_value);
    struct identifier *temp_node = SymbolHashTable[hash_value];

    while (temp_node != NULL)
    {
        if (temp_node->is_variable && !strcmp(temp_node->character_string_value, character_string_value))
        {
            // object exists
            return true;
        }
        temp_node = temp_node->next;
    }
    // object does not exist
    return false;
}

// install id function which inserts identifiers or variable into symbol table.
void install_id(char *character_string_value)
{
    /*
    checking if the identifier has already been inserted in the symbol table and 
    skipping the insertion if it already exists.
    */
    if (queryVariableExistence(character_string_value))
    {
        return;
    }

    // creating new identifier object and assigning appropriate values.
    int hash_value = getVariableHashValue(character_string_value);
    struct identifier *temp_node = (struct identifier *)malloc(sizeof(struct identifier));

    temp_node->character_string_value = character_string_value;
    temp_node->is_variable = true;
    temp_node->token_number = 22;

    /*
    Getting the hash_value, hashed value corresponding to the object and then appending the object to the 
    SymbolHashTable[hash_value] chain. 
    */
    if (SymbolHashTable[hash_value] == NULL)
    {
        SymbolHashTable[hash_value] = temp_node;
    }
    else
    {
        temp_node->next = SymbolHashTable[hash_value];
        SymbolHashTable[hash_value] = temp_node;
    }
}

/*
Functions related to query and insertion of integers in SymbolHashTable.
*/

// get the hash function value for the integer using the custom defined hash function.
int getIntegerHashValue(int integer)
{
    return integer % VALUE_HASH;
}

// check if the integer queried already exists in symbol table or not
bool queryIntegerExistence(int integer)
{
    int hash_value = getIntegerHashValue(integer);
    struct identifier *temp_node = SymbolHashTable[hash_value];

    while (temp_node != NULL)
    {
        if (temp_node->is_integer && temp_node->integer_value == integer)
        {
            // object exists
            return true;
        }
        temp_node = temp_node->next;
    }
    // object does not exist
    return false;
}

// install num function which inserts integer into symbol table
void install_num(int integer)
{
    /*
    checking if the integer has already been inserted in the symbol table and 
    skipping the insertion if it already exists.
    */
    if (queryIntegerExistence(integer))
    {
        return;
    }

    // creating new identifier object and assigning appropriate values.
    int hash_value = getIntegerHashValue(integer);
    struct identifier *temp_node = (struct identifier *)malloc(sizeof(struct identifier));

    temp_node->integer_value = integer;
    temp_node->is_integer = true;
    temp_node->token_number = 23;

    /*
    Getting the hash_value, hashed value corresponding to the object and then appending the object to the 
    SymbolHashTable[hash_value] chain. 
    */
    if (SymbolHashTable[hash_value] == NULL)
    {
        SymbolHashTable[hash_value] = temp_node;
    }
    else
    {
        temp_node->next = SymbolHashTable[hash_value];
        SymbolHashTable[hash_value] = temp_node;
    }
}

/**********************************************************************************************/

// function needed to be defined if multiple files has to be compiled siultaneously in terminal.
int yywrap()
{
    return 1;
}

int main()
{
    // output.txt file in which output is printed.
    FILE *output;
    output = fopen("output.txt", "w");

    fprintf(output, "Line          Token Type             Token Specifier\n");
    fprintf(output, "----------------------------------------------------\n");

    // token value of the latest read token
    int token_value;
    token_value = yylex();

    // token string latest read
    char *token_text = "";

    // reading first token of the line in program.
    while (token_value != 0)
    {
        // first token of the line
        token_text = yytext;

        // if the token is identifier
        if (token_value == 22)
        {
            // inserting the identifier in the SymbolHashTable
            install_id(token_text);
            fprintf(output, "%-14d%-23d^%-14s\n", my_line_number, token_value, token_text);
        }
        // if the token is integer 
        else if (token_value == 23)
        {
            // inserting the integer in SymbolHashTable
            install_num(atoi(token_text));
            fprintf(output, "%-14d%-23d#%-14s\n", my_line_number, token_value, token_text);
        }
        // if the token is keyword or symbol
        else
        {
            fprintf(output, "%-14d%-23d               \n", my_line_number, token_value);
        }

        // now we read all the tokens in the line till we get the new line charactar
        token_value = yylex();

        while (token_value != NEW_LINE && token_value != 0)
        {
            // token read
            token_text = yytext;

            // if the token is identifier
            if (token_value == 22)
            {
                // inserting the identifier in the SymbolHashTable
                install_id(token_text);
                fprintf(output, "              %-23d^%-14s\n", token_value, token_text);
            }
            // if the token is integer 
            else if (token_value == 23)
            {
                // inserting the integer in SymbolHashTable
                install_num(atoi(token_text));
                fprintf(output, "              %-23d#%-14s\n", token_value, token_text);
            }
            // if the token is keyword or symbol
            else
            {
                fprintf(output, "              %-23d               \n", token_value, "");
            }
            token_value = yylex();
        }
        token_value = yylex();
    }
    fclose(output);
    return 0;
}