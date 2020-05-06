// qdef.h
// Defines the structs and types of the main datatypes in Quokka.

#ifndef _qdef
#define _qdef

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

#endif
