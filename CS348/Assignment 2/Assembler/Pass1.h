#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <stdbool.h>
#include <math.h>

char *LOCCTR, *start_address, *control_section;

// intermediate file to store the info
FILE *intermediate;

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

// to create the literal pool and write the literals which were defined after last LTORG
// or start of the program in the intermediate file.
void createLiteralPool()
{
    literal *temp_lit = LITTAB_head;
    while (temp_lit != NULL)
    {
        // checking if the literal has already been written in the file
        if (temp_lit->written == false)
        {
            char *temp2 = temp_lit->value;

            // assigning the LOCCTR as the address of the literal
            assignAddress(temp_lit->name, makeProperLength(LOCCTR, 4));

            // printing the literal pool entry to the intermediate file
            fprintf(intermediate, "\n%-8s\t%-12s\t%-14s\t%-10s", temp_lit->adr, "*", temp_lit->name, "");

            // adding size of literal to LOCCTR
            LOCCTR = addHexToDec(LOCCTR, (strlen(temp_lit->value) + 1) / 2);
            temp_lit->written = true;
        }
        temp_lit = temp_lit->next;
    }
}

// breaking the first line of assembler program into 3 parts namely program name, START code and start address.
// also adding the main control section in control section list named as program name
void getHeaderData(char *line)
{
    int s = strlen(line);
    int i = 0;
    int j = 0;
    char *part[3];
    part[0] = part[1] = part[2] = "";

    while (i < s && j < 3)
    {
        if (line[i] != ' ' && line[i] != '\t')
        {
            int k = 0;
            char *temp;
            temp = (char *)malloc(sizeof(char) * s);
            while (i < s && line[i] != ' ' && line[i] != '\t' && line[i] != '\n')
            {
                temp[k] = line[i];
                i++;
                k++;
            }
            part[j] = temp;
        }
        j++;
        while (i < s && (line[i] == ' ' || line[i] == '\t' || line[i] == '\n'))
        {
            i++;
        }
    }

    // initializing the LOCCTR to 0 if the opcode label in 1st line is not START
    if (strcmp(part[1], "START"))
    {
        LOCCTR = "0";
    }
    else
    {
        LOCCTR = part[2];
    }
    start_address = LOCCTR;
    LOCCTR = makeProperLength(LOCCTR, 4);

    // assigning the program name to the name of 1st or main control section and adding it to control sections list
    control_section = part[0];
    addToCSB(control_section);

    // writing the header record to intermdiate file
    fprintf(intermediate, "%-8s\t%-12s\t%-14s\t%-10s", LOCCTR, part[0], part[1], LOCCTR);
}

