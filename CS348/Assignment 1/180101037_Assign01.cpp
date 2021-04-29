/*
    Name: Anket Sanjay Kotkar
    Roll Number: 180101037

    Enviroment details:
    1. Linux g++ enviroment
    2. gcc version 9.3.0

    Instructions to be executed:
    $g++ -w 180101037_Assign01.cpp
    $./a.out < input.txt
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

// including the user created .h files
#include "GenerateTable.h"
#include "ArithmeticOperations.h"
#include "Pass1.h"
#include "Pass2.h"

int main()
{
    generateOPTAB();
    pass_1();
    pass_2();
    return 0;
}