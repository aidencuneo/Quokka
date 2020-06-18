// Retrieve __pos__ method from an Object
void * objOperPos(Object * obj)
{
    if (!strcmp(obj->name, "int"))
        return &__pos___int;
    if (!strcmp(obj->name, "long"))
        return &__pos___long;

    void * func = objectGetAttr(obj, "__pos__");

    if (func == NULL)
    {
        char * err = malloc(6 + strlen(obj->name) + 38 + 1);
        strcpy(err, "type '");
        strcat(err, obj->name);
        strcat(err, "' does not have a method for unary '+'");
        error(err, line_num);
    }

    return func;
}

// Retrieve __neg__ method from an Object
void * objOperNeg(Object * obj)
{
    if (!strcmp(obj->name, "int"))
        return &__neg___int;
    if (!strcmp(obj->name, "long"))
        return &__neg___long;

    void * func = objectGetAttr(obj, "__neg__");

    if (func == NULL)
    {
        char * err = malloc(6 + strlen(obj->name) + 38 + 1);
        strcpy(err, "type '");
        strcat(err, obj->name);
        strcat(err, "' does not have a method for unary '-'");
        error(err, line_num);
    }

    return func;
}

// Retrieve __add__ method from an Object
void * objOperAdd(Object * obj)
{
    if (!strcmp(obj->name, "int"))
        return &__add___int;
    if (!strcmp(obj->name, "long"))
        return &__add___long;
    if (!strcmp(obj->name, "string"))
        return &__add___string;
    if (!strcmp(obj->name, "list"))
        return &__add___list;

    void * func = objectGetAttr(obj, "__add__");

    if (func == NULL)
    {
        char * err = malloc(6 + strlen(obj->name) + 37 + 1);
        strcpy(err, "type '");
        strcat(err, obj->name);
        strcat(err, "' does not have a method for addition");
        error(err, line_num);
    }

    return func;
}

// Retrieve __sub__ method from an Object
void * objOperSub(Object * obj)
{
    if (!strcmp(obj->name, "int"))
        return &__sub___int;
    if (!strcmp(obj->name, "long"))
        return &__sub___long;

    void * func = objectGetAttr(obj, "__sub__");

    if (func == NULL)
    {
        char * err = malloc(6 + strlen(obj->name) + 37 + 1);
        strcpy(err, "type '");
        strcat(err, obj->name);
        strcat(err, "' does not have a method for subtraction");
        error(err, line_num);
    }

    return func;
}

// Retrieve __mul__ method from an Object
void * objOperMul(Object * obj)
{
    if (!strcmp(obj->name, "int"))
        return &__mul___int;
    if (!strcmp(obj->name, "long"))
        return &__mul___long;
    if (!strcmp(obj->name, "string"))
        return &__mul___string;

    void * func = objectGetAttr(obj, "__mul__");

    if (func == NULL)
    {
        char * err = malloc(6 + strlen(obj->name) + 43 + 1);
        strcpy(err, "type '");
        strcat(err, obj->name);
        strcat(err, "' does not have a method for multiplication");
        error(err, line_num);
    }

    return func;
}

// Retrieve __div__ method from an Object
void * objOperDiv(Object * obj)
{
    if (!strcmp(obj->name, "int"))
        return &__div___int;
    if (!strcmp(obj->name, "long"))
        return &__div___long;

    void * func = objectGetAttr(obj, "__div__");

    if (func == NULL)
    {
        char * err = malloc(6 + strlen(obj->name) + 37 + 1);
        strcpy(err, "type '");
        strcat(err, obj->name);
        strcat(err, "' does not have a method for division");
        error(err, line_num);
    }

    return func;
}

// Retrieve __pow__ method from an Object
void * objOperPow(Object * obj)
{
    if (!strcmp(obj->name, "int"))
        return &__pow___int;
    if (!strcmp(obj->name, "long"))
        return &__pow___long;

    void * func = objectGetAttr(obj, "__pow__");

    if (func == NULL)
    {
        char * err = malloc(6 + strlen(obj->name) + 36 + 1);
        strcpy(err, "type '");
        strcat(err, obj->name);
        strcat(err, "' does not have a method for indices");
        error(err, line_num);
    }

    return func;
}

