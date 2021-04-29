#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <bits/stdc++.h>

// including the user created .h files
#include "GenerateTable_Linking_Loader.h"
#include "ArithmeticOperations_Linking_Loader.h"
#include "Pass1_Linking_Loader.h"
#include "Pass2_Linking_Loader.h"


int main()
{
    cout<<"Please give the PROGADDR or starting address for the program\n";
    PROGADDR = (char *)malloc(sizeof(char) * 7);
    size_t s1 = 6;
    getline(&PROGADDR, &s1, stdin);
    PROGADDR[strlen(PROGADDR)-1] = '\0';
    PROGADDR = makeProperLength(PROGADDR, 6);
    fflush(stdin);

    cout<<"\nPlease enter the sample input file name\n";
    start_write_address = (char *)malloc(sizeof(char) * 7);
    for(int i=0;i<5;i++){
        start_write_address[i] = PROGADDR[i];
    }
    start_write_address[5] = '0';


    INPUT_FILE_NAME = (char *)malloc(sizeof(char) * 20);
    size_t s2 = 19;
    getline(&INPUT_FILE_NAME, &s2, stdin);
    fflush(stdin);

    Pass1();
    Pass2();
    return 0;
}
