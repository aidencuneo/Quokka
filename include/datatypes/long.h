Object * __add___long(int argc, Object ** argv)
{
    if (!strcmp(argv[1]->name, "int"))
    {
        long long * first = objectGetAttr(argv[0], "value");
        int * secnd = objectGetAttr(argv[1], "value");

        long long * third = makeLLPtr(first[0] + secnd[0]);

        return makeLong(third, 1);
    }
    else if (!strcmp(argv[1]->name, "long"))
    {
        long long * first = objectGetAttr(argv[0], "value");
        long long * secnd = objectGetAttr(argv[1], "value");

        long long * third = makeLLPtr(first[0] + secnd[0]);

        return makeLong(third, 1);
    }

    char * err = malloc(18 + strlen(argv[1]->name) + 30 + 1);
    strcpy(err, "types 'long' and '");
    strcat(err, argv[1]->name);
    strcat(err, "' are invalid operands for '+'");
    error(err, line_num);

    return makeNull();
}

Object * __sub___long(int argc, Object ** argv)
{
    if (!strcmp(argv[1]->name, "int"))
    {
        long long * first = objectGetAttr(argv[0], "value");
        int * secnd = objectGetAttr(argv[1], "value");

        long long * third = makeLLPtr(first[0] - secnd[0]);

        return makeLong(third, 1);
    }
    else if (!strcmp(argv[1]->name, "long"))
    {
        long long * first = objectGetAttr(argv[0], "value");
        long long * secnd = objectGetAttr(argv[1], "value");

        long long * third = makeLLPtr(first[0] - secnd[0]);

        return makeLong(third, 1);
    }

    char * err = malloc(18 + strlen(argv[1]->name) + 30 + 1);
    strcpy(err, "types 'long' and '");
    strcat(err, argv[1]->name);
    strcat(err, "' are invalid operands for '-'");
    error(err, line_num);

    return makeNull();
}

Object * __mul___long(int argc, Object ** argv)
{
    if (!strcmp(argv[1]->name, "int"))
    {
        long long * first = objectGetAttr(argv[0], "value");
        int * secnd = objectGetAttr(argv[1], "value");

        long long * third = makeLLPtr(first[0] * secnd[0]);

        return makeLong(third, 1);
    }
    else if (!strcmp(argv[1]->name, "long"))
    {
        long long * first = objectGetAttr(argv[0], "value");
        long long * secnd = objectGetAttr(argv[1], "value");

        long long * third = makeLLPtr(first[0] * secnd[0]);

        return makeLong(third, 1);
    }

    char * err = malloc(18 + strlen(argv[1]->name) + 30 + 1);
    strcpy(err, "types 'long' and '");
    strcat(err, argv[1]->name);
    strcat(err, "' are invalid operands for '*'");
    error(err, line_num);

    return makeNull();
}

Object * __div___long(int argc, Object ** argv)
{
    if (!strcmp(argv[1]->name, "int"))
    {
        long long * first = objectGetAttr(argv[0], "value");
        int * secnd = objectGetAttr(argv[1], "value");

        if (!secnd[0])
            return makeInt(&falsePtr, 0);

        long long * third = makeLLPtr(first[0] / secnd[0]);

        return makeLong(third, 1);
    }
    else if (!strcmp(argv[1]->name, "long"))
    {
        long long * first = objectGetAttr(argv[0], "value");
        long long * secnd = objectGetAttr(argv[1], "value");

        if (!secnd[0])
            return makeInt(&falsePtr, 0);

        long long * third = makeLLPtr(first[0] / secnd[0]);

        return makeLong(third, 1);
    }

    char * err = malloc(18 + strlen(argv[1]->name) + 30 + 1);
    strcpy(err, "types 'long' and '");
    strcat(err, argv[1]->name);
    strcat(err, "' are invalid operands for '/'");
    error(err, line_num);

    return makeNull();
}

