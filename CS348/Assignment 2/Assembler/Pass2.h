#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <stdbool.h>

// reading input from intermediate.txt and then deleting it. Writing output to output.txt.
FILE *output, *input;

char text_record[61];
char *start_address_for_record;
char *control_section_name;
int control_block_number = 0;
char *modification_records[50];
int modification_records_count = 0;
char *extdef_array[50];
int extdef_count = 0;
char *extref_array[50];
int extref_count = 0;

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
    object_code = "";
    char *temp[4];
    int j = 0;

    /* 
    storing the values in temp[4] as follow:
    temp[0]: address of the corresponding instruction 
    temp[1]: label if exists, otherwise ''
    temp[2]: opcode
    temp[3]: operand if exists, otherwise ''
    */

    char *token = strtok(line, "\t");
    while (token != NULL)
    {
        temp[j] = token;
        for (int i = 0; i < strlen(temp[j]); i++)
        {
            if (temp[j][i] == ' ')
            {
                temp[j][i] = '\0';
                break;
            }
        }
        j++;
        token = strtok(NULL, "\t");
    }

    if (!strcmp(temp[2], "END"))
    {
        return;
    }

    // for the EXTDEF directive, store all the labels in extdef_array and then print the define record for the current
    // control section in output file
    if (!strcmp(temp[2], "EXTDEF"))
    {
        fprintf(output, "D");
        token = strtok(temp[3], ",");
        while (token != NULL)
        {
            if (token[strlen(token) - 1] == '\n')
            {
                token[strlen(token) - 1] = '\0';
            }
            extdef_array[extdef_count] = token;
            fprintf(output, "%-6s%s", token, makeProperLength(getSYMCodeWithoutSection(token), 6));
            extdef_count++;
            token = strtok(NULL, ",");
        }
        fprintf(output, "\n");
        return;
    }

    // for the EXTREF directive, store all the labels in extref_array and then print the refer record for the current
    // control section in output file
    if (!strcmp(temp[2], "EXTREF"))
    {
        fprintf(output, "R");
        token = strtok(temp[3], ",");
        while (token != NULL)
        {
            if (token[strlen(token) - 1] == '\n')
            {
                token[strlen(token) - 1] = '\0';
            }
            extref_array[extref_count] = token;
            fprintf(output, "%-6s", token);
            extref_count++;
            token = strtok(NULL, ",");
        }
        fprintf(output, "\n");
        return;
    }

    // begining of the new control section
    if (!strcmp(temp[2], "CSECT"))
    {
        // write the text record to output file if its length is greater than 0
        appendOrWriteLine("", start_address_for_record, 1);

        // write all modification records for the last control section in output file
        for (int h = 0; h < modification_records_count; h++)
        {
            fprintf(output, "%s\n", modification_records[h]);
        }
        modification_records_count = 0;

        // whether the first control block is finished or not, write ending accordingly.
        if (control_block_number == 0)
        {
            fprintf(output, "E000000\n\n");
        }
        else
        {
            fprintf(output, "E\n\n");
        }

        // increase the block number
        control_block_number++;
        control_section_name = temp[1];

        // header for new control section record
        fprintf(output, "H%-6s000000%s\n", control_section_name, makeProperLength(getBlockSize(control_section_name), 6));
    }

    // write the object code for literals
    if (!strcmp(temp[1], "*"))
    {
        object_code = getLiteralValue(temp[2]);
        appendOrWriteLine(object_code, temp[0]);
    }
    // if opcode is in the place of instruction code in non extended format
    else if (checkIfOPCodeExists(temp[2]))
    {
        char *tempp;
        char *format = getFormat(temp[2]);

        // opcode has 1 byte instructions
        if (!strcmp(format, "1"))
        {
            tempp = addHexToDec(temp[0], 1);
            object_code = getOPCode(temp[2]);
        }
        // opcode has 2 byte instructions
        if (!strcmp(format, "2"))
        {
            object_code = "0000";
            tempp = addHexToDec(temp[0], 2);
            object_code = addHexNum(object_code, strAppend(getOPCode(temp[2]), "00"));

            char *temp2[2];
            temp2[0] = strtok(temp[3], ",");
            temp2[1] = strtok(NULL, ",");
            object_code = addHexNum(object_code, strAppend(getSYMCodeWithoutSection(temp2[0]), "0"));

            if (temp2[1] != NULL)
                object_code = addHexNum(object_code, getSYMCodeWithoutSection(temp2[1]));
        }
        // opcode has 3 byte instructions
        if (!strcmp(format, "3/4"))
        {
            object_code = "000000";
            tempp = addHexToDec(temp[0], 3);
            object_code = addHexNum(object_code, strAppend(getOPCode(temp[2]), "0000"));

            if (strcmp(temp[3], ""))
            {
                // simple addressing
                if (checkIfLabelExistsWithoutSection(temp[3]))
                {
                    char *code = subHexNum(getSYMCodeWithoutSection(temp[3]), tempp);
                    object_code = addHexNum(object_code, code);
                    // adding hexadecimal equivalent of the flags
                    object_code = addHexNum(object_code, "32000");
                }
                // indexed addressing
                else if (temp[3][strlen(temp[3]) - 2] == ',' && temp[3][strlen(temp[3]) - 1] == 'X')
                {
                    char *temmpp = (char *)malloc(sizeof(char) * strlen(temp[3]));
                    strncpy(temmpp, temp[3], strlen(temp[3]) - 2);
                    if (checkIfLabelExistsWithoutSection(temmpp))
                    {
                        char *code = subHexNum(getSYMCodeWithoutSection(temmpp), tempp);
                        object_code = addHexNum(object_code, code);
                        // adding hexadecimal equivalent of the flags
                        object_code = addHexNum(object_code, "3A000");
                    }
                    else
                    {
                        printf("Invalid input");
                        exit(1);
                    }
                }
                // indirect addressing
                else if (temp[3][0] == '@')
                {
                    char *tempp2 = (char *)malloc(sizeof(char) * strlen(temp[3]));
                    strncpy(tempp2, temp[3] + 1, strlen(temp[3]) - 1);
                    if (checkIfLabelExistsWithoutSection(tempp2))
                    {
                        char *code = subHexNum(getSYMCodeWithoutSection(tempp2), tempp);
                        object_code = addHexNum(object_code, code);
                        // adding hexadecimal equivalent of the flags
                        object_code = addHexNum(object_code, "22000");
                    }
                    else
                    {
                        printf("Wrong input operation code");
                        exit(1);
                    }
                }
                // literal in operand
                else if (temp[3][0] == '=')
                {
                    char *tempp2 = (char *)malloc(sizeof(char) * strlen(temp[3]));
                    strncpy(tempp2, temp[3] + 1, strlen(temp[3]) - 1);
                    if (checkIfLiteralExist(tempp2))
                    {
                        char *code = subHexNum(getLiteralAddress(tempp2), tempp);
                        object_code = addHexNum(object_code, code);
                        // adding hexadecimal equivalent of the flags
                        object_code = addHexNum(object_code, "32000");
                    }
                    else
                    {
                        printf("Invalid literal in input");
                        exit(1);
                    }
                }
                // immediate addresing
                else if (temp[3][0] == '#')
                {
                    char *tempp2 = (char *)malloc(sizeof(char) * strlen(temp[3]));
                    strncpy(tempp2, temp[3] + 1, strlen(temp[3]) - 1);
                    if (checkIfLabelExistsWithoutSection(tempp2))
                    {
                        object_code = addHexNum(object_code, getSYMCodeWithoutSection(tempp2));
                        object_code = addHexNum(object_code, "10000");
                    }
                    else
                    {
                        object_code = addHexNum(object_code, decToHex(atoi(tempp2)));
                        object_code = addHexNum(object_code, "10000");
                    }
                }
            }
            else
            {
                object_code = addHexNum(object_code, "30000");
            }

            object_code = makeProperLength(object_code, 6);
        }

        appendOrWriteLine(object_code, temp[0]);
    }
    // if opcode is in the place of instruction code and in extended format
    else if (temp[2][0] == '+')
    {
        // add a modification record entry for the extended format instruction as the operand will be a label defined in
        // other control section
        modification_records[modification_records_count] = strAppend("M", strAppend(makeProperLength(addHexNum(temp[0], "1"), 6), strAppend("05+", temp[3])));

        char *temppp = (char *)malloc(sizeof(char) * strlen(temp[2]));
        strncpy(temppp, temp[2] + 1, strlen(temp[2]) - 1);
        temppp[strlen(temp[2]) - 1] = '\0';

        if (checkIfOPCodeExists(temppp))
        {
            // adding hexadecimal numbers equivalent to the set flags
            object_code = strAppend(getOPCode(temppp), "100000");
            object_code = addHexNum(object_code, "3000000");

            if (temp[3][strlen(temp[3]) - 2] == ',' && temp[3][strlen(temp[3]) - 1] == 'X')
            {
                object_code = addHexNum(object_code, "800000");
                char *temmpp = (char *)malloc(sizeof(char) * strlen(temp[3]));
                strncpy(temmpp, temp[3], strlen(temp[3]) - 2);
                modification_records[modification_records_count] = strAppend("M", strAppend(makeProperLength(addHexNum(temp[0], "1"), 6), strAppend("05+", temmpp)));
            }
            else
            {
                modification_records[modification_records_count] = strAppend("M", strAppend(makeProperLength(addHexNum(temp[0], "1"), 6), strAppend("05+", temp[3])));
            }
        }
        else
        {
            printf("Invalid operation code in input");
            exit(1);
        }

        modification_records_count++;
        appendOrWriteLine(object_code, temp[0]);
    }
    // evaluating the expression in operand.
    else if (!strcmp(temp[2], "WORD"))
    {
        // solving the expression given as operand
        char *tempp2[100]; // to store the operands in expression
        char temp2[100];   // to store the operaators

        // breaking into operands
        int j = 0;
        char *temp3 = (char *)malloc(sizeof(char) * (strlen(temp[3] + 1)));
        strncpy(temp3, temp[3], strlen(temp[3]));
        char *token = strtok(temp3, "-+");
        while (token != NULL)
        {
            if (token[strlen(token) - 1] == '\n')
            {
                token[strlen(token) - 1] = '\0';
            }
            tempp2[j] = token;
            j++;
            token = strtok(NULL, "\t");
        }

        // getting all operators
        int k = 0;
        for (int i = 0; i < strlen(temp[3]); i++)
        {
            if (temp[3][i] == '+' || temp[3][i] == '-')
            {
                temp2[k] = temp[3][i];
                k++;
            }
        }

        // evaluating the expression in operand place
        int ans;
        int flag = 0;
        if (checkIfLabelExistsWithoutSection(tempp2[0]))
        {
            if (!checkIfLabelExists(tempp2[0], control_section_name))
            {
                modification_records[modification_records_count] = strAppend("M", strAppend(makeProperLength(temp[0], 6), strAppend("06+", tempp2[0])));
                modification_records_count++;
                flag++;
            }
            ans = strToDec(getSYMCodeWithoutSection(tempp2[0]));
        }
        else if (!strcmp("*", tempp2[0]))
        {
            ans = strToDec(temp[0]);
        }
        else
        {
            ans = atoi(tempp2[0]);
        }

        for (int i = 1; i < j; i++)
        {
            if (temp2[i - 1] == '-')
            {
                if (checkIfLabelExistsWithoutSection(tempp2[i]))
                {
                    if (!checkIfLabelExists(tempp2[i], control_section_name))
                    {
                        modification_records[modification_records_count] = strAppend("M", strAppend(makeProperLength(temp[0], 6), strAppend("06-", tempp2[i])));
                        modification_records_count++;
                        flag++;
                    }
                    ans -= strToDec(getSYMCodeWithoutSection(tempp2[i]));
                }
                else if (!strcmp("*", tempp2[i]))
                {
                    ans -= strToDec(temp[0]);
                }
                else
                {
                    ans -= atoi(tempp2[i]);
                }
            }
            else
            {
                if (checkIfLabelExistsWithoutSection(tempp2[i]))
                {
                    ans += strToDec(getSYMCodeWithoutSection(tempp2[i]));
                    if (!checkIfLabelExists(tempp2[i], control_section_name))
                    {
                        modification_records[modification_records_count] = strAppend("M", strAppend(makeProperLength(temp[0], 6), strAppend("06+", tempp2[i])));
                        modification_records_count++;
                        flag++;
                    }
                }
                else if (!strcmp("*", tempp2[i]))
                {
                    ans += strToDec(temp[0]);
                }
                else
                {
                    ans += atoi(tempp2[i]);
                }
            }
        }

        // if the expression referenced any label outside the current control section, then make
        // the object code 000000
        if (flag == 0)
        {
            object_code = makeProperLength(decToHex(ans), 6);
        }
        else
        {
            object_code = "000000";
        }

        appendOrWriteLine(object_code, temp[0]);
    }

    else if (!strcmp(temp[2], "BYTE"))
    {
        // if it is character constant, storing ascii values of letters in hexadecimal format
        if (temp[3][0] == 'C')
        {
            char *code = (char *)malloc(strlen(temp[3]) * sizeof(char));
            strncpy(code, temp[3] + 2, strlen(temp[3]) - 3);
            code[strlen(temp[3]) - 3] = '\0';
            for (int i = 0; i < strlen(code); i++)
            {
                int a = code[i];
                object_code = strAppend(object_code, decToHex(a));
            }
        }
        // if it is hexadecimal constant
        else
        {
            // if the operand data cannot perfectly fit in byte format, adding an additional zero at the starting of the program.
            char *temp5 = (char *)malloc(strlen(temp[3]) * sizeof(char));
            strncpy(temp5, temp[3] + 2, strlen(temp[3]) - 3);
            if (strlen(temp5) % 2)
            {
                temp5 = strAppend("0", temp5);
            }
            object_code = temp5;
        }
        appendOrWriteLine(object_code, temp[0]);
    }

    else
    {
        // there are RESW or RESB instructions causing a line break
        appendOrWriteLine(object_code, temp[0], 1);
    }
}

