Object * __add___int(int argc, Object ** argv)
{
    if (!strcmp(argv[1]->name, "int"))
    {
        int * first = objectGetAttr(argv[0], "value");
        int * secnd = objectGetAttr(argv[1], "value");

        int * third = makeIntPtr(first[0] + secnd[0]);

        return makeInt(third, 1);
    }
    else if (!strcmp(argv[1]->name, "long"))
    {
        int * first = objectGetAttr(argv[0], "value");
        long long * secnd = objectGetAttr(argv[1], "value");

        long long * third = makeLLPtr(first[0] + secnd[0]);

        return makeLong(third, 1);
    }

    char * err = malloc(17 + strlen(argv[1]->name) + 30 + 1);
    strcpy(err, "types 'int' and '");
    strcat(err, argv[1]->name);
    strcat(err, "' are invalid operands for '+'");
    error(err, line_num);

    return makeNull();
}

Object * __sub___int(int argc, Object ** argv)
{
    if (!strcmp(argv[1]->name, "int"))
    {
        int * first = objectGetAttr(argv[0], "value");
        int * secnd = objectGetAttr(argv[1], "value");

        int * third = makeIntPtr(first[0] - secnd[0]);

        return makeInt(third, 1);
    }
    else if (!strcmp(argv[1]->name, "long"))
    {
        int * first = objectGetAttr(argv[0], "value");
        long long * secnd = objectGetAttr(argv[1], "value");

        long long * third = makeLLPtr(first[0] - secnd[0]);

        return makeLong(third, 1);
    }

    char * err = malloc(17 + strlen(argv[1]->name) + 30 + 1);
    strcpy(err, "types 'int' and '");
    strcat(err, argv[1]->name);
    strcat(err, "' are invalid operands for '-'");
    error(err, line_num);

    return makeNull();
}

Object * __mul___int(int argc, Object ** argv)
{
    if (!strcmp(argv[1]->name, "int"))
    {
        int * first = objectGetAttr(argv[0], "value");
        int * secnd = objectGetAttr(argv[1], "value");

        int * third = makeIntPtr(first[0] * secnd[0]);

        return makeInt(third, 1);
    }
    else if (!strcmp(argv[1]->name, "long"))
    {
        int * first = objectGetAttr(argv[0], "value");
        long long * secnd = objectGetAttr(argv[1], "value");

        long long * third = makeLLPtr(first[0] * secnd[0]);

        return makeLong(third, 1);
    }

    char * err = malloc(17 + strlen(argv[1]->name) + 30 + 1);
    strcpy(err, "types 'int' and '");
    strcat(err, argv[1]->name);
    strcat(err, "' are invalid operands for '*'");
    error(err, line_num);

    return makeNull();
}

Object * __div___int(int argc, Object ** argv)
{
    if (!strcmp(argv[1]->name, "int"))
    {
        int * first = objectGetAttr(argv[0], "value");
        int * secnd = objectGetAttr(argv[1], "value");

        if (!secnd[0])
            return makeInt(&falsePtr, 0);

        int * third = makeIntPtr(first[0] / secnd[0]);

        return makeInt(third, 1);
    }
    else if (!strcmp(argv[1]->name, "long"))
    {
        int * first = objectGetAttr(argv[0], "value");
        long long * secnd = objectGetAttr(argv[1], "value");

        if (!secnd[0])
            return makeInt(&falsePtr, 0);

        long long * third = makeLLPtr(first[0] / secnd[0]);

        return makeLong(third, 1);
    }

    char * err = malloc(17 + strlen(argv[1]->name) + 30 + 1);
    strcpy(err, "types 'int' and '");
    strcat(err, argv[1]->name);
    strcat(err, "' are invalid operands for '/'");
    error(err, line_num);

    return makeNull();
}

Object * __pow___int(int argc, Object ** argv)
{
    if (!strcmp(argv[1]->name, "int"))
    {
        int * first = objectGetAttr(argv[0], "value");
        int * secnd = objectGetAttr(argv[1], "value");

        int * third = makeIntPtr(ipowMath(first[0], secnd[0]));

        return makeInt(third, 1);
    }
    else if (!strcmp(argv[1]->name, "long"))
    {
        int * first = objectGetAttr(argv[0], "value");
        long long * secnd = objectGetAttr(argv[1], "value");

        long long * third = makeLLPtr(lpowMath(first[0], secnd[0]));

        return makeLong(third, 1);
    }

    char * err = malloc(17 + strlen(argv[1]->name) + 31 + 1);
    strcpy(err, "types 'int' and '");
    strcat(err, argv[1]->name);
    strcat(err, "' are invalid operands for '**'");
    error(err, line_num);

    return makeNull();
}