Object * __pow___long(int argc, Object ** argv)
{
    if (!strcmp(argv[1]->name, "int"))
    {
        long long * first = objectGetAttr(argv[0], "value");
        int * secnd = objectGetAttr(argv[1], "value");

        long long * third = makeLLPtr(lpowMath(first[0], secnd[0]));

        return makeLong(third, 1);
    }
    else if (!strcmp(argv[1]->name, "long"))
    {
        long long * first = objectGetAttr(argv[0], "value");
        long long * secnd = objectGetAttr(argv[1], "value");

        long long * third = makeLLPtr(lpowMath(first[0], secnd[0]));

        return makeLong(third, 1);
    }

    char * err = malloc(18 + strlen(argv[1]->name) + 31 + 1);
    strcpy(err, "types 'long' and '");
    strcat(err, argv[1]->name);
    strcat(err, "' are invalid operands for '**'");
    error(err, line_num);

    return makeNull();
}

Object * __eq___long(int argc, Object ** argv)
{
    if (!strcmp(argv[1]->name, "int"))
    {
        long long * first = objectGetAttr(argv[0], "value");
        int * secnd = objectGetAttr(argv[1], "value");

        if (first[0] == secnd[0])
            return makeInt(&truePtr, 0);
        return makeInt(&falsePtr, 0);
    }
    else if (!strcmp(argv[1]->name, "long"))
    {
        long long * first = objectGetAttr(argv[0], "value");
        long long * secnd = objectGetAttr(argv[1], "value");

        if (first[0] == secnd[0])
            return makeInt(&truePtr, 0);
        return makeInt(&falsePtr, 0);
    }

    char * err = malloc(18 + strlen(argv[1]->name) + 31 + 1);
    strcpy(err, "types 'long' and '");
    strcat(err, argv[1]->name);
    strcat(err, "' can not be compared with '=='");
    error(err, line_num);

    return makeNull();
}

Object * __lt___long(int argc, Object ** argv)
{
    if (!strcmp(argv[1]->name, "int"))
    {
        long long * first = objectGetAttr(argv[0], "value");
        int * secnd = objectGetAttr(argv[1], "value");

        if (first[0] < secnd[0])
            return makeInt(&truePtr, 0);
        return makeInt(&falsePtr, 0);
    }
    else if (!strcmp(argv[1]->name, "long"))
    {
        long long * first = objectGetAttr(argv[0], "value");
        long long * secnd = objectGetAttr(argv[1], "value");

        if (first[0] < secnd[0])
            return makeInt(&truePtr, 0);
        return makeInt(&falsePtr, 0);
    }

    char * err = malloc(18 + strlen(argv[1]->name) + 30 + 1);
    strcpy(err, "types 'long' and '");
    strcat(err, argv[1]->name);
    strcat(err, "' can not be compared with '<'");
    error(err, line_num);

    return makeNull();
}

Object * __le___long(int argc, Object ** argv)
{
    if (!strcmp(argv[1]->name, "int"))
    {
        long long * first = objectGetAttr(argv[0], "value");
        int * secnd = objectGetAttr(argv[1], "value");

        if (first[0] <= secnd[0])
            return makeInt(&truePtr, 0);
        return makeInt(&falsePtr, 0);
    }
    else if (!strcmp(argv[1]->name, "long"))
    {
        long long * first = objectGetAttr(argv[0], "value");
        long long * secnd = objectGetAttr(argv[1], "value");

        if (first[0] <= secnd[0])
            return makeInt(&truePtr, 0);
        return makeInt(&falsePtr, 0);
    }

    char * err = malloc(18 + strlen(argv[1]->name) + 31 + 1);
    strcpy(err, "types 'long' and '");
    strcat(err, argv[1]->name);
    strcat(err, "' can not be compared with '<='");
    error(err, line_num);

    return makeNull();
}

