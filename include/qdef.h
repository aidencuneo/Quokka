// qdef.h
// Defines the structs and types of the main datatypes in Quokka.

#ifndef _qdef
#define _qdef

// Interpreter global variables
int bc_line;
int bc_line_count;

int * makeIntPtrFromStr(char * st)
{
    int * n = malloc(sizeof(int));
    n[0] = strtol(st, (char **)NULL, 10);
    return n;
}

int * makeIntPtr(int i)
{
    int * n = malloc(sizeof(int));
    n[0] = i;
    return n;
}

//
/// Object & Varlist
//

typedef struct __Object_Struct__ Object;
struct __Object_Struct__
{
    char * name;

    char ** names;
    void ** values;
    int value_count;
};

typedef struct __Varlist_Struct__ Varlist;
struct __Varlist_Struct__
{
    int count;

    char ** names;
    Object * values;
};

// Function declarations
void interp_init();
void quokka_interpret_line(char * linetext);
void quokka_interpret_line_tokens(char ** line);
void quokka_interpret_tokens(char ** tokens);
void quokka_interpret(char * bytecode);

void resetStack();
Object emptyObject(char * name);
Object makeObject(char * name, void * value);
Object addObjectValue(Object obj, char * name, void * value);
int objectHasAttr(Object obj, char * name);
void * objectGetAttr(Object obj, char * name);
void freeObject(Object obj);
void pushTop(Object obj);
Object popTop();
void addVar(char * name, Object obj);
void addGVar(char * name, Object obj);
Object getVar(char * name);
int getGVarIndex(char * name);
int getLVarIndex(char * name);
int getVarIndex(char * name);
Object * makeArglist(Object obj);

//
/// Function
//

typedef struct __Function_Struct__ Function;
struct __Function_Struct__
{
    Object (*func)(Object * argv);
    int * is_function;
    int * argc;
};

// Function declarations
void addFunction(Function funcobj);

//
/// Quokka datatypes
//

// int
Object makeInteger(int * value);

// string
Object makeString(char * value);

// list
Object makeList(int length, Object * value, int flipped);

// null
Object makeNull();

// function
Object makeFunction(Object (*func)(Object * argv), int argc);
Object makeMethod(Object (*func)(Object * argv), int * argc);

#endif
