%{
// including the standard c libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <stdbool.h>

#include "y.tab.h"

// symbol table size
#define SYMTABLESIZE 6174

// line number currently getting parsed.
int line_number=1;

// assign identifier symbol to yylval.symbol. This can be found in union in 180101037.y file.
int assignIdentifier(){
    yylval.symbol = (char*)malloc(80*sizeof(char)); 
    strcpy(yylval.symbol,yytext);
    return(identifier);
}
%}

SIGN [+\-]
INT_VALUE {SIGN}?([1-9][0-9]*|0)
REAL_NUMBER {}+\.{}+
digit	[0-9]
number	(-)?{digit}+({digit}+)?(E[+\-]?{digit}+)?

%%
 /* keywords */
[P][R][O][G][R][A][M]       return	(PROGRAM);
[V][A][R]                    return	(VAR);
[B][E][G][I][N]             return	(BEGINN);
[E][N][D]                  return	(END);
[E][N][D][\.]                return	(ENDL);
[I][N][T][E][G][E][R]        {yylval.expressionValueType = 1;return	(INTEGER);}
[R][E][A][L]                {yylval.expressionValueType = 2;return    (REAL);}
[F][O][R]				 return	(FOR);
[R][E][A][D]				 return	(READ);
[W][R][I][T][E]			 return	(WRITE);
[T][O]                       return	(TO);
[D][O]                      return	(DO);
[D][I][V]					 return	(DIV);
[p][r][o][g][r][a][m]       return	(PROGRAM);
[v][a][r]                    return	(VAR);
[b][e][g][i][n]             return	(BEGINN);
[e][n][d]                  return	(END);
[e][n][d][\.]                return	(ENDL);
[i][n][t][e][g][e][r]        {yylval.expressionValueType = 1;return	(INTEGER);}
[r][e][a][l]                {yylval.expressionValueType = 2;return    (REAL);}
[f][o][r]				 return	(FOR);
[r][e][a][d]				 return	(READ);
[w][r][i][t][e]			 return	(WRITE);
[t][o]                       return	(TO);
[d][o]                      return	(DO);
[d][i][v]					 return	(DIV);

    /* identifier */
[a-zA-Z]([a-zA-Z0-9_])*      return assignIdentifier();

    /* symbols */
":="                         return(ASSIGN);
":"                          return(COLON);
","                          return(COMMA);
"("                          return(OBRACKET);
"-"                          return(MINUS);
"+"                          return(PLUS);
"*"							 return(MULTIPLY);					
")"                          return(CBRACKET);
";"                          return(SEMICOLON);

    /* numbers */
{INT_VALUE}    					 { yylval.expressionValueType = 1; // assigning 1 as identifier type
                               return (INT_VALUE);}
{number}"."{number}          { yylval.expressionValueType = 2; // assigning 2 as identifier type
                               return (REAL_NUMBER);}

[ \t\f]                      {;}   
\n                           {line_number++;}


%%

// function to execute multiple files simultaneously
int yywrap(void){
    return 1;
}