Object * __gt___long(int argc, Object ** argv)
{
    if (!strcmp(argv[1]->name, "int"))
    {
        long long * first = objectGetAttr(argv[0], "value");
        int * secnd = objectGetAttr(argv[1], "value");

        if (first[0] > secnd[0])
            return makeInt(&truePtr, 0);
        return makeInt(&falsePtr, 0);
    }
    else if (!strcmp(argv[1]->name, "long"))
    {
        long long * first = objectGetAttr(argv[0], "value");
        long long * secnd = objectGetAttr(argv[1], "value");

        if (first[0] > secnd[0])
            return makeInt(&truePtr, 0);
        return makeInt(&falsePtr, 0);
    }

    char * err = malloc(18 + strlen(argv[1]->name) + 30 + 1);
    strcpy(err, "types 'long' and '");
    strcat(err, argv[1]->name);
    strcat(err, "' can not be compared with '>'");
    error(err, line_num);

    return makeNull();
}

Object * __ge___long(int argc, Object ** argv)
{
    if (!strcmp(argv[1]->name, "int"))
    {
        long long * first = objectGetAttr(argv[0], "value");
        int * secnd = objectGetAttr(argv[1], "value");

        if (first[0] >= secnd[0])
            return makeInt(&truePtr, 0);
        return makeInt(&falsePtr, 0);
    }
    else if (!strcmp(argv[1]->name, "long"))
    {
        long long * first = objectGetAttr(argv[0], "value");
        long long * secnd = objectGetAttr(argv[1], "value");

        if (first[0] >= secnd[0])
            return makeInt(&truePtr, 0);
        return makeInt(&falsePtr, 0);
    }

    char * err = malloc(18 + strlen(argv[1]->name) + 31 + 1);
    strcpy(err, "types 'long' and '");
    strcat(err, argv[1]->name);
    strcat(err, "' can not be compared with '>='");
    error(err, line_num);

    return makeNull();
}

Object * __sizeof___long(int argc, Object ** argv)
{
    long long * thisvalue = objectGetAttr(argv[0], "value");

    int * size = makeIntPtr(sizeof(argv[0]) + sizeof(thisvalue[0]));
    // pushTrash(size);

    return makeInt(size, 1);
}

Object * __pos___long(int argc, Object ** argv)
{
    long long * thisvalue = objectGetAttr(argv[0], "value");

    if (thisvalue[0] > INT_MAX)
    {
        int * intptr = makeIntPtr(INT_MAX);

        return makeInt(intptr, 1);
    }

    int * intptr = makeIntPtr(thisvalue[0]);

    return makeInt(intptr, 1);
}

Object * __neg___long(int argc, Object ** argv)
{
    long long * thisvalue = objectGetAttr(argv[0], "value");

    long long * negptr = makeLLPtr(-thisvalue[0]);

    return makeLong(negptr, 1);
}

Object * __disp___long(int argc, Object ** argv)
{
    long long * thisvalue = objectGetAttr(argv[0], "value");

    char * tostr = LLToStr(thisvalue[0]);
    mstrcat(&tostr, "L");

    return makeString(tostr, 1);
}

Object * __bool___long(int argc, Object ** argv)
{
    long long * thisvalue = objectGetAttr(argv[0], "value");

    if (thisvalue[0])
        return makeInt(&truePtr, 0);
    return makeInt(&falsePtr, 0);
}

Object * __int___long(int argc, Object ** argv)
{
    long long * thisvalue = objectGetAttr(argv[0], "value");

    if (thisvalue[0] > UINT_MAX)
        return makeInt(makeIntPtr(UINT_MAX), 0);

    return makeInt(makeIntPtr(thisvalue[0]), 1);
}

Object * __free___long(int argc, Object ** argv)
{
    long long * thisvalue = objectGetAttr(argv[0], "value");
    free(thisvalue);

    return makeNull();
}

Object * makeLong(long long * value, int is_malloc_ptr)
{
    Object * self = objectPointer();

    self->name = "long";

    // 1 to 2 Attributes
    if (is_malloc_ptr)
    {
        self->names = malloc(2 * sizeof(char *));
        self->values = malloc(2 * sizeof(void *));
    }
    else
    {
        self->names = malloc(sizeof(char *));
        self->values = malloc(sizeof(void *));
    }
    self->value_count = 0;

    self = objectAddAttr(self, "value", value);

    if (is_malloc_ptr)
    {
        // __free__
        self = objectAddAttr(self, "__free__", &__free___long);
    }

    return self;
}
