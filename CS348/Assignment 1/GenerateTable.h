#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

// value modulo which hash value is calculated
#define hashValue 31

// number of operation codes to be registered in OPTAB
int num_of_opcodes;

// operation name
char *op_names[] = {"LDA", "LDX", "LDL", "STA", "STX", "STL", "LDCH", "STCH", "ADD",
                    "SUB", "MUL", "DIV", "COMP", "J", "JLT", "JEQ", "JGT", "JSUB",
                    "RSUB", "TIX", "TD", "RD", "WD"};

// opcode corresponding to the operation in the op_names[]
char *op_codes[] = {"00", "04", "08", "0C", "10", "14", "50", "54", "18",
                    "1C", "20", "24", "28", "3C", "38", "30", "34", "48",
                    "4C", "2C", "E0", "D8", "DC"};

// structure of an operation entry in OPTAB
struct opcode
{
    char *name;
    char *code;
    opcode *next;
};

// structure of a label entry in SYMTAB
struct sym
{
    char *label;
    char *address;
    bool error_flag;
    sym *next;
};

typedef struct opcode opcode;
typedef struct sym sym;

// initializing hash table for OPTAB and SYMTAB
opcode *OPTAB[hashValue];
sym *SYMTAB[hashValue];

// getting the key(position) in hash table to which the given entry correspond according to hash function
int getHashKey(char *a)
{
    int count = 0;
    int iterator = 0;

    while (a[iterator] != '\0')
    {
        count += a[iterator] - 65;
        iterator++;
    }
    return (count + hashValue) % hashValue;
}

// generate the OPTAB with entries for all the operations in op_names[]
void generateOPTAB()
{
    // initializing the chain start as NULL for all
    for (int i = 0; i < hashValue; i++)
    {
        OPTAB[i] = NULL;
        SYMTAB[i] = NULL;
    }

    // number of operation codes to be registered in OPTAB
    num_of_opcodes = sizeof(op_names) / sizeof(op_names[0]);

    for (int i = 0; i < num_of_opcodes; i++)
    {
        // creating new opcode object and assigning values
        opcode *operation = (opcode *)malloc(sizeof(opcode));
        operation->name = op_names[i];
        operation->code = op_codes[i];

        /* 
        Getting hashVal, the hash function value corresponding to this object and attaching this object 
        to the chain of OPTAB[hashVal]. Here in the case of collision of hashVal of 2 different objects, 
        we are making a chain at OPTAB[hashVal] with all hashVal colliding objects 
        */
        int hashVal = getHashKey(op_names[i]);
        if (OPTAB[hashVal] == NULL)
        {
            OPTAB[hashVal] = operation;
        }
        else
        {
            operation->next = OPTAB[hashVal];
            OPTAB[hashVal] = operation;
        }
    }
    printf("OPTAB Generation Completed\n");
}

// check if the operation queried exists in OPTAB or not
bool checkIfOPCodeExists(char *inp)
{
    int hashVal = getHashKey(inp);
    opcode *node = OPTAB[hashVal];

    while (node != NULL)
    {
        if (!strcmp(node->name, inp))
        {
            // object found
            return true;
        }
        node = node->next;
    };
    // object not found
    return false;
}

// get OPCode corresponding to the operation queried
char *getOPCode(char *inp)
{
    int hashVal = getHashKey(inp);
    opcode *node = OPTAB[hashVal];

    while (node != NULL)
    {
        if (!strcmp(node->name, inp))
        {
            return node->code;
        }
        node = node->next;
    };
}

// checking if the queried label exists in SYMTAB
bool checkIfLabelExists(char *inp)
{
    int hashVal = getHashKey(inp);
    sym *node = SYMTAB[hashVal];

    while (node != NULL)
    {
        if (!strcmp(node->label, inp))
        {
            // label found
            return true;
        }
        node = node->next;
    };
    // label not found
    return false;
}

// setting error flag of the label for which label with same name already exists
void setErrorFlag(char *inp)
{
    int hashVal = getHashKey(inp);
    sym *node = SYMTAB[hashVal];

    while (node != NULL)
    {
        if (!strcmp(node->label, inp))
        {
            node->error_flag = true;
            break;
        }
        node = node->next;
    };
}

// add the label to SYMTAB
void addToSYMTAB(char *inp, char *adr)
{
    // creating new sym object and assigning values
    sym *node = (sym *)malloc(sizeof(sym));
    int hashVal = getHashKey(inp);
    node->address = adr;
    node->label = inp;
    node->error_flag = false;
    node->next = NULL;

    /* 
    Getting hashVal, the hash function value corresponding to this object and attaching this object 
    to the chain of SYMTAB[hashVal]. Here in the case of collision of hashVal of 2 different objects, 
    we are making a chain at SYMTAB[hashVal] with all hashVal colliding objects
    */
    if (SYMTAB[hashVal] == NULL)
    {
        SYMTAB[hashVal] = node;
    }
    else
    {
        node->next = SYMTAB[hashVal];
        SYMTAB[hashVal] = node;
    }
}

// get address corresponding to the queried label
char *getSYMCode(char *inp)
{
    int hashVal = getHashKey(inp);
    sym *node = SYMTAB[hashVal];

    while (node != NULL)
    {
        if (!strcmp(node->label, inp))
        {
            return node->address;
        }
        node = node->next;
    };
}