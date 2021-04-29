#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <stdbool.h>

char *program_name, *operation_code, *LOCCTR, *start_address;

// intermediate file to store the info
FILE *intermediate;

// breaking the first line of assembler program into 3 parts namely program name, START code and start address.
void getHeaderData(char *line)
{
    int j = 0, s = strlen(line);
    program_name = (char *)malloc(sizeof(char) * s);
    operation_code = (char *)malloc(sizeof(char) * s);
    LOCCTR = (char *)malloc(sizeof(char) * s);
    start_address = (char *)malloc(sizeof(char) * s);

    for (int i = 0; i < s; i++)
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

        if (k == 0)
            continue;

        if (j == 0)
        {
            program_name = temp;
        }
        else if (j == 1)
        {
            operation_code = temp;
        }
        else if (j == 2)
        {
            LOCCTR = temp;
        }
        j++;
    }

    // initializing the LOCCTR to 0 if the opcode label in 1st line is not START
    if (strcmp(operation_code, "START"))
    {
        LOCCTR = "0";
    }
    start_address = LOCCTR;
}

// process the input line and break it in parts
void processLine(char *line)
{
    // check if the line is a comment line or not
    if (line[0] == '.')
    {
        return;
    }

    char *part1, *part2, *part3;
    part1 = part2 = part3 = "";
    int j = 0;
    int s = strlen(line);

    for (int i = 0; i < s; i++)
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

        if (k == 0)
            continue;

        if (j == 0)
        {
            part1 = temp;
        }
        else if (j == 1)
        {
            part2 = temp;
        }
        else if (j == 2)
        {
            part3 = temp;
        }
        j++;
    }

    // Processing the parts of the input line created. Searching them among different tables or adding
    // them in some table along with updating the LOCCTR simultaneously.
    if (strcmp(part1, "") && strcmp(part2, "") && strcmp(part3, ""))
    {
        if (strcmp(part2, "END"))
        {
            bool b = checkIfLabelExists(part1);
            if (b)
            {
                // if the label already exists make the error flag for the label true
                setErrorFlag(part1);
                fprintf(intermediate, "\n%-8s%-12s%-14s%-10s%s", LOCCTR, part1, part2, part3, "Duplicate Symbol");
            }
            else
            {
                // add new label to the SYMTAB
                addToSYMTAB(part1, LOCCTR);

                if (checkIfOPCodeExists(part2))
                {
                    fprintf(intermediate, "\n%-8s%-12s%-14s%-10s%s", LOCCTR, part1, part2, part3, "");
                    // adding 3 to the LOCCTR if part2 corresponds to a operation in OPTAB
                    LOCCTR = addHexToDec(LOCCTR, 3);
                }
                else if (!strcmp(part2, "WORD"))
                {
                    fprintf(intermediate, "\n%-8s%-12s%-14s%-10s%s", LOCCTR, part1, part2, part3, "");
                    // adding 3 to the LOCCTR if part2 corresponds to a "WORD"
                    LOCCTR = addHexToDec(LOCCTR, 3);
                }
                else if (!strcmp(part2, "RESW"))
                {
                    fprintf(intermediate, "\n%-8s%-12s%-14s%-10s%s", LOCCTR, part1, part2, part3, "");
                    // adding 3 * #operand to the LOCCTR if part2 corresponds to a "RESW"
                    int s = atoi(part3);
                    s *= 3;
                    LOCCTR = addHexToDec(LOCCTR, s);
                }
                else if (!strcmp(part2, "RESB"))
                {
                    fprintf(intermediate, "\n%-8s%-12s%-14s%-10s%s", LOCCTR, part1, part2, part3, "");
                    // adding #operand to the LOCCTR if part2 corresponds to a "RESB"
                    int s = atoi(part3);
                    LOCCTR = addHexToDec(LOCCTR, s);
                }
                else if (!strcmp(part2, "BYTE"))
                {
                    fprintf(intermediate, "\n%-8s%-12s%-14s%-10s%s", LOCCTR, part1, part2, part3, "");
                    // adding required number of bytes to LOCCTR which are required to store the data
                    // specified in the form of operand
                    if (part3[0] == 'C')
                    {
                        LOCCTR = addHexToDec(LOCCTR, strlen(part3) - 3);
                    }
                    else
                    {
                        int a = strlen(part3) - 3;
                        LOCCTR = addHexToDec(LOCCTR, (a + 1) / 2);
                    }
                }
                else
                {
                    fprintf(intermediate, "\n%-8s%-12s%-14s%-10s%s", LOCCTR, part1, part2, part3, "Invalid Operation Code");
                }
            }
        }
        else
        {
            fprintf(intermediate, "\n%-8s%-12s%-14s%-10s%s\n", "", part1, part2, part3, "");
        }
    }
    else
    {
        // checking if the operation code is not "END" marking the end of a program
        if (strcmp(part1, "END"))
        {
            if (checkIfOPCodeExists(part1))
            {
                fprintf(intermediate, "\n%-8s%-12s%-14s%-10s%s", LOCCTR, "", part1, part2, "");
                // adding 3 to the LOCCTR if part2 corresponds to a operation in OPTAB
                LOCCTR = addHexToDec(LOCCTR, 3);
            }
            else if (!strcmp(part1, "WORD"))
            {
                fprintf(intermediate, "\n%-8s%-12s%-14s%-10s%s", LOCCTR, "", part1, part2, "");
                // adding 3 to the LOCCTR if part2 corresponds to a "WORD"
                LOCCTR = addHexToDec(LOCCTR, 3);
            }
            else if (!strcmp(part1, "RESW"))
            {
                fprintf(intermediate, "\n%-8s%-12s%-14s%-10s%s", LOCCTR, "", part1, part2, "");
                // adding 3 * #operand to the LOCCTR if part2 corresponds to a "RESW"
                int s = atoi(part2);
                s *= 3;
                LOCCTR = addHexToDec(LOCCTR, s);
            }
            else if (!strcmp(part1, "RESB"))
            {
                fprintf(intermediate, "\n%-8s%-12s%-14s%-10s%s", LOCCTR, "", part1, part2, "");
                // adding #operand to the LOCCTR if part2 corresponds to a "RESB"
                int s = atoi(part2);
                LOCCTR = addHexToDec(LOCCTR, s);
            }
            else if (!strcmp(part1, "BYTE"))
            {
                fprintf(intermediate, "\n%-8s%-12s%-14s%-10s%s", LOCCTR, "", part1, part2, "");
                // adding required number of bytes to LOCCTR which are required to store the data
                // specified in the form of operand
                if (part2[0] == 'C')
                {
                    LOCCTR = addHexToDec(LOCCTR, strlen(part2) - 3);
                }
                else
                {
                    int a = strlen(part2) - 3;
                    LOCCTR = addHexToDec(LOCCTR, (a + 1) / 2);
                }
            }
            else
            {
                fprintf(intermediate, "\n%-8s%-12s%-14s%-10s%s", LOCCTR, "", part1, part2, "Invalid operation code");
            }
        }
        else
        {
            fprintf(intermediate, "\n%-8s%-12s%-14s%-10s%s\n", "", "", part1, part2, "");
        }
    }
}

void pass_1()
{
    intermediate = fopen("intermediate.txt", "w");

    char *input_line;
    size_t s = 60;
    input_line = (char *)malloc(sizeof(char) * s);

    getline(&input_line, &s, stdin);
    getHeaderData(input_line);

    fprintf(intermediate, "%-8s%-12s%-14s%-10s%s", start_address, program_name, operation_code, start_address, "");

    while (getline(&input_line, &s, stdin) > 0)
    {
        processLine(input_line);
    }

    fclose(intermediate);
    printf("Pass 1 done.\n");
}