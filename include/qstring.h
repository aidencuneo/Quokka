#ifndef _qstring
#define _qstring

#include "qdef.h"

//
/// Function declarations
//

// Special functions `__funcname__`
string __add_string__(string self, string other);

// Type functions `__typename__`
bool __bool_string__(string self);

// Regular functions `funcname`
string __lstrip_string__(string st);
string __rstrip_string__(string st);
string __strip_string__(string st);
string __upper_string__(string st);
string __lower_string__(string st);
string __slice_string__(string st, int start, int stop);
string __reorder_string__(string st, int step);
string __replace_string__(string st, string strep, string repwith);

//
/// Constructors
//

string StringFromString(string value)
{
    return value;
}

string StringFromCharPointer(char * value)
{
    string self;

    self.value = value;
    self.length = Integer(strlen(value));

    self.__add__ = __add_string__;

    self.__bool__ = __bool_string__;

    self.lstrip = __lstrip_string__;
    self.rstrip = __rstrip_string__;
    self.strip = __strip_string__;
    self.upper = __upper_string__;
    self.lower = __lower_string__;
    self.slice = __slice_string__;
    self.reorder = __reorder_string__;
    self.replace = __replace_string__;

    return self;
}

string StringFromInt(int value)
{
    char * newval = malloc(11);
    sprintf(newval, "%d", value);
    return StringFromCharPointer(newval);
}

string StringFromLong(long value)
{
    char * newval = malloc(21);
    sprintf(newval, "%ld", value);
    return StringFromCharPointer(newval);
}

string StringFromChar(char value)
{
    return StringFromCharPointer(&value);
}

string StringFromInteger(integer value)
{
    return StringFromLong(value.value);
}

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
/// Function definitions
//

// Special functions `__funcname__`
string __add_string__(string self, string other)
{
    char * x = (char *)malloc(self.length.value + other.length.value + 1);
    strcpy(x, self.value);
    strcat(x, other.value);

    string out = String(x);
    return out;
}

// Type functions `__typename__`
bool __bool_string__(string self)
{
    if (strlen(self.value))
        return Bool(1);
    return Bool(0);
}

// Regular functions `funcname`
string __lstrip_string__(string s)
{
    if (!s.value)
        return s;

    while (s.value[0] == ' ')
        s.value++;

    return s;
}

string __rstrip_string__(string st)
{
    char * s = (char *)malloc(st.length.value + 1);
    strcpy(s, st.value);

    if (!s)
        return st;

    size_t size;
    char * end;

    size = st.length.value;

    if (!size)
        return st;

    end = s + size - 1;
    while (end >= s && isspace(* end))
        end--;
    *(end + 1) = '\0';

    return String(s);
}

string __strip_string__(string st)
{
    char * s = (char *)malloc(st.length.value + 1);
    strcpy(s, st.value);

    if (!s)
        return st;

    size_t size;
    char * end;

    size = st.length.value;

    if (!size)
        return st;

    end = s + size - 1;
    while (end >= s && isspace(* end))
        end--;
    *(end + 1) = '\0';

    return __lstrip_string__(String(s));
}

string __upper_string__(string st)
{
    char * x = (char *)malloc(st.length.value * sizeof(char));

    for (int i = 0; i < st.length.value; i++)
    {
        char letter[2] = {0};
        letter[0] = toupper(st.value[i]);
        strcat(x, letter);
    }

    return String(x);
}

string __lower_string__(string st)
{
    char * x = (char *)malloc(st.length.value * sizeof(char));

    for (int i = 0; i < st.length.value; i++)
    {
        char letter[2] = {0};
        letter[0] = tolower(st.value[i]);
        strcat(x, letter);
    }

    return String(x);
}

string __slice_string__(string st, int start, int stop)
{
    char * x = (char *)malloc(st.length.value + 1 * sizeof(char));
    strcpy(x, st.value);

    if (!st.value || !st.length.value)
        return st;

    char * end;

    end = x + st.length.value - 1;
    end -= stop;
    *(end + 1) = '\0';

    x += start;

    return String(x);
}

string __reorder_string__(string st, int step)
{
    char * x = (char *)malloc(st.length.value * sizeof(char));

    if (step < 1) step = 1;
    if (step > st.length.value) step = st.length.value;
    for (int i = 0; i < st.length.value; i += step)
    {
        char * letter = malloc(2 * sizeof(char));
        letter[0] = st.value[i];
        strcat(x, letter);
    }

    return String(x);
}

string __replace_string__(string st, string strep, string repwith) {
    char * orig = st.value;
    char * rep = strep.value;
    char * with = repwith.value;

    char * result; // the return string
    char * ins;    // the next insert point
    char * tmp;    // varies
    int len_rep;  // length of rep (the string to remove)
    int len_with; // length of with (the string to replace rep with)
    int len_front; // distance between rep and end of last rep
    int count;    // number of replacements

    // sanity checks and initialization
    if (!orig || !rep)
        return String("");
    len_rep = strlen(rep);
    if (len_rep == 0)
        return String(""); // empty rep causes infinite loop during count
    if (!with)
        with = "";
    len_with = strlen(with);

    // count the number of replacements needed
    ins = orig;
    for (count = 0; (tmp = strstr(ins, rep)); ++count) {
        ins = tmp + len_rep;
    }

    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return String("");

    // first time through the loop, all the variable are set correctly
    // from here on,
    //    tmp points to the end of the result string
    //    ins points to the next occurrence of rep in orig
    //    orig points to the remainder of orig after "end of rep"
    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    }

    strcpy(tmp, orig);
    string out;
    out = String(result);
    return out;
}

#endif
