#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

// value modulo which hash value is calculated
#define hashValue 31

// number of operation codes to be registered in OPTAB
int num_of_opcodes;

// operation name
char *op_names[] = {"LDA", "LDX", "LDL", "LDB", "LDT", "STA", "STX", "STL", "LDCH",
                    "ADD", "SUB", "MUL", "DIV", "COMP", "COMPR", "CLEAR", "J", "JLT", "JEQ",
                    "JGT", "JSUB", "RSUB", "TIX", "TIXR", "TD", "RD", "WD", "STCH"};

// opcode corresponding to the operation in the op_names[]
char *op_codes[] = {"00", "04", "08", "68", "74", "0C", "10", "14", "50",
                    "18", "1C", "20", "24", "28", "A0", "B4", "3C", "38", "30",
                    "34", "48", "4C", "2C", "B8", "E0", "D8", "DC", "54"};

// format of the corresponding operation in op_names[]
char *op_format[] = {"3/4", "3/4", "3/4", "3/4", "3/4", "3/4", "3/4", "3/4", "3/4",
                     "3/4", "3/4", "3/4", "3/4", "3/4", "2", "2", "3/4", "3/4", "3/4",
                     "3/4", "3/4", "3/4", "3/4", "2", "3/4", "3/4", "3/4", "3/4"};

// structure of an operation entry in OPTAB
struct opcode
{
    char *name;
    char *code;
    char *format;
    opcode *next;
};

// structure of a label entry in SYMTAB
struct sym
{
    char *label;
    char *address;
    bool error_flag;
    char *control_section;
    char *position;
    sym *next;
};

// control section entries
struct control_section_block
{
    char *name;
    char *size;
    control_section_block *next;
};

// literals
struct literal
{
    char *name;
    char *adr;
    char *value;
    char *exp;
    bool written;
    literal *next;
};

typedef struct opcode opcode;
typedef struct sym sym;
typedef struct control_section_block csb;
typedef struct literal literal;

// initializing hash table for OPTAB and SYMTAB and linked lists for control sections and literals
opcode *OPTAB[hashValue];
sym *SYMTAB[hashValue];
csb *csb_head = NULL;
csb *csb_tail = NULL;
literal *LITTAB_head = NULL;
literal *LITTAB_tail = NULL;

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
        operation->format = op_format[i];

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

// get instruction length format for the given OPCode
char *getFormat(char *inp)
{
    int hashVal = getHashKey(inp);
    opcode *node = OPTAB[hashVal];

    while (node != NULL)
    {
        if (!strcmp(node->name, inp))
        {
            return node->format;
        }
        node = node->next;
    };
}

// checking if the queried label exists in SYMTAB given section name
bool checkIfLabelExists(char *inp, char *section)
{
    int hashVal = getHashKey(inp);
    sym *node = SYMTAB[hashVal];

    while (node != NULL)
    {
        if (!strcmp(node->label, inp) && !strcmp(node->control_section, section))
        {
            // label found
            return true;
        }
        node = node->next;
    };
    // label not found
    return false;
}

// checking if the queried label exists in SYMTAB without section name given
bool checkIfLabelExistsWithoutSection(char *inp)
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
void setErrorFlag(char *inp, char *section)
{
    int hashVal = getHashKey(inp);
    sym *node = SYMTAB[hashVal];

    while (node != NULL)
    {
        if (!strcmp(node->label, inp) && !strcmp(node->control_section, section))
        {
            node->error_flag = true;
            break;
        }
        node = node->next;
    };
}

// add the label to SYMTAB
void addToSYMTAB(char *inp, char *section, char *adr, char *pos)
{
    // creating new sym object and assigning values
    sym *node = (sym *)malloc(sizeof(sym));
    int hashVal = getHashKey(inp);
    node->address = adr;
    node->label = inp;
    node->control_section = section;
    node->error_flag = false;
    node->position = pos;
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

// get address corresponding to the queried label given section name too
char *getSYMCode(char *inp, char *section)
{
    int hashVal = getHashKey(inp);
    sym *node = SYMTAB[hashVal];

    while (node != NULL)
    {
        if (!strcmp(node->label, inp) && !strcmp(node->control_section, section))
        {
            return node->address;
        }
        node = node->next;
    };
}

// get address corresponding to the queried label without giving section name
char *getSYMCodeWithoutSection(char *inp)
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

// getting the section name given the label name
char *getSectionName(char *inp)
{
    int hashVal = getHashKey(inp);
    sym *node = SYMTAB[hashVal];

    while (node != NULL)
    {
        if (!strcmp(node->label, inp))
        {
            return node->control_section;
        }
        node = node->next;
    };
}

// add entry to ocntrol sections list
void addToCSB(char *name)
{
    csb *node = (csb *)malloc(sizeof(control_section_block));
    node->name = name;
    node->size = "0";
    if (csb_head == NULL)
    {
        csb_head = node;
        csb_tail = csb_head;
    }
    else
    {
        csb_tail->next = node;
        csb_tail = node;
    }
}

// assign size of the control section
void increaseCSBsize(char *name, char *size)
{
    csb *temp;
    temp = csb_head;
    while (temp != NULL)
    {
        if (!strcmp(temp->name, name))
        {
            temp->size = size;
        }
        temp = temp->next;
    }
}

// get the size of the control section
char *getBlockSize(char *name)
{
    csb *temp;
    temp = csb_head;
    while (temp != NULL)
    {
        if (!strcmp(temp->name, name))
        {
            return temp->size;
        }
        temp = temp->next;
    }
}

// add literals to literals list
void addToLITTAB(char *name, char *adr, char *value, char *exp)
{
    literal *node = (literal *)malloc(sizeof(literal));
    node->adr = adr;
    node->name = name;
    node->value = value;
    node->written = false;
    node->exp = exp;
    if (LITTAB_head == NULL)
    {
        LITTAB_head = node;
        LITTAB_tail = node;
    }
    else
    {
        LITTAB_tail->next = node;
        LITTAB_tail = node;
    }
}

// assign address to literals when LTORG comes
void assignAddress(char *name, char *adr)
{
    literal *temp;
    temp = LITTAB_head;
    while (temp != NULL)
    {
        if (!strcmp(temp->name, name))
        {
            temp->adr = adr;
        }
        temp = temp->next;
    }
}

// check if the literal already exists to avoid duplicate copies
bool checkIfLiteralExist(char *name)
{
    literal *temp;
    temp = LITTAB_head;
    while (temp != NULL)
    {
        if (!strcmp(temp->name, name))
        {
            return true;
        }
        temp = temp->next;
    }
    return false;
}

// get the literal address from the literal name
char *getLiteralAddress(char *name)
{
    literal *temp;
    temp = LITTAB_head;
    while (temp != NULL)
    {
        if (!strcmp(temp->name, name))
        {
            return temp->adr;
        }
        temp = temp->next;
    }
}

// get the literal value from literal name
char *getLiteralValue(char *name)
{
    literal *temp;
    temp = LITTAB_head;
    while (temp != NULL)
    {
        if (!strcmp(temp->name, name))
        {
            return temp->value;
        }
        temp = temp->next;
    }
}
