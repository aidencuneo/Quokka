// Retrieve __add__ method from an Object
void * objOperAdd(Object * obj)
{
    if (!strcmp(obj->name, "int"))
        return &__add___int;

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

// Retrieve __eq__ method from an Object
void * objOperEq(Object * obj)
{
    if (!strcmp(obj->name, "int"))
        return &__eq___int;

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
