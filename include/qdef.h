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

long long * makeLLPtrFromStr(char * st)
{
    long long * n = malloc(sizeof(long long));
    n[0] = strtoll(st, (char **)NULL, 10);
    return n;
}

long long * makeLLPtr(long long i)
{
    long long * n = malloc(sizeof(long long));
    n[0] = i;
    return n;
}

//
/// Object * & Varlist
//

typedef struct __Object_Struct__ Object;
struct __Object_Struct__
{
    char * name; // Object * type name

    // Attributes
    // (list of names, list of values, and value count)
    char ** names;
    void ** values;
    int value_count;

    int refs; // References to this Object
};

typedef struct __Varlist_Struct__ Varlist;
struct __Varlist_Struct__
{
    int count;
    int offset;
    int alloc;

    char ** names;
    Object ** values;
};

// Function declarations
void interp_init();
void quokka_interpret_line(char * linetext);
void quokka_interpret_line_tokens(char ** line);
void quokka_interpret_tokens(char *** tokens);
void quokka_interpret(char * bytecode);
void _quokka_interpret(char * bytecode);

void freeRecursive(Object ** ptr, int size);
void resetStack();
Object * emptyObject(char * name);
Object * makeObject(char * name, void * value);
void objectSummary(Object * obj);
Object * addObjectValue(Object * obj, char * name, void * value);
int objectHasAttr(Object * obj, char * name);
void * objectGetAttr(Object * obj, char * name);
void freeObjectR(Object * obj);
void freeObject(Object * obj);

void objUnref(Object * obj);
void objDeref(Object * obj);

void pushTop(Object * obj);
Object * popTop();
void pushTrash(void * ptr);

void addGVar(char * name, Object * obj);
void addVar(char * name, Object * obj);

Object * getGVar(char * name);
Object * getGVarSilent(char * name);
Object * getLVar(char * name);
Object * getLVarSilent(char * name);
Object * getVar(char * name);
Object * getVarSilent(char * name);

int getGVarIndex(char * name);
int getLVarIndex(char * name);
int getVarIndex(char * name);

Object ** makeArglist(Object * obj);
Object ** makeDoubleArglist(Object * first, Object * secnd);

//
/// Function
//

typedef struct __Function_Struct__ Function;
struct __Function_Struct__
{
    char ** f_code;
    int * argc;
};

// Function declarations
void addFunction(Function funcobj);

//
/// Quokka datatypes (alphabetical order)
//

// cfunction
Object * makeCFunction(void * function_pointer, int argmin, int argmax);

// cmethod
Object * makeCMethod(Object * parent, void * method_pointer, int argmin, int argmax);

// file
Object * makeFile(char * path, char * mode);

// function
Object * makeFunction(char * filepath, char ** bytecode, int argmin, int argmax);

// int
Object * makeInt(int * value, int digits, int mult);
Object * makeIntRaw(int * value, int digits, int mult);

int intsign(int size);
void qint_normalise(Object * obj);

Object * qint_addition(Object * a, Object * b);
Object * qint_subtraction(Object * a, Object * b);
Object * qint_division(Object * a, Object * b);

Object * qint_mul1(Object * a, long n);
Object * qint_muladd1(Object * obj, long n, int extra);
Object * qint_from_string(char * str, int base);
Object * qint_divrem1(Object * obj, int n, long * remptr);
int qint_divrem(Object * a, Object * b, Object ** divptr, Object ** remptr);
int qint_divmod(Object * a, Object * b, Object ** divptr, Object ** modptr);
char * string_from_qint(Object * obj, int base);

// list
Object * makeList(int length, Object ** value, int flipped);

// long
Object * makeLong(long long * value, int is_malloc_ptr);

// method
// Not implemented

// module
Object * makeModule(char * name, int item_count);

// null
Object * makeNull();
Object * makeNullRaw();

// string
Object * makeString(char * value, int is_malloc_ptr);
Object * makeStringRaw(char * value, int is_malloc_ptr);

//
/// Operation functions
//

void * objOperBool(Object * obj);
void * objOperInt(Object * obj);
void * objOperLong(Object * obj);
void * objOperString(Object * obj);
void * objOperDisp(Object * obj);
void * objOperLen(Object * obj);
void * objOperSizeof(Object * obj);
void * objOperFree(Object * obj);

#endif
