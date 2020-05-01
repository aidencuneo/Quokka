#ifndef _qbool
#define _qbool

#include "qdef.h"

//
/// Function declarations
//

// Special functions `__funcname__`
integer __add_bool__(bool self, bool other);
integer __sub_bool__(bool self, bool other);
integer __mul_bool__(bool self, bool other);
integer __div_bool__(bool self, bool other);

// Type functions `__typename__`
// (None yet)

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

    self.__add__ = __add_bool__;
    self.__sub__ = __sub_bool__;
    self.__mul__ = __mul_bool__;
    self.__div__ = __div_bool__;

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
integer __add_bool__(bool self, bool other)
{
    if (self.value.value)
        return Integer(1 + other.value.value);
    return other.value;
}

integer __sub_bool__(bool self, bool other)
{
    if (self.value.value)
        return Integer(1 - other.value.value);
    return Integer(-other.value.value);
}

integer __mul_bool__(bool self, bool other)
{
    if (self.value.value)
        return other.value;
    return Integer(0);
}

integer __div_bool__(bool self, bool other)
{
    if (!other.value.value)
        return Integer(0);
    return Integer(self.value.value / other.value.value);
}

// Type functions `__typename__`
// (None yet)

// Regular functions `funcname`
// (None yet)

#endif
