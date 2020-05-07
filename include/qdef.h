// qdef.h
// Defines the structs and types of the main datatypes in Quokka.

#ifndef _qdef
#define _qdef

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
/// string
//

typedef struct __string_Struct__ string;
struct __string_Struct__
{
    char * value;
    int length;

    string (*__add__)(string self, string other);
    string (*__sub__)(string self, string other);
    string (*__mul__)(string self, string other);
    string (*__div__)(string self, string other);

    string (*__type__)(string self);

    string (*lstrip)(string self);
    string (*rstrip)(string self);
    string (*strip)(string self);
    string (*upper)(string self);
    string (*lower)(string self);
    string (*slice)(string st, int start, int stop);
    string (*reorder)(string self, int step);
    string (*replace)(string self, string strep, string repwith);
};

string StringFromString(string value);
string StringFromCharPointer(char * value);
string StringFromInt(int value);
string StringFromLong(long value);
string StringFromChar(char value);

#define String(value) _Generic((value),\
    int     : StringFromInt,\
    long    : StringFromLong,\
    char    : StringFromChar,\
    char *  : StringFromCharPointer,\
    string  : StringFromString,\
    default : StringFromCharPointer)(value)

#define __string_Constructor__(value) String(value)

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
Object emptyObject(char * name);
Object makeObject(char * name, void * value);
Object addObjectValue(Object obj, char * name, void * value);
int objectHasAttr(Object obj, char * name);
void * objectGetAttr(Object obj, char * name);
void pushTop(Object obj);
Object popTop();
void addVar(char * name, Object obj);
Object getVar(char * name);
int getVarIndex(char * name);

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

// bool
Object makeBool(short * value);

// int
Object makeInteger(int * value);

// string
Object makeString(char * value);

// function
Object makeFunction(Object (*func)(Object * argv), int argc);
Object makeMethod(Object (*func)(Object * argv), int * argc);

#endif
