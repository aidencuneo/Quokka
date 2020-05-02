#ifndef _qbool
#define _qbool

#include "qdef.h"

//
/// Function declarations
//

// Special functions `__funcname__`
integer __add__bool__(bool self, bool other);
integer __sub__bool__(bool self, bool other);
integer __mul__bool__(bool self, bool other);
integer __div__bool__(bool self, bool other);

// Type functions `__typename__`
bool __bool__bool__(bool self);
string __type__bool__(bool self);

// Regular functions `funcname`
// (None yet)

//
/// Constructors
//

bool BoolFromInt(int value)
{
    bool self;

    if (value)
        self.value = Integer(1);
    else self.value = Integer(0);

    self.__add__ = __add__bool__;
    self.__sub__ = __sub__bool__;
    self.__mul__ = __mul__bool__;
    self.__div__ = __div__bool__;

    self.__bool__ = __bool__bool__;
    self.__type__ = __type__bool__;

    return self;
}

bool BoolFromBool(bool value)
{
    return value;
}

bool BoolFromString(string value)
{
    if (strlen(value.value))
        return BoolFromInt(1);
    return BoolFromInt(0);
}

bool BoolFromInteger(integer value)
{
    return BoolFromInt(value.value);
}

#define Bool(value) _Generic((value),\
    int     : BoolFromInt,\
    bool    : BoolFromBool,\
    string  : BoolFromString,\
    integer : BoolFromInteger,\
    default : BoolFromInt)(value)

#define __bool_Constructor__(value) Bool(value)

//
/// Function definitions
//

// Special functions `__funcname__`
integer __add__bool__(bool self, bool other)
{
    if (self.value.value)
        return Integer(1 + other.value.value);
    return other.value;
}

integer __sub__bool__(bool self, bool other)
{
    if (self.value.value)
        return Integer(1 - other.value.value);
    return Integer(-other.value.value);
}

integer __mul__bool__(bool self, bool other)
{
    if (self.value.value)
        return other.value;
    return Integer(0);
}

integer __div__bool__(bool self, bool other)
{
    if (!other.value.value)
        return Integer(0);
    return Integer(self.value.value / other.value.value);
}

// Type functions `__typename__`
bool __bool__bool__(bool self)
{
    return self;
}

string __type__bool__(bool self)
{
    return String("bool");
}

// Regular functions `funcname`
// (None yet)

#endif
