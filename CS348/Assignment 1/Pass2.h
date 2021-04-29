#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <stdbool.h>

// reading input from intermediate.txt and then deleting it. Writing output to output.txt.
FILE *output, *input;

char text_record[61];
char *start_address_for_record;

// make the 16th bit of instruction from right set if the indexed addressing is used.
char *makeAddressIndexed(char *adr)
{
    adr = addHexNum(adr, "8000");
    return adr;
}

// making the length of input string equal to desired length by joining 0 to start
char *makeProperLength(char *inp, int num)
{
    char *temp;
    temp = (char *)malloc(sizeof(char) * num + 1);
    int i = 0;
    while (i < num - strlen(inp))
    {
        temp[i] = '0';
        i++;
    }
    while (i < num)
    {
        temp[i] = inp[i - num + strlen(inp)];
        i++;
    }
    temp[i] = '\0';
    return temp;
}

// custom function to append 2 strings
char *strAppend(char *str1, char *str2)
{
    char *str;
    str = (char *)malloc(strlen(str1) + strlen(str2) + 1);
    int i = 0;
    int s = strlen(str1);
    while (i < s)
    {
        str[i] = str1[i];
        i++;
    }
    i = 0;
    while (i < strlen(str2))
    {
        str[i + s] = str2[i];
        i++;
    }
    str[i + s] = '\0';
    return str;
}

/*
Checking whether after appending the object code for the current size, will the record exceed its 
maximum allowed length. If it does so, write the text record to output file.

Whenever we come across a RESW or RESB instruction, there is line break in text record. These 
instructions don't get accounted in object code. So whenever these instructions appear, 
start_address_for_record is assigned empty string. Whenever at the start of loop, we encounter 
start_address_for_record is equal to "", we assign start_address_for_record the current instruction 
address. If these instruction is also RESW or RESB, start_address_for_record will again become "",
otherwise it will store address it is assigned, the address of first non RESW or RESB instruction 
after RESW or RESB instructions.
*/
void appendOrWriteLine(char *object_code, char *adr, int flag = 0)
{
    if (!flag)
    {
        if (strlen(text_record) + strlen(object_code) <= 60)
        {
            int s = strlen(text_record);

            if (s == 0)
            {
                strcpy(start_address_for_record, adr);
            }

            int i = 0;
            while (i < strlen(object_code))
            {
                text_record[i + s] = object_code[i];
                i++;
            }
            text_record[i + s] = '\0';
        }
        else
        {
            start_address_for_record = makeProperLength(start_address_for_record, 6);

            // writing new text record to output file and setting text record equal to current object code provided
            fprintf(output, "T%s%s%s\n", start_address_for_record,
                    makeProperLength(decToHex(strlen(text_record) / 2), 2), text_record);

            strcpy(start_address_for_record, adr);
            strcpy(text_record, object_code);
        }
    }
    else
    {
        if (strlen(text_record) > 0)
        {
            start_address_for_record = makeProperLength(start_address_for_record, 6);
            fprintf(output, "T%s%s%s\n", start_address_for_record, makeProperLength(decToHex(strlen(text_record) / 2), 2), text_record);
        }

        strcpy(start_address_for_record, adr);
        strcpy(text_record, "");
    }
}