// Retrieve __inadd__ method from an Object (no error raising)
void * objOperInadd(Object * obj)
{
    if (!strcmp(obj->name, "string"))
        return &__inadd___string;
    if (!strcmp(obj->name, "list"))
        return &__inadd___list;

    return objectGetAttr(obj, "__inadd__");
}

// Retrieve __insub__ method from an Object (no error raising)
void * objOperInsub(Object * obj)
{
    return objectGetAttr(obj, "__insub__");
}

// Retrieve __inmul__ method from an Object (no error raising)
void * objOperInmul(Object * obj)
{
    return objectGetAttr(obj, "__inmul__");
}

// Retrieve __indiv__ method from an Object (no error raising)
void * objOperIndiv(Object * obj)
{
    return objectGetAttr(obj, "__indiv__");
}

// Retrieve __eq__ method from an Object
void * objOperEq(Object * obj)
{
    if (!strcmp(obj->name, "int"))
        return &__eq___int;
    if (!strcmp(obj->name, "long"))
        return &__eq___long;
    if (!strcmp(obj->name, "string"))
        return &__eq___string;

    void * func = objectGetAttr(obj, "__eq__");

    if (func == NULL)
    {
        char * err = malloc(6 + strlen(obj->name) + 53 + 1);
        strcpy(err, "type '");
        strcat(err, obj->name);
        strcat(err, "' does not have a method for equality (==) comparison");
        error(err, line_num);
    }

    return func;
}

// Retrieve __lt__ method from an Object
void * objOperLt(Object * obj)
{
    if (!strcmp(obj->name, "int"))
        return &__lt___int;
    if (!strcmp(obj->name, "long"))
        return &__lt___long;

    void * func = objectGetAttr(obj, "__lt__");

    if (func == NULL)
    {
        char * err = malloc(6 + strlen(obj->name) + 53 + 1);
        strcpy(err, "type '");
        strcat(err, obj->name);
        strcat(err, "' does not have a method for less than (<) comparison");
        error(err, line_num);
    }

    return func;
}

// Retrieve __gt__ method from an Object
void * objOperGt(Object * obj)
{
    if (!strcmp(obj->name, "int"))
        return &__gt___int;
    if (!strcmp(obj->name, "long"))
        return &__gt___long;

    void * func = objectGetAttr(obj, "__gt__");

    if (func == NULL)
    {
        char * err = malloc(6 + strlen(obj->name) + 56 + 1);
        strcpy(err, "type '");
        strcat(err, obj->name);
        strcat(err, "' does not have a method for greater than (>) comparison");
        error(err, line_num);
    }

    return func;
}

// Retrieve __le__ method from an Object
void * objOperLe(Object * obj)
{
    if (!strcmp(obj->name, "int"))
        return &__le___int;
    if (!strcmp(obj->name, "long"))
        return &__le___long;

    void * func = objectGetAttr(obj, "__le__");

    if (func == NULL)
    {
        char * err = malloc(6 + strlen(obj->name) + 66 + 1);
        strcpy(err, "type '");
        strcat(err, obj->name);
        strcat(err, "' does not have a method for less than or equal to (<=) comparison");
        error(err, line_num);
    }

    return func;
}

// Retrieve __ge__ method from an Object
void * objOperGe(Object * obj)
{
    if (!strcmp(obj->name, "int"))
        return &__ge___int;
    if (!strcmp(obj->name, "long"))
        return &__ge___long;

    void * func = objectGetAttr(obj, "__ge__");

    if (func == NULL)
    {
        char * err = malloc(6 + strlen(obj->name) + 69 + 1);
        strcpy(err, "type '");
        strcat(err, obj->name);
        strcat(err, "' does not have a method for greater than or equal to (>=) comparison");
        error(err, line_num);
    }

    return func;
}

