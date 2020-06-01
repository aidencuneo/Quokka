Object __add___long(int argc, Object * argv)
{
    if (!strcmp(argv[1].name, "int"))
    {
        long long * first = objectGetAttr(argv[0], "value");
        int * secnd = objectGetAttr(argv[1], "value");

        long long * third = makeLLPtr(first[0] + secnd[0]);
        // pushTrash(third);

        return makeLong(third);
    }
    else if (!strcmp(argv[1].name, "long"))
    {
        long long * first = objectGetAttr(argv[0], "value");
        long long * secnd = objectGetAttr(argv[1], "value");

        long long * third = makeLLPtr(first[0] + secnd[0]);
        // pushTrash(third);

        return makeLong(third);
    }

    char * err = malloc(18 + strlen(argv[1].name) + 30 + 1);
    strcpy(err, "types 'long' and '");
    strcat(err, argv[1].name);
    strcat(err, "' are invalid operands for '+'");
    error(err, line_num);

    return makeNull();
}

Object __sub___long(int argc, Object * argv)
{
    if (!strcmp(argv[1].name, "int"))
    {
        long long * first = objectGetAttr(argv[0], "value");
        int * secnd = objectGetAttr(argv[1], "value");

        long long * third = makeLLPtr(first[0] - secnd[0]);
        // pushTrash(third);

        return makeLong(third);
    }
    else if (!strcmp(argv[1].name, "long"))
    {
        long long * first = objectGetAttr(argv[0], "value");
        long long * secnd = objectGetAttr(argv[1], "value");

        long long * third = makeLLPtr(first[0] - secnd[0]);
        // pushTrash(third);

        return makeLong(third);
    }

    char * err = malloc(18 + strlen(argv[1].name) + 30 + 1);
    strcpy(err, "types 'long' and '");
    strcat(err, argv[1].name);
    strcat(err, "' are invalid operands for '-'");
    error(err, line_num);

    return makeNull();
}

Object __mul___long(int argc, Object * argv)
{
    if (!strcmp(argv[1].name, "int"))
    {
        long long * first = objectGetAttr(argv[0], "value");
        int * secnd = objectGetAttr(argv[1], "value");

        long long * third = makeLLPtr(first[0] * secnd[0]);
        // pushTrash(third);

        return makeLong(third);
    }
    else if (!strcmp(argv[1].name, "long"))
    {
        long long * first = objectGetAttr(argv[0], "value");
        long long * secnd = objectGetAttr(argv[1], "value");

        long long * third = makeLLPtr(first[0] * secnd[0]);
        // pushTrash(third);

        return makeLong(third);
    }

    char * err = malloc(18 + strlen(argv[1].name) + 30 + 1);
    strcpy(err, "types 'long' and '");
    strcat(err, argv[1].name);
    strcat(err, "' are invalid operands for '*'");
    error(err, line_num);

    return makeNull();
}

Object __div___long(int argc, Object * argv)
{
    if (!strcmp(argv[1].name, "int"))
    {
        long long * first = objectGetAttr(argv[0], "value");
        int * secnd = objectGetAttr(argv[1], "value");

        if (!secnd[0])
            return makeInt(&falsePtr);

        long long * third = makeLLPtr(first[0] / secnd[0]);
        // pushTrash(third);

        return makeLong(third);
    }
    else if (!strcmp(argv[1].name, "long"))
    {
        long long * first = objectGetAttr(argv[0], "value");
        long long * secnd = objectGetAttr(argv[1], "value");

        if (!secnd[0])
            return makeInt(&falsePtr);

        long long * third = makeLLPtr(first[0] / secnd[0]);
        // pushTrash(third);

        return makeLong(third);
    }

    char * err = malloc(18 + strlen(argv[1].name) + 30 + 1);
    strcpy(err, "types 'long' and '");
    strcat(err, argv[1].name);
    strcat(err, "' are invalid operands for '/'");
    error(err, line_num);

    return makeNull();
}

Object __pow___long(int argc, Object * argv)
{
    if (!strcmp(argv[1].name, "int"))
    {
        long long * first = objectGetAttr(argv[0], "value");
        int * secnd = objectGetAttr(argv[1], "value");

        long long * third = makeLLPtr(lpowMath(first[0], secnd[0]));
        // pushTrash(third);

        return makeLong(third);
    }
    else if (!strcmp(argv[1].name, "long"))
    {
        long long * first = objectGetAttr(argv[0], "value");
        long long * secnd = objectGetAttr(argv[1], "value");

        long long * third = makeLLPtr(lpowMath(first[0], secnd[0]));
        // pushTrash(third);

        return makeLong(third);
    }

    char * err = malloc(18 + strlen(argv[1].name) + 31 + 1);
    strcpy(err, "types 'long' and '");
    strcat(err, argv[1].name);
    strcat(err, "' are invalid operands for '**'");
    error(err, line_num);

    return makeNull();
}