Object * __eq___int(int argc, Object ** argv)
{
    if (!strcmp(argv[1]->name, "int"))
    {
        int * first = objectGetAttr(argv[0], "value");
        int * secnd = objectGetAttr(argv[1], "value");

        if (first[0] == secnd[0])
            return makeInt(&truePtr, 0);
        return makeInt(&falsePtr, 0);
    }
    else if (!strcmp(argv[1]->name, "long"))
    {
        int * first = objectGetAttr(argv[0], "value");
        long long * secnd = objectGetAttr(argv[1], "value");

        if (first[0] == secnd[0])
            return makeInt(&truePtr, 0);
        return makeInt(&falsePtr, 0);
    }

    char * err = malloc(17 + strlen(argv[1]->name) + 31 + 1);
    strcpy(err, "types 'int' and '");
    strcat(err, argv[1]->name);
    strcat(err, "' can not be compared with '=='");
    error(err, line_num);

    return makeNull();
}

Object * __lt___int(int argc, Object ** argv)
{
    if (!strcmp(argv[1]->name, "int"))
    {
        int * first = objectGetAttr(argv[0], "value");
        int * secnd = objectGetAttr(argv[1], "value");

        if (first[0] < secnd[0])
            return makeInt(&truePtr, 0);
        return makeInt(&falsePtr, 0);
    }
    else if (!strcmp(argv[1]->name, "long"))
    {
        int * first = objectGetAttr(argv[0], "value");
        long long * secnd = objectGetAttr(argv[1], "value");

        if (first[0] < secnd[0])
            return makeInt(&truePtr, 0);
        return makeInt(&falsePtr, 0);
    }

    char * err = malloc(17 + strlen(argv[1]->name) + 30 + 1);
    strcpy(err, "types 'int' and '");
    strcat(err, argv[1]->name);
    strcat(err, "' can not be compared with '<'");
    error(err, line_num);

    return makeNull();
}

Object * __le___int(int argc, Object ** argv)
{
    if (!strcmp(argv[1]->name, "int"))
    {
        int * first = objectGetAttr(argv[0], "value");
        int * secnd = objectGetAttr(argv[1], "value");

        if (first[0] <= secnd[0])
            return makeInt(&truePtr, 0);
        return makeInt(&falsePtr, 0);
    }
    else if (!strcmp(argv[1]->name, "long"))
    {
        int * first = objectGetAttr(argv[0], "value");
        long long * secnd = objectGetAttr(argv[1], "value");

        if (first[0] <= secnd[0])
            return makeInt(&truePtr, 0);
        return makeInt(&falsePtr, 0);
    }

    char * err = malloc(17 + strlen(argv[1]->name) + 31 + 1);
    strcpy(err, "types 'int' and '");
    strcat(err, argv[1]->name);
    strcat(err, "' can not be compared with '<='");
    error(err, line_num);

    return makeNull();
}

Object * __gt___int(int argc, Object ** argv)
{
    if (!strcmp(argv[1]->name, "int"))
    {
        int * first = objectGetAttr(argv[0], "value");
        int * secnd = objectGetAttr(argv[1], "value");

        if (first[0] > secnd[0])
            return makeInt(&truePtr, 0);
        return makeInt(&falsePtr, 0);
    }
    else if (!strcmp(argv[1]->name, "long"))
    {
        int * first = objectGetAttr(argv[0], "value");
        long long * secnd = objectGetAttr(argv[1], "value");

        if (first[0] > secnd[0])
            return makeInt(&truePtr, 0);
        return makeInt(&falsePtr, 0);
    }

    char * err = malloc(17 + strlen(argv[1]->name) + 30 + 1);
    strcpy(err, "types 'int' and '");
    strcat(err, argv[1]->name);
    strcat(err, "' can not be compared with '>'");
    error(err, line_num);

    return makeNull();
}

Object * __ge___int(int argc, Object ** argv)
{
    if (!strcmp(argv[1]->name, "int"))
    {
        int * first = objectGetAttr(argv[0], "value");
        int * secnd = objectGetAttr(argv[1], "value");

        if (first[0] >= secnd[0])
            return makeInt(&truePtr, 0);
        return makeInt(&falsePtr, 0);
    }
    else if (!strcmp(argv[1]->name, "long"))
    {
        int * first = objectGetAttr(argv[0], "value");
        long long * secnd = objectGetAttr(argv[1], "value");

        if (first[0] >= secnd[0])
            return makeInt(&truePtr, 0);
        return makeInt(&falsePtr, 0);
    }

    char * err = malloc(17 + strlen(argv[1]->name) + 31 + 1);
    strcpy(err, "types 'int' and '");
    strcat(err, argv[1]->name);
    strcat(err, "' can not be compared with '>='");
    error(err, line_num);

    return makeNull();
}

Object * __sizeof___int(int argc, Object ** argv)
{
    int * thisvalue = objectGetAttr(argv[0], "value");

    int * size = makeIntPtr(sizeof(argv[0]) + sizeof(thisvalue[0]));

    return makeInt(size, 1);
}

Object * __pos___int(int argc, Object ** argv)
{
    int * first = objectGetAttr(argv[0], "value");

    int * ret = makeIntPtr(first[0]);

    return makeInt(ret, 1);
}

Object * __neg___int(int argc, Object ** argv)
{
    int * first = objectGetAttr(argv[0], "value");

    int * ret = makeIntPtr(-first[0]);

    return makeInt(ret, 1);
}