// Retrieve __index__ method from an Object
void * objOperIndex(Object * obj)
{
    if (!strcmp(obj->name, "string"))
        return &__index___string;
    if (!strcmp(obj->name, "list"))
        return &__index___list;

    void * func = objectGetAttr(obj, "__index__");

    if (func == NULL)
    {
        char * err = malloc(6 + strlen(obj->name) + 45 + 1);
        strcpy(err, "type '");
        strcat(err, obj->name);
        strcat(err, "' does not have a method for index retrieving");
        error(err, line_num);
    }

    return func;
}

// Retrieve __copy__ method from an Object
void * objOperCopy(Object * obj)
{
    if (!strcmp(obj->name, "string"))
        return &__copy___string;
    if (!strcmp(obj->name, "list"))
        return &__copy___list;

    void * func = objectGetAttr(obj, "__copy__");

    if (func == NULL)
    {
        char * err = malloc(6 + strlen(obj->name) + 36 + 1);
        strcpy(err, "type '");
        strcat(err, obj->name);
        strcat(err, "' does not have a method for copying");
        error(err, line_num);
    }

    return func;
}

// Retrieve __setindex__ method from an Object
void * objOperSetindex(Object * obj)
{
    if (!strcmp(obj->name, "list"))
        return &__setindex___list;

    void * func = objectGetAttr(obj, "__setindex__");

    if (func == NULL)
    {
        char * err = malloc(6 + strlen(obj->name) + 44 + 1);
        strcpy(err, "type '");
        strcat(err, obj->name);
        strcat(err, "' does not have a method for index assigning");
        error(err, line_num);
    }

    return func;
}

//
/// Regular methods
/// (No error raising)
//

// Retrieve __bool__ method from an Object
void * objOperBool(Object * obj)
{
    if (!strcmp(obj->name, "null"))
        return &__bool___null;
    if (!strcmp(obj->name, "int"))
        return &__bool___int;
    if (!strcmp(obj->name, "long"))
        return &__bool___long;
    if (!strcmp(obj->name, "string"))
        return &__bool___string;
    if (!strcmp(obj->name, "list"))
        return &__bool___list;

    return objectGetAttr(obj, "__bool__");
}

// Retrieve __int__ method from an Object
void * objOperInt(Object * obj)
{
    if (!strcmp(obj->name, "long"))
        return &__int___long;
    if (!strcmp(obj->name, "string"))
        return &__int___string;

    return objectGetAttr(obj, "__int__");
}


// Retrieve __long__ method from an Object
void * objOperLong(Object * obj)
{
    if (!strcmp(obj->name, "int"))
        return &__long___int;
    if (!strcmp(obj->name, "string"))
        return &__long___string;

    return objectGetAttr(obj, "__long__");
}

// Retrieve __string__ method from an Object
void * objOperString(Object * obj)
{
    if (!strcmp(obj->name, "string"))
        return &__string___string;

    return objectGetAttr(obj, "__string__");
}

// Retrieve __disp__ method from an Object
void * objOperDisp(Object * obj)
{
    if (!strcmp(obj->name, "null"))
        return &__disp___null;
    if (!strcmp(obj->name, "int"))
        return &__disp___int;
    if (!strcmp(obj->name, "long"))
        return &__disp___long;
    if (!strcmp(obj->name, "string"))
        return &__disp___string;
    if (!strcmp(obj->name, "list"))
        return &__disp___list;

    return objectGetAttr(obj, "__disp__");
}

// Retrieve __len__ method from an Object
void * objOperLen(Object * obj)
{
    if (!strcmp(obj->name, "list"))
        return &__len___list;
    if (!strcmp(obj->name, "string"))
        return &__len___string;

    return objectGetAttr(obj, "__len__");
}

// Retrieve __sizeof__ method from an Object
void * objOperSizeof(Object * obj)
{
    if (!strcmp(obj->name, "null"))
        return &__sizeof___null;
    if (!strcmp(obj->name, "int"))
        return &__sizeof___int;
    if (!strcmp(obj->name, "long"))
        return &__sizeof___long;
    if (!strcmp(obj->name, "string"))
        return &__sizeof___string;
    if (!strcmp(obj->name, "list"))
        return &__sizeof___list;

    return objectGetAttr(obj, "__sizeof__");
}