void pass_2()
{
    // adding register codes to SYMTAB
    addToSYMTAB("A", NULL, "0", "A");
    addToSYMTAB("X", NULL, "1", "A");
    addToSYMTAB("L", NULL, "2", "A");
    addToSYMTAB("PC", NULL, "8", "A");
    addToSYMTAB("SW", NULL, "9", "A");
    addToSYMTAB("B", NULL, "3", "A");
    addToSYMTAB("S", NULL, "4", "A");
    addToSYMTAB("T", NULL, "5", "A");
    addToSYMTAB("F", NULL, "6", "A");

    start_address_for_record = (char *)malloc(20 * sizeof(char));
    start_address_for_record = "";

    output = fopen("output.txt", "w");
    input = fopen("intermediate.txt", "r");

    char *input_line;
    size_t s = 90;
    input_line = (char *)malloc(sizeof(char) * s);
    text_record[0] = '\0';

    // write header record
    char *temp[4];
    int j = 0;
    getline(&input_line, &s, input);
    char *token = strtok(input_line, "\t");
    while (token != NULL)
    {
        temp[j] = token;
        for (int i = 0; i < strlen(temp[j]); i++)
        {
            if (temp[j][i] == ' ')
            {
                temp[j][i] = '\0';
                break;
            }
        }
        j++;
        token = strtok(NULL, "\t");
    }

    if (!strcmp(temp[2], "START"))
    {
        fprintf(output, "H%-6s000000%s\n", temp[1], makeProperLength(getBlockSize(temp[1]), 6));
    }

    // giving starting control name as program name
    control_section_name = temp[1];
    start_address_for_record = start_address;

    while (getline(&input_line, &s, input) > 0)
    {
        generateObjectCode(input_line);
    }

    // writing last text record to file
    appendOrWriteLine("", start_address_for_record, 1);

    // writing modification records for last control section
    for (int h = 0; h < modification_records_count; h++)
    {
        fprintf(output, "%s\n", modification_records[h]);
    }

    // writing end record to file
    if (control_block_number > 0)
    {
        fprintf(output, "E");
    }
    else
    {
        fprintf(output, "E000000");
    }

    fclose(output);
    fclose(input);
    printf("Pass 2 done.\nIntermediate file is intermediate.txt. This file is used as input for Pass 2.\nOutput of the program is written in the file output.txt\n");
}