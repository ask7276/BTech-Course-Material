#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <bits/stdc++.h>

// value modulo which hash value is calculated
#define hashValue 31

struct external_symbol
{
    char *label;
    char *adr;
    char *control_section;
    external_symbol *next;
};

struct memory
{
    char *location;
    char *data;
    memory *next;
};

typedef external_symbol extsym;
typedef memory memory;

extsym *ESTAB[hashValue];
memory *start = NULL;
memory *end = NULL;

// getting the key(position) in hash table to which the given entry correspond according to hash function
int getHashKey(char *a)
{
    int count = 0;
    int iterator = 0;

    while (a[iterator] != '\0')
    {
        count += a[iterator];
        iterator++;
    }
    return (count + hashValue) % hashValue;
}

// add new external symbol entry to ESTAB
void addToESTAB(char *inp, char *adr, char *control_section)
{
    // add new external symbol object to ESTAB
    extsym *node = (extsym *)malloc(sizeof(extsym));
    node->label = inp;
    node->adr = adr;
    node->control_section = control_section;
    int hashVal = getHashKey(inp);

    /*
    Getting hashVal, the hash function value corresponding to this object and attaching this object 
    to the chain of ESTAB[hashVal]. Here in the case of collision of hashVal of 2 different objects, 
    we are making a chain at ESTAB[hashVal] with all hashVal colliding objects.
    */
    if (ESTAB[hashVal] == NULL)
    {
        ESTAB[hashVal] = node;
    }
    else
    {
        node->next = ESTAB[hashVal];
        ESTAB[hashVal] = node;
    }
}

char *addressFromESTAB(char *inp)
{
    int hashVal = getHashKey(inp);
    extsym *node = ESTAB[hashVal];

    while (node != NULL)
    {
        if (!strcmp(node->label, inp))
        {
            return node->adr;
        }
        node = node->next;
    }
}

bool checkIfSectionExists(char *inp)
{
    int hashVal = getHashKey(inp);
    extsym *node = ESTAB[hashVal];

    while (node != NULL)
    {
        if (!strcmp(node->label, inp))
        {
            return true;
        }
        node = node->next;
    }
    return false;
}

void addToMemory(char *loc, char *data)
{
    memory *node = (memory *)malloc(sizeof(memory));
    node->data = data;
    node->location = loc;

    if (start == NULL)
    {
        start = node;
        end = node;
    }
    else
    {
        end->next = node;
        end = node;
    }
}

char *getData(char *loc)
{
    memory *temp = start;
    while (temp != NULL)
    {
        if (!strcmp(temp->location, loc))
        {
            return temp->data;
        }
        temp = temp->next;
    }
}

bool checkIfLocationExist(char *loc)
{
    memory *temp = start;
    while (temp != NULL)
    {
        if (!strcmp(temp->location, loc))
        {
            return true;
        }
        temp = temp->next;
    }
    return false;
}

void changeMemory(char *loc, char *data){
    memory *temp = start;
    while (temp != NULL)
    {
        if (!strcmp(temp->location, loc))
        {
            temp->data = data;
        }
        temp = temp->next;
    }
}
