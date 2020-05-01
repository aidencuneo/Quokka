#ifndef _qinteger
#define _qinteger

#include "qdef.h"

//
/// Function declarations
//

// Special functions `__funcname__`
integer __add_integer__(integer self, integer other);
integer __sub_integer__(integer self, integer other);
integer __mul_integer__(integer self, integer other);
integer __div_integer__(integer self, integer other);

// Type functions `__typename__`
bool __bool_integer__(integer self);

// Regular functions `funcname`
// (None yet)

//
/// Constructors
//

integer IntegerFromInt(int value)
{
    integer self;

    self.value = value;

    self.__add__ = __add_integer__;
    self.__sub__ = __sub_integer__;
    self.__mul__ = __mul_integer__;
    self.__div__ = __div_integer__;

    self.__bool__ = __bool_integer__;

    return self;
}

integer IntegerFromInteger(integer value)
{
    return value;
}

integer IntegerFromLong(long value)
{
    return IntegerFromInt((int)value);
}

integer IntegerFromChar(char value)
{
    return IntegerFromInt((int)value);
}

#define Integer(value) _Generic((value),\
    int     : IntegerFromInt,\
    long    : IntegerFromLong,\
    char    : IntegerFromChar,\
    integer : IntegerFromInteger,\
    default : IntegerFromInt)(value)
    // I need to fill this in with every single C datatype

#define __integer_Constructor__(value) Integer(value)

//
/// Function definitions
//

// Special functions `__funcname__`
integer __add_integer__(integer self, integer other)
{
    return Integer(self.value + other.value);
}

integer __sub_integer__(integer self, integer other)
{
    return Integer(self.value - other.value);
}

integer __mul_integer__(integer self, integer other)
{
    return Integer(self.value * other.value);
}

integer __div_integer__(integer self, integer other)
{
    return Integer(self.value / other.value);
}

// Type functions `__typename__`
bool __bool_integer__(integer self)
{
    if (self.value)
        return Bool(1);
    return Bool(0);
}

// Regular functions `funcname`
// (None yet)

#endif