// process the input line and break it in parts
void processLine(char *line)
{
    // check if the line is a comment line or not
    if (line[0] == '.')
    {
        return;
    }

    int s = strlen(line);
    int i = 0;
    int j = 0;
    char *part[3];
    part[0] = part[1] = part[2] = "";

    // breaking the line in different parts
    while (i < s && j < 3)
    {
        if (line[i] != ' ' && line[i] != '\t')
        {
            int k = 0;
            char *temp;
            temp = (char *)malloc(sizeof(char) * s);
            while (i < s && line[i] != ' ' && line[i] != '\t' && line[i] != '\n')
            {
                temp[k] = line[i];
                i++;
                k++;
            }
            part[j] = temp;
        }
        j++;
        while (i < s && (line[i] == ' ' || line[i] == '\t' || line[i] == '\n'))
        {
            i++;
        }
    }

    // writing the remaining literals in the intermediate file which were not written previously by function
    // createLiteralPool and then assigning the last control section size to the corresponding control section block.
    if (!strcmp(part[1], "END"))
    {
        fprintf(intermediate, "\n%-8s\t%-12s\t%-14s\t%-10s", "", part[0], part[1], part[2]);
        createLiteralPool();
        increaseCSBsize(control_section, makeProperLength(LOCCTR, 4));
        return;
    }

    // write the input lines with assembler directives EXTREF or EXTDEF into immediate file
    if (!strcmp(part[1], "EXTREF") || !strcmp(part[1], "EXTDEF"))
    {
        fprintf(intermediate, "\n%-8s\t%-12s\t%-14s\t%-10s", "", part[0], part[1], part[2]);
        return;
    }

    // create the literal pool whenever come across the directive LTORG
    if (!strcmp(part[1], "LTORG"))
    {
        fprintf(intermediate, "\n%-8s\t%-12s\t%-14s\t%-10s", "", part[0], part[1], part[2]);
        createLiteralPool();
        return;
    }

    // start of new control section
    // assign size of previous control section to corresponding control section block
    // set the LOCCTR to 0
    // add the new control section to control sections list
    if (!strcmp(part[1], "CSECT"))
    {
        fprintf(intermediate, "\n%-8s\t%-12s\t%-14s\t%-10s", "0000", part[0], part[1], part[2]);
        increaseCSBsize(control_section, makeProperLength(LOCCTR, 4));
        control_section = part[0];
        LOCCTR = "0";
        addToCSB(part[0]);
        return;
    }

    // if label exists
    if (strcmp(part[0], ""))
    {
        // if the input line contains the directive EQU
        if (!strcmp(part[1], "EQU"))
        {
            if (!strcmp(part[2], "*"))
            {
                addToSYMTAB(part[0], control_section, LOCCTR, "R");
            }
            else
            {
                // solving the expression given as operand
                char *temp[100]; // to store the operands in expression
                char temp2[100]; // to store the operaators
                int j = 0;

                // breaking into operands
                char *temp3 = (char *)malloc(sizeof(char) * (strlen(part[2] + 1)));
                strncpy(temp3, part[2], strlen(part[2]));
                char *token = strtok(temp3, "-+");
                while (token != NULL)
                {
                    temp[j] = token;
                    j++;
                    token = strtok(NULL, "\t");
                }

                // getting all operators
                int k = 0;
                for (int i = 0; i < strlen(part[2]); i++)
                {
                    if (part[2][i] == '+' || part[2][i] == '-')
                    {
                        temp2[k] = part[2][i];
                        k++;
                    }
                }

                // evaluating the expression in operand place and then assigning the value to the label
                int ans;
                if (checkIfLabelExists(temp[0], control_section))
                {
                    ans = strToDec(getSYMCode(temp[0], control_section));
                }
                else if (!strcmp("*", temp[0]))
                {
                    ans = strToDec(LOCCTR);
                }
                else
                {
                    ans = atoi(temp[0]);
                }

                for (int i = 1; i < j; i++)
                {
                    if (temp2[i - 1] == '-')
                    {
                        if (checkIfLabelExists(temp[i], control_section))
                        {
                            ans -= strToDec(getSYMCode(temp[i], control_section));
                        }
                        else if (!strcmp("*", temp[i]))
                        {
                            ans -= strToDec(LOCCTR);
                        }
                        else
                        {
                            ans -= atoi(temp[i]);
                        }
                    }
                    else
                    {
                        if (checkIfLabelExists(temp[i], control_section))
                        {
                            ans += strToDec(getSYMCode(temp[i], control_section));
                        }
                        else if (!strcmp("*", temp[i]))
                        {
                            ans += strToDec(LOCCTR);
                        }
                        else
                        {
                            ans += atoi(temp[i]);
                        }
                    }
                }

                // adding the new symbol to SYMTAB
                addToSYMTAB(part[0], control_section, decToHex(ans), "R");
            }

            // printing the line to intermediate file
            fprintf(intermediate, "\n%-8s\t%-12s\t%-14s\t%-10s", getSYMCode(part[0], control_section), part[0], part[1], part[2]);
            return;
        }
        // label already exists in control section
        else if (checkIfLabelExists(part[0], control_section))
        {
            // if the label already exists make the error flag for the label true
            setErrorFlag(part[0], control_section);
            printf("Error Code: Duplicate Symbol Found\n");
            exit(1);
        }
        // new label in current control section
        else
        {
            addToSYMTAB(part[0], control_section, makeProperLength(LOCCTR, 4), "R");
        }
    }

    // write intermediate line
    fprintf(intermediate, "\n%-8s\t%-12s\t%-14s\t%-10s", makeProperLength(LOCCTR, 4), part[0], part[1], part[2]);

    // if opcode is in the place of instruction code in non extended format
    if (checkIfOPCodeExists(part[1]))
    {
        char *format = getFormat(part[1]);
        if (!strcmp(format, "1"))
        {
            LOCCTR = addHexToDec(LOCCTR, 1);
        }
        if (!strcmp(format, "2"))
        {
            LOCCTR = addHexToDec(LOCCTR, 2);
        }
        if (!strcmp(format, "3/4"))
        {
            LOCCTR = addHexToDec(LOCCTR, 3);
        }
    }
    // if opcode is in the place of instruction code in extended format
    else if (part[1][0] == '+')
    {
        char *temp2;
        temp2 = (char *)malloc(sizeof(char) * strlen(part[1]));
        strncpy(temp2, part[1] + 1, strlen(part[1]) - 1);
        if (checkIfOPCodeExists(temp2))
        {
            LOCCTR = addHexToDec(LOCCTR, 4);
        }
        else
        {
            printf("Invalid extended opcode");
            exit(1);
        }
    }
    // word instruction code
    else if (!strcmp(part[1], "WORD"))
    {
        LOCCTR = addHexToDec(LOCCTR, 3);
    }
    // byte instruction code
    else if (!strcmp(part[1], "BYTE"))
    {
        if (part[2][0] == 'C')
        {
            LOCCTR = addHexToDec(LOCCTR, strlen(part[2]) - 3);
        }
        else
        {
            int a = strlen(part[2]) - 3;
            LOCCTR = addHexToDec(LOCCTR, (a + 1) / 2);
        }
    }
    // resw code
    else if (!strcmp(part[1], "RESW"))
    {
        int s = atoi(part[2]);
        s *= 3;
        LOCCTR = addHexToDec(LOCCTR, s);
    }
    // resb code
    else if (!strcmp(part[1], "RESB"))
    {
        int s = atoi(part[2]);
        LOCCTR = addHexToDec(LOCCTR, s);
    }
    else
    {
        printf("Error Code: Invalid Instruction Code Found\n");
        remove("output.txt");
        remove("intermediate.txt");
        exit(1);
    }

    // if the operand is literal
    if (strlen(part[2]) > 0 && part[2][0] == '=')
    {
        char *temp3;
        char *object_code = "";
        temp3 = (char *)malloc(sizeof(char) * strlen(part[2]));
        strncpy(temp3, part[2] + 1, strlen(part[2]) - 1);

        // if the operand is character constant
        if (temp3[0] == 'C')
        {
            char *code = (char *)malloc(strlen(temp3) * sizeof(char));
            strncpy(code, temp3 + 2, strlen(temp3) - 3);
            code[strlen(temp3) - 3] = '\0';
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
            char *temp5 = (char *)malloc(strlen(part[2]) * sizeof(char));

            strncpy(temp5, part[2] + 3, strlen(part[2]) - 4);
            if (strlen(temp5) % 2)
            {
                temp5 = strAppend("0", temp5);
            }
            object_code = temp5;
        }

        if (!checkIfLiteralExist(temp3))
        {
            addToLITTAB(temp3, NULL, object_code, NULL);
        }
    }
}

void pass_1()
{
    intermediate = fopen("intermediate.txt", "w");

    control_section = "main";

    char *input_line;
    size_t s = 60;
    input_line = (char *)malloc(sizeof(char) * s);

    getline(&input_line, &s, stdin);
    getHeaderData(input_line);

    while (getline(&input_line, &s, stdin) > 0)
    {
        processLine(input_line);
    }

    fclose(intermediate);
    printf("Pass 1 done.\n");
}