Object __eq___long(int argc, Object * argv)
{
    if (!strcmp(argv[1].name, "int"))
    {
        long long * first = objectGetAttr(argv[0], "value");
        int * secnd = objectGetAttr(argv[1], "value");

        if (first[0] == secnd[0])
            return makeInt(&truePtr);
        return makeInt(&falsePtr);
    }
    else if (!strcmp(argv[1].name, "long"))
    {
        long long * first = objectGetAttr(argv[0], "value");
        long long * secnd = objectGetAttr(argv[1], "value");

        if (first[0] == secnd[0])
            return makeInt(&truePtr);
        return makeInt(&falsePtr);
    }

    char * err = malloc(18 + strlen(argv[1].name) + 31 + 1);
    strcpy(err, "types 'long' and '");
    strcat(err, argv[1].name);
    strcat(err, "' can not be compared with '=='");
    error(err, line_num);

    return makeNull();
}

Object __lt___long(int argc, Object * argv)
{
    if (!strcmp(argv[1].name, "int"))
    {
        long long * first = objectGetAttr(argv[0], "value");
        int * secnd = objectGetAttr(argv[1], "value");

        if (first[0] < secnd[0])
            return makeInt(&truePtr);
        return makeInt(&falsePtr);
    }
    else if (!strcmp(argv[1].name, "long"))
    {
        long long * first = objectGetAttr(argv[0], "value");
        long long * secnd = objectGetAttr(argv[1], "value");

        if (first[0] < secnd[0])
            return makeInt(&truePtr);
        return makeInt(&falsePtr);
    }

    char * err = malloc(18 + strlen(argv[1].name) + 30 + 1);
    strcpy(err, "types 'long' and '");
    strcat(err, argv[1].name);
    strcat(err, "' can not be compared with '<'");
    error(err, line_num);

    return makeNull();
}

Object __le___long(int argc, Object * argv)
{
    if (!strcmp(argv[1].name, "int"))
    {
        long long * first = objectGetAttr(argv[0], "value");
        int * secnd = objectGetAttr(argv[1], "value");

        if (first[0] <= secnd[0])
            return makeInt(&truePtr);
        return makeInt(&falsePtr);
    }
    else if (!strcmp(argv[1].name, "long"))
    {
        long long * first = objectGetAttr(argv[0], "value");
        long long * secnd = objectGetAttr(argv[1], "value");

        if (first[0] <= secnd[0])
            return makeInt(&truePtr);
        return makeInt(&falsePtr);
    }

    char * err = malloc(18 + strlen(argv[1].name) + 31 + 1);
    strcpy(err, "types 'long' and '");
    strcat(err, argv[1].name);
    strcat(err, "' can not be compared with '<='");
    error(err, line_num);

    return makeNull();
}

Object __gt___long(int argc, Object * argv)
{
    if (!strcmp(argv[1].name, "int"))
    {
        long long * first = objectGetAttr(argv[0], "value");
        int * secnd = objectGetAttr(argv[1], "value");

        if (first[0] > secnd[0])
            return makeInt(&truePtr);
        return makeInt(&falsePtr);
    }
    else if (!strcmp(argv[1].name, "long"))
    {
        long long * first = objectGetAttr(argv[0], "value");
        long long * secnd = objectGetAttr(argv[1], "value");

        if (first[0] > secnd[0])
            return makeInt(&truePtr);
        return makeInt(&falsePtr);
    }

    char * err = malloc(18 + strlen(argv[1].name) + 30 + 1);
    strcpy(err, "types 'long' and '");
    strcat(err, argv[1].name);
    strcat(err, "' can not be compared with '>'");
    error(err, line_num);

    return makeNull();
}

Object __ge___long(int argc, Object * argv)
{
    if (!strcmp(argv[1].name, "int"))
    {
        long long * first = objectGetAttr(argv[0], "value");
        int * secnd = objectGetAttr(argv[1], "value");

        if (first[0] >= secnd[0])
            return makeInt(&truePtr);
        return makeInt(&falsePtr);
    }
    else if (!strcmp(argv[1].name, "long"))
    {
        long long * first = objectGetAttr(argv[0], "value");
        long long * secnd = objectGetAttr(argv[1], "value");

        if (first[0] >= secnd[0])
            return makeInt(&truePtr);
        return makeInt(&falsePtr);
    }

    char * err = malloc(18 + strlen(argv[1].name) + 31 + 1);
    strcpy(err, "types 'long' and '");
    strcat(err, argv[1].name);
    strcat(err, "' can not be compared with '>='");
    error(err, line_num);

    return makeNull();
}

