#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <stdbool.h>
#include <math.h>
#include <bits/stdc++.h>

using namespace std;

char *EXECADDR, *start_write_address, *cur_token, *adr2 = "";
long long int value;
char *local_data;

map<string, string> memory_locations;

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

// write to the output to file
void writeToFile()
{
    CSADDR = makeProperLength(addHexNum(CSADDR, "10"), 6);
    CSADDR[5] = '0';
    fstream afile;
    afile.open("output.txt", ios::out | ios::in);

    // iterating from start_write_address to final value of CSADDR in which the current program will get loaded
    while (strcmp(start_write_address, CSADDR))
    {
        string record = start_write_address;
        record += "   ";
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                if (checkIfLocationExist(start_write_address))
                {
                    record += memory_locations[start_write_address];
                }
                else
                {
                    record += "--";
                }
                start_write_address = makeProperLength(addHexNum(start_write_address, "1"), 6);
            }
            record += " ";
        }
        afile << record << endl;
    }
}

void Pass2()
{
    CSADDR = PROGADDR;
    EXECADDR = PROGADDR;
    number_of_control_sections = 0;

    FILE *input_file;
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
        // it is a header record, storing the section length in CSLTH
        if (input_line[0] == 'H')
        {
            number_of_control_sections++;
            if (number_of_control_sections == 1)
            {
                CSADDR = PROGADDR;
            }
            else
            {
                CSADDR = makeProperLength(addHexNum(CSADDR, CSLTH), 6);
            }

            CSLTH = (char *)malloc(sizeof(char) * 7);
            strncpy(CSLTH, input_line + 13, 6);
        }
        // if it is a text record, storing the byte wise data into locations in a map memory_locations.
        else if (input_line[0] == 'T')
        {
            char *start_address = (char *)malloc(sizeof(char) * 7);
            strncpy(start_address, input_line + 1, 6);

            start_address = addHexNum(start_address, CSADDR);
            start_address = makeProperLength(start_address, 6);
            char *address = start_address;

            for (int i = 9; i < strlen(input_line) - 1; i += 2)
            {
                char *data = (char *)malloc(sizeof(char) * 3);
                strncpy(data, input_line + i, 2);
                data[2] = '\0';
                memory_locations[address] = data;
                addToMemory(address, data);
                address = makeProperLength(addHexNum(address, "1"), 6);
            }
        }
        // modification record
        else if (input_line[0] == 'M')
        {
            char *adr = (char *)malloc(sizeof(char) * 10);
            char *token = (char *)malloc(sizeof(char) * 10);
            strncpy(adr, input_line + 1, 6);
            strncpy(token, input_line + 10, strlen(input_line) - 10);
            adr = makeProperLength(addHexNum(adr, CSADDR), 6);
            token[strlen(token) - 2] = '\0';

            char *local_data;
            if (!strcmp(adr2, ""))
            {
                adr2 = adr;
                local_data = (char *)malloc(sizeof(char) * 7);
                local_data = getData(adr);
                adr = makeProperLength(addHexNum(adr, "1"), 6);
                local_data = strAppend(local_data, getData(adr));
                adr = makeProperLength(addHexNum(adr, "1"), 6);
                local_data = strAppend(local_data, getData(adr));
                value = strToDec(local_data);
            }
            else if (strcmp(adr2, adr))
            {
                // byte level updatation
                local_data = makeProperLength(decToHex(value), 6);
                char *data = (char *)malloc(sizeof(char) * 3);
                data[0] = local_data[0];
                data[1] = local_data[1];
                data[2] = '\0';
                memory_locations[adr2] = data;

                data[0] = local_data[2];
                data[1] = local_data[3];
                adr2 = makeProperLength(addHexNum(adr2, "1"), 6);
                changeMemory(adr2, data);
                memory_locations[adr2] = data;

                data[0] = local_data[4];
                data[1] = local_data[5];
                adr2 = makeProperLength(addHexNum(adr2, "1"), 6);
                changeMemory(adr2, data);
                memory_locations[adr2] = data;

                adr2 = adr;
                local_data = (char *)malloc(sizeof(char) * 7);
                local_data = getData(adr);
                adr = makeProperLength(addHexNum(adr, "1"), 6);
                local_data = strAppend(local_data, getData(adr));
                adr = makeProperLength(addHexNum(adr, "1"), 6);
                local_data = strAppend(local_data, getData(adr));
                if (local_data[0] >= '8')
                {
                    value = strToDec(local_data);
                    value -= pow(2, 24);
                }
                else
                {
                    value = strToDec(local_data);
                }
            }

            if (input_line[9] == '+')
            {
                value += strToDec(addressFromESTAB(token));
            }
            else
            {
                value -= strToDec(addressFromESTAB(token));
            }
        }
        else if (input_line[0] == 'E')
        {
            if (strlen(input_line) > 1 && input_line[1] == '0')
            {
                char *adr = (char *)malloc(sizeof(char) * strlen(input_line));
                strncpy(adr, input_line + 1, strlen(input_line) - 2);
                for (int i = 0; i < strlen(adr); i++)
                {
                    if (adr[i] == '\n')
                    {
                        adr[i] = '\0';
                    }
                }
                EXECADDR = makeProperLength(addHexNum(CSADDR, adr), 6);
            }
        }
    }

    local_data = makeProperLength(decToHex(value), 6);
    char *data = (char *)malloc(sizeof(char) * 3);
    data[0] = local_data[0];
    data[1] = local_data[1];
    data[2] = '\0';
    memory_locations[adr2] = data;

    data[0] = local_data[2];
    data[1] = local_data[3];
    adr2 = makeProperLength(addHexNum(adr2, "1"), 6);
    changeMemory(adr2, data);
    memory_locations[adr2] = data;

    data[0] = local_data[4];
    data[1] = local_data[5];
    adr2 = makeProperLength(addHexNum(adr2, "1"), 6);
    changeMemory(adr2, data);
    memory_locations[adr2] = data;

    CSADDR = makeProperLength(addHexNum(CSADDR, CSLTH), 6);
    printf("The execution will start from here i.e. value of EXECADDR is %s\nOutptut is written to output.txt\n", EXECADDR);
    writeToFile();
}