Object * __disp___int(int argc, Object ** argv)
{
    int * thisvalue = objectGetAttr(argv[0], "value");

    char * tostr = intToStr(thisvalue[0]);

    return makeString(tostr, 1);
}

Object * __bool___int(int argc, Object ** argv)
{
    int * thisvalue = objectGetAttr(argv[0], "value");

    if (thisvalue[0])
        return makeInt(&truePtr, 0);
    return makeInt(&falsePtr, 0);
}

Object * __int___int(int argc, Object ** argv)
{
    int * first = objectGetAttr(argv[0], "value");

    return makeInt(makeIntPtr(first[0]), 1);
}

Object * __long___int(int argc, Object ** argv)
{
    int * first = objectGetAttr(argv[0], "value");

    return makeLong(makeLLPtr(first[0]), 1);
}

Object * __free___int(int argc, Object ** argv)
{
    int * thisvalue = objectGetAttr(argv[0], "value");
    free(thisvalue);

    return makeNull();
}

Object * makeInt(int * value, int is_malloc_ptr)
{
    // If value == 0, return the already made constant for 0
    if (!value[0])
    {
        if (is_malloc_ptr)
            free(value);
        return constants[0];
    }
    // If value == 1, return the already made constant for 1
    if (value[0] == 1)
    {
        if (is_malloc_ptr)
            free(value);
        return constants[1];
    }

    return makeIntRaw(value, is_malloc_ptr);
}

Object * makeIntRaw(int * value, int is_malloc_ptr)
{
    Object * self = objectPointer();

    self->name = "int";

    // 35 to 36 Attributes
    if (is_malloc_ptr)
    {
        self->names = malloc(36 * sizeof(char *));
        self->values = malloc(36 * sizeof(void *));
    }
    else
    {
        self->names = malloc(35 * sizeof(char *));
        self->values = malloc(35 * sizeof(void *));
    }
    self->value_count = 0;

    self = objectAddAttr(self, "value", value);

    // Two argument methods

    // __add__
    self = objectAddAttr(self, "__add__", &__add___int);
    self = objectAddAttr(self, "__add__argc", &twoArgc);

    // __sub__
    self = objectAddAttr(self, "__sub__", &__sub___int);
    self = objectAddAttr(self, "__sub__argc", &twoArgc);

    // __mul__
    self = objectAddAttr(self, "__mul__", &__mul___int);
    self = objectAddAttr(self, "__mul__argc", &twoArgc);

    // __div__
    self = objectAddAttr(self, "__div__", &__div___int);
    self = objectAddAttr(self, "__div__argc", &twoArgc);

    // __pow__
    self = objectAddAttr(self, "__pow__", &__pow___int);
    self = objectAddAttr(self, "__pow__argc", &twoArgc);

    // __eq__
    self = objectAddAttr(self, "__eq__", &__eq___int);
    self = objectAddAttr(self, "__eq__argc", &twoArgc);

    // __lt__
    self = objectAddAttr(self, "__lt__", &__lt___int);
    self = objectAddAttr(self, "__lt__argc", &twoArgc);

    // __le__
    self = objectAddAttr(self, "__le__", &__le___int);
    self = objectAddAttr(self, "__le__argc", &twoArgc);

    // __gt__
    self = objectAddAttr(self, "__gt__", &__gt___int);
    self = objectAddAttr(self, "__gt__argc", &twoArgc);

    // __ge__
    self = objectAddAttr(self, "__ge__", &__ge___int);
    self = objectAddAttr(self, "__ge__argc", &twoArgc);

    // One argument methods

    // __sizeof__
    self = objectAddAttr(self, "__sizeof__", &__sizeof___int);
    self = objectAddAttr(self, "__sizeof__argc", &oneArgc);

    // __pos__
    self = objectAddAttr(self, "__pos__", &__pos___int);
    self = objectAddAttr(self, "__pos__argc", &oneArgc);

    // __neg__
    self = objectAddAttr(self, "__neg__", &__neg___int);
    self = objectAddAttr(self, "__neg__argc", &oneArgc);

    // __disp__
    self = objectAddAttr(self, "__disp__", &__disp___int);
    self = objectAddAttr(self, "__disp__argc", &oneArgc);

    // __bool__
    self = objectAddAttr(self, "__bool__", &__bool___int);
    self = objectAddAttr(self, "__bool__argc", &oneArgc);

    // __int__
    self = objectAddAttr(self, "__int__", &__int___int);
    self = objectAddAttr(self, "__int__argc", &oneArgc);

    // __long__
    self = objectAddAttr(self, "__long__", &__long___int);
    self = objectAddAttr(self, "__long__argc", &oneArgc);

    // // __string__
    // self = objectAddAttr(self, "__string__", &__disp___int);
    // self = objectAddAttr(self, "__string__argc", &oneArgc);

    if (is_malloc_ptr)
    {
        // __free__
        self = objectAddAttr(self, "__free__", &__free___int);
    }

    return self;
}
