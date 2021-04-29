#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <stdbool.h>
#include <math.h>

char *PROGADDR, *CSADDR, *INPUT_FILE_NAME, *control_section, *CSLTH;
int number_of_control_sections = 0;

void Pass1()
{
    // reading from file
    FILE *input_file;
    INPUT_FILE_NAME[strlen(INPUT_FILE_NAME) - 1] = '\0';
    const char *file_name = INPUT_FILE_NAME;
    if ((input_file = fopen(file_name, "r")) == NULL)
    {
        printf("Error while reading the file");
        exit(1);
    }

    char *input_line;
    input_line = (char *)malloc(sizeof(char) * 100);
    size_t temps = 100;
    while (getline(&input_line, &temps, input_file) > 0)
    {
        // if it header record or define record, inserting symbols in ESTAB
        if (input_line[0] == 'H')
        {
            number_of_control_sections++;
            if (number_of_control_sections == 1)
            {
                // this is the parent section
                CSADDR = PROGADDR;
            }
            else
            {
                // increase CSADDR so that it becomes equal to starting address of next section
                CSADDR = makeProperLength(addHexNum(CSADDR, CSLTH), 6);
            }

            char *name = (char *)malloc(sizeof(char) * 7);
            strncpy(name, input_line + 1, 6);
            for (int i = 0; i < 6; i++)
            {
                if (name[i] == ' ')
                {
                    name[i] = '\0';
                    break;
                }
            }

            control_section = name;
            CSLTH = (char *)malloc(sizeof(char) * 7);
            strncpy(CSLTH, input_line + 13, 6);

            if (checkIfSectionExists(name))
            {
                printf("Duplicate Section Exists");
                exit(1);
            }
            else
            {
                addToESTAB(name, CSADDR, control_section);
            }
        }
        else if (input_line[0] == 'D')
        {
            // breaking the string and then adding all seperate records to ESTAB
            for (int i = 1; i < strlen(input_line) - 2; i += 12)
            {
                char *name = (char *)malloc(sizeof(char) * 7);

                strncpy(name, input_line + i, 6);
                for (int i = 0; i < 6; i++)
                {
                    if (name[i] == ' ' || name[i] == '\n')
                    {
                        name[i] = '\0';
                        break;
                    }
                }

                char *adr = (char *)malloc(sizeof(char) * 7);
                strncpy(adr, input_line + i + 6, 6);
                adr = makeProperLength(addHexNum(adr, CSADDR), 6);

                if (checkIfSectionExists(name))
                {
                    printf("Duplicate Extern Symbol");
                    exit(1);
                }
                else
                {
                    addToESTAB(name, adr, control_section);
                }
            }
        }
    }

    fflush(stdin);
}