// generate the object code from data in intermediate file and write it in output.txt
void generateObjectCode(char *line)
{
    // checking if the line is comment line
    if (line[0] == '.')
    {
        return;
    }

    char *object_code;
    object_code = (char *)malloc(10 * sizeof(char));
    object_code = "";
    char *temp[5];
    char *temp2;
    int j = 0;

    /* 
    storing the values in temp[4] as follow:
    temp[0]: address of the corresponding instruction 
    temp[1]: label if exists, otherwise '\t'
    temp[2]: opcode
    temp[3]: operand if exists, otherwise '\t'
    temp[4]: error flag
    */
    temp[0] = (char *)malloc(30 * sizeof(char));
    temp[1] = (char *)malloc(30 * sizeof(char));
    temp[2] = (char *)malloc(30 * sizeof(char));
    temp[3] = (char *)malloc(30 * sizeof(char));
    temp[4] = (char *)malloc(30 * sizeof(char));

    int i = 0;
    while (i < 8 && line[i] != ' ')
    {
        temp[0][i] = line[i];
        i++;
    }
    temp[0][i] = '\0';
     i = 8;
    while (i < 20 && line[i] != ' ')
    {
        temp[1][i-8] = line[i];
        i++;
    }
    temp[1][i-8] = '\0';
     i = 20;
    while (i < 34 && line[i] != ' ')
    {
        temp[2][i-20] = line[i];
        i++;
    }
    temp[2][i-20] = '\0';
     i = 34;
    while (i < 44 && line[i] != ' ')
    {
        temp[3][i-34] = line[i];
        i++;
    }
    temp[3][i-34] = '\0';
     i = 44;
    while (i < 69 && line[i] != '\0')
    {
        temp[4][i-44] = line[i];
        i++;
    }
    temp[4][i-44] = '\0';

    if( temp[4][0] != '\n'){
        printf("Error while executing the program.\nError code: ");
        printf("%s",temp[4]);
        exit(1);
    }

    char *temp9[4];
    temp9[0] = temp[0];
    temp9[1] = temp[1];
    temp9[2] = temp[2];
    temp9[3] = temp[3];

    temp[0] = temp9[1];
    temp[1] = temp9[2];
    temp[2] = temp9[3];
    temp[3] = temp9[0];

    if(!strcmp(temp[1], "END")){
        return;
    }

    if (checkIfOPCodeExists(temp[1]))
    {
        // setting first 2 bits of object code to OPCode.
        object_code = strAppend(object_code, getOPCode(temp[1]));

        if (checkIfLabelExists(temp[2]))
        {
            // searching the SYMTAB for operand
            char *code = getSYMCode(temp[2]);
            object_code = strAppend(object_code, makeProperLength(code, 4));
        }
        else if (temp[2][strlen(temp[2]) - 2] == ',' && temp[2][strlen(temp[2]) - 1] == 'X')
        {
            // operand is label + ',X' denoting that address is indexed
            char *temp_label;
            temp_label = (char *)malloc(strlen(temp[2]) * sizeof(char));
            strncpy(temp_label, temp[2], strlen(temp[2]) - 2);

            if (checkIfLabelExists(temp_label))
            {
                char *code = getSYMCode(temp_label);
                code = makeAddressIndexed(code);
                object_code = strAppend(object_code, makeProperLength(code, 4));
            }
            else
            {
                printf("Invalid label name.\n");
                exit(1);
            }
        }
        else if (!strcmp(temp[2], ""))
        {
            // if the operand field is empty
            object_code = strAppend(object_code, "0000");
        }
        else
        {
            printf("Invalid label name.\n");
            exit(1);
        }

        appendOrWriteLine(object_code, temp[3]);
    }

    else if (!strcmp(temp[1], "WORD"))
    {
        // converting the operand into object code of size 6
        char *hexCode = decToHex(atoi(temp[2]));
        object_code = makeProperLength(hexCode, 6);
        appendOrWriteLine(object_code, temp[3]);
    }

    else if (!strcmp(temp[1], "BYTE"))
    {
        // checking what type of operand is provided
        if (temp[2][0] == 'C')
        {
            // if it is constant, storing ascii values of letters in hexadecimal format
            char *code;
            code = (char *)malloc(strlen(temp[2]) * sizeof(char));
            strncpy(code, temp[2] + 2, strlen(temp[2]) - 3);
            code[strlen(temp[2]) - 3] = '\0';
            for (int i = 0; i < strlen(code); i++)
            {
                int a = code[i];
                object_code = strAppend(object_code, decToHex(a));
            }
        }
        else
        {
            // if the operand data cannot perfectly fit in byte format, adding an additional zero at the
            // starting of the program.
            char *temp5;
            temp5 = (char *)malloc(strlen(temp[2]) * sizeof(char));
            strncpy(temp5, temp[2] + 2, strlen(temp[2]) - 3);
            if (strlen(temp5) % 2)
            {
                temp5 = strAppend("0", temp5);
            }
            object_code = temp5;
        }

        appendOrWriteLine(object_code, temp[3]);
    }

    else
    {
        // there are RESW or RESB instructions causing a line break
        appendOrWriteLine(object_code, temp[3], 1);
    }

}

void pass_2()
{
    start_address_for_record = (char *)malloc(20 * sizeof(char));
    start_address_for_record = "";

    output = fopen("output.txt", "w");
    input = fopen("intermediate.txt", "r");

    char *input_line;
    size_t s = 90;
    input_line = (char *)malloc(sizeof(char) * s);
    text_record[0] = '\0';

    char *adr = makeProperLength(start_address, 6);
    char *size_of_program = subHexNum(LOCCTR, start_address);
    char *updated_size_of_program = makeProperLength(size_of_program, 6);

    // writing the header record in the output file
    fprintf(output, "H%-6s%s%s\n", program_name, adr, updated_size_of_program);

    getline(&input_line, &s, input);

    start_address_for_record = start_address;
    while (getline(&input_line, &s, input) > 0)
    {
        generateObjectCode(input_line);
    }

    // writing last text record to file
    start_address_for_record = makeProperLength(start_address_for_record, 6);
    fprintf(output, "T%s%s%s\n", start_address_for_record,
            makeProperLength(decToHex(strlen(text_record) / 2), 2), text_record);

    // writing end record to file
    fprintf(output, "E%s", adr);

    fclose(output);
    fclose(input);
    printf("Pass 2 done.\nIntermediate file is intermediate.txt. This file is used as input for Pass 2.\nOutput of the program is written in the file output.txt\n");
}