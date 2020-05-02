// qdef.h
// Defines the structs and types of the main datatypes in Quokka.

#ifndef _qdef
#define _qdef

//
/// Declarations
//

typedef struct __integer_Struct__ integer;
typedef struct __string_Struct__ string;
typedef struct __bool_Struct__ bool;


//
/// integer
//

struct __integer_Struct__
{
    long value;

    integer (*__add__)(integer self, integer other);
    integer (*__sub__)(integer self, integer other);
    integer (*__mul__)(integer self, integer other);
    integer (*__div__)(integer self, integer other);

    bool (*__bool__)(integer self);
    string (*__type__)(integer self);
};

integer IntegerFromInt(int value);
integer IntegerFromInteger(integer value);
integer IntegerFromLong(long value);
integer IntegerFromChar(char value);

#define Integer(value) _Generic((value),\
    int     : IntegerFromInt,\
    long    : IntegerFromLong,\
    char    : IntegerFromChar,\
    integer : IntegerFromInteger,\
    default : IntegerFromInt)(value)
    // I need to fill this in with every single C datatype

#define __integer_Constructor__(value) Integer(value)

//
/// string
//

struct __string_Struct__
{
    char * value;
    integer length;

    string (*__add__)(string self, string other);
    string (*__sub__)(string self, string other);
    string (*__mul__)(string self, string other);
    string (*__div__)(string self, string other);

    bool (*__bool__)(string self);
    string (*__type__)(string self);

    string (*lstrip)(string self);
    string (*rstrip)(string self);
    string (*strip)(string self);
    string (*upper)(string self);
    string (*lower)(string self);
    string (*slice)(string self, int start, int stop);
    string (*reorder)(string self, int step);
    string (*replace)(string self, string strep, string repwith);
};

string StringFromString(string value);
string StringFromCharPointer(char * value);
string StringFromInt(int value);
string StringFromLong(long value);
string StringFromChar(char value);
string StringFromInteger(integer value);

#define String(value) _Generic((value),\
    int     : StringFromInt,\
    long    : StringFromLong,\
    char    : StringFromChar,\
    char *  : StringFromCharPointer,\
    string  : StringFromString,\
    integer : StringFromInteger,\
    default : StringFromCharPointer)(value)

#define __string_Constructor__(value) String(value)

//
/// bool
//

struct __bool_Struct__
{
    integer value;

    integer (*__add__)(bool self, bool other);
    integer (*__sub__)(bool self, bool other);
    integer (*__mul__)(bool self, bool other);
    integer (*__div__)(bool self, bool other);

    bool (*__bool__)(bool self);
    string (*__type__)(bool self);
};

bool BoolFromInt(int value);
bool BoolFromBool(bool value);
bool BoolFromString(string value);
bool BoolFromInteger(integer value);

#define Bool(value) _Generic((value),\
    int     : BoolFromInt,\
    bool    : BoolFromBool,\
    string  : BoolFromString,\
    integer : BoolFromInteger,\
    default : BoolFromInt)(value)

#define __bool_Constructor__(value) Bool(value)

#endif
