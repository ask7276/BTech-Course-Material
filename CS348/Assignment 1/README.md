## Author: Anket Kotkar
## Roll Number: 180101037

## Assignment 1

In this assignment, I have implemented 2 pass assembler with following instructions set in **Linux g++ enviroment** :
Instruction set: LDA, LDX, LDL,STA, STX, STL, LDCH, STCH, ADD, SUB, MUL, DIV, COMP, J, JLT, JEQ, JGT, JSUB, RSUB, TIX, TD, RD, WD, RESW, RESB, WORD, BYTE.

## Execution of the program:

**Enviroment details:**

1. Linux g++ enviroment
2. gcc version 9.3.0

**Instructions to be executed are as follow:**

First navigate to the directory 180101037 in terminal and then execute following instructions.

    $g++ -w 180101037_Assign01.cpp
    $./a.out < input_file_path

    For example:
    $g++ -w 180101037_Assign01.cpp
    $./a.out < input.txt

input_file_path should be replaced by the sample input file path.

Output would be written in the file output.txt. 

## Assumptions for this Assignment:

1. For any instruction other than those in Instruction set, program will stop the execution printing "Invalid Instruction in input" and exit. For the case of label not found or duplicate label found, program will stop the execution printing "Invalid input" and exit. This is done as input is in wrong format.

2. Name of the program is of maximum 6 characters. In the reference material provided, it was mentioned that 2nd to 7th character of the header record notes the program name. So I have considered that program name will be of maximum 6 characters.

3. Whenever there is continous address range break due to "RESW" or "RESB", new text record is started.