Object __sizeof___long(int argc, Object * argv)
{
    long long * thisvalue = objectGetAttr(argv[0], "value");

    int * size = makeIntPtr(sizeof(argv[0]) + sizeof(thisvalue[0]));
    // pushTrash(size);

    return makeInt(size);
}

Object __pos___long(int argc, Object * argv)
{
    long long * thisvalue = objectGetAttr(argv[0], "value");

    if (thisvalue[0] > INT_MAX)
    {
        int * intptr = makeIntPtr(INT_MAX);
        // pushTrash(intptr);

        return makeInt(intptr);
    }

    int * intptr = makeIntPtr(thisvalue[0]);
    // pushTrash(intptr);

    return makeInt(intptr);
}

Object __neg___long(int argc, Object * argv)
{
    long long * thisvalue = objectGetAttr(argv[0], "value");

    long long * negptr = makeLLPtr(-thisvalue[0]);
    // pushTrash(negptr);

    return makeLong(negptr);
}

Object __disp___long(int argc, Object * argv)
{
    long long * thisvalue = objectGetAttr(argv[0], "value");

    char * tostr = LLToStr(thisvalue[0]);
    mstrcat(&tostr, "L");
    // pushTrash(tostr);

    return makeString(tostr);
}

Object __bool___long(int argc, Object * argv)
{
    long long * thisvalue = objectGetAttr(argv[0], "value");

    if (thisvalue[0])
        return makeInt(&truePtr);
    return makeInt(&falsePtr);
}

Object __int___long(int argc, Object * argv)
{
    long long * thisvalue = objectGetAttr(argv[0], "value");

    if (thisvalue[0] > UINT_MAX)
        return makeInt(makeIntPtr(UINT_MAX));

    return makeInt(makeIntPtr(thisvalue[0]));
}

Object makeLong(long long * value)
{
    Object self;

    self.name = "long";

    // 33 Attributes
    self.names = malloc(33 * sizeof(char *));
    self.values = malloc(33 * sizeof(void *));
    self.value_count = 0;

    self = objectAddAttr(self, "value", value);

    // Two argument methods

    // __add__
    self = objectAddAttr(self, "__add__", &__add___long);
    self = objectAddAttr(self, "__add__argc", &twoArgc);

    // __sub__
    self = objectAddAttr(self, "__sub__", &__sub___long);
    self = objectAddAttr(self, "__sub__argc", &twoArgc);

    // __mul__
    self = objectAddAttr(self, "__mul__", &__mul___long);
    self = objectAddAttr(self, "__mul__argc", &twoArgc);

    // __div__
    self = objectAddAttr(self, "__div__", &__div___long);
    self = objectAddAttr(self, "__div__argc", &twoArgc);

    // __pow__
    self = objectAddAttr(self, "__pow__", &__pow___long);
    self = objectAddAttr(self, "__pow__argc", &twoArgc);

    // __eq__
    self = objectAddAttr(self, "__eq__", &__eq___long);
    self = objectAddAttr(self, "__eq__argc", &twoArgc);

    // __lt__
    self = objectAddAttr(self, "__lt__", &__lt___long);
    self = objectAddAttr(self, "__lt__argc", &twoArgc);

    // __le__
    self = objectAddAttr(self, "__le__", &__le___long);
    self = objectAddAttr(self, "__le__argc", &twoArgc);

    // __gt__
    self = objectAddAttr(self, "__gt__", &__gt___long);
    self = objectAddAttr(self, "__gt__argc", &twoArgc);

    // __ge__
    self = objectAddAttr(self, "__ge__", &__ge___long);
    self = objectAddAttr(self, "__ge__argc", &twoArgc);

    // One argument methods

    // __sizeof__
    self = objectAddAttr(self, "__sizeof__", &__sizeof___long);
    self = objectAddAttr(self, "__sizeof__argc", &oneArgc);

    // __pos__
    self = objectAddAttr(self, "__pos__", &__pos___long);
    self = objectAddAttr(self, "__pos__argc", &oneArgc);

    // __neg__
    self = objectAddAttr(self, "__neg__", &__neg___long);
    self = objectAddAttr(self, "__neg__argc", &oneArgc);

    // __disp__
    self = objectAddAttr(self, "__disp__", &__disp___long);
    self = objectAddAttr(self, "__disp__argc", &oneArgc);

    // __bool__
    self = objectAddAttr(self, "__bool__", &__bool___long);
    self = objectAddAttr(self, "__bool__argc", &oneArgc);

    // __int__
    self = objectAddAttr(self, "__int__", &__int___long);
    self = objectAddAttr(self, "__int__argc", &oneArgc);

    // // __string__
    // self = objectAddAttr(self, "__string__", &__disp___long);
    // self = objectAddAttr(self, "__string__argc", &oneArgc);

    return self;
}
