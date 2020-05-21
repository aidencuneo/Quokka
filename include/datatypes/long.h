Object __add___long(int argc, Object * argv)
{
    if (!strcmp(argv[1].name, "int"))
    {
        long long * first = objectGetAttr(argv[0], "value");
        int * secnd = objectGetAttr(argv[1], "value");
        return makeLong(makeLLPtr(first[0] + secnd[0]));
    }
    else if (!strcmp(argv[1].name, "long"))
    {
        long long * first = objectGetAttr(argv[0], "value");
        long long * secnd = objectGetAttr(argv[1], "value");
        return makeLong(makeLLPtr(first[0] + secnd[0]));
    }

    char * err = malloc(18 + strlen(argv[1].name) + 30 + 1);
    strcpy(err, "types 'long' and '");
    strcat(err, argv[1].name);
    strcat(err, "' are invalid operands for '+'");
    error(err, line_num);
}

Object __sub___long(int argc, Object * argv)
{
    if (!strcmp(argv[1].name, "int"))
    {
        long long * first = objectGetAttr(argv[0], "value");
        int * secnd = objectGetAttr(argv[1], "value");
        return makeLong(makeLLPtr(first[0] - secnd[0]));
    }
    else if (!strcmp(argv[1].name, "long"))
    {
        long long * first = objectGetAttr(argv[0], "value");
        long long * secnd = objectGetAttr(argv[1], "value");
        return makeLong(makeLLPtr(first[0] - secnd[0]));
    }

    char * err = malloc(18 + strlen(argv[1].name) + 30 + 1);
    strcpy(err, "types 'long' and '");
    strcat(err, argv[1].name);
    strcat(err, "' are invalid operands for '-'");
    error(err, line_num);
}

Object __mul___long(int argc, Object * argv)
{
    if (!strcmp(argv[1].name, "int"))
    {
        long long * first = objectGetAttr(argv[0], "value");
        int * secnd = objectGetAttr(argv[1], "value");
        return makeLong(makeLLPtr(first[0] * secnd[0]));
    }
    else if (!strcmp(argv[1].name, "long"))
    {
        long long * first = objectGetAttr(argv[0], "value");
        long long * secnd = objectGetAttr(argv[1], "value");
        return makeLong(makeLLPtr(first[0] * secnd[0]));
    }

    char * err = malloc(18 + strlen(argv[1].name) + 30 + 1);
    strcpy(err, "types 'long' and '");
    strcat(err, argv[1].name);
    strcat(err, "' are invalid operands for '*'");
    error(err, line_num);
}

Object __div___long(int argc, Object * argv)
{
    if (!strcmp(argv[1].name, "int"))
    {
        long long * first = objectGetAttr(argv[0], "value");
        int * secnd = objectGetAttr(argv[1], "value");

        if (!secnd[0])
            return makeInt(makeIntPtr(0));

        return makeLong(makeLLPtr(first[0] / secnd[0]));
    }
    else if (!strcmp(argv[1].name, "long"))
    {
        long long * first = objectGetAttr(argv[0], "value");
        long long * secnd = objectGetAttr(argv[1], "value");

        if (!secnd[0])
            return makeInt(makeIntPtr(0));

        return makeLong(makeLLPtr(first[0] / secnd[0]));
    }

    char * err = malloc(18 + strlen(argv[1].name) + 30 + 1);
    strcpy(err, "types 'long' and '");
    strcat(err, argv[1].name);
    strcat(err, "' are invalid operands for '/'");
    error(err, line_num);
}

Object __pow___long(int argc, Object * argv)
{
    if (!strcmp(argv[1].name, "int"))
    {
        long long * first = objectGetAttr(argv[0], "value");
        int * secnd = objectGetAttr(argv[1], "value");
        return makeLong(makeLLPtr(lpowMath(first[0], secnd[0])));
    }
    else if (!strcmp(argv[1].name, "long"))
    {
        long long * first = objectGetAttr(argv[0], "value");
        long long * secnd = objectGetAttr(argv[1], "value");
        return makeLong(makeLLPtr(lpowMath(first[0], secnd[0])));
    }

    char * err = malloc(18 + strlen(argv[1].name) + 31 + 1);
    strcpy(err, "types 'long' and '");
    strcat(err, argv[1].name);
    strcat(err, "' are invalid operands for '**'");
    error(err, line_num);
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
}

Object __pos___long(int argc, Object * argv)
{
    long long * thisvalue = objectGetAttr(argv[0], "value");

    if (thisvalue[0] > INT_MAX)
        return makeInt(makeIntPtr(INT_MAX));

    return makeInt(makeIntPtr(thisvalue[0]));
}

Object __neg___long(int argc, Object * argv)
{
    long long * thisvalue = objectGetAttr(argv[0], "value");

    return makeLong(makeLLPtr(-thisvalue[0]));
}

Object __disp___long(int argc, Object * argv)
{
    long long * thisvalue = objectGetAttr(argv[0], "value");
    char * tostr = LLToStr(thisvalue[0]);

    pushTrash(tostr);

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

    self = makeObject("long", value);

    // Two argument methods

    // __add__
    self = addObjectValue(self, "__add__", &__add___long);
    self = addObjectValue(self, "__add__argc", &twoArgc);

    // __sub__
    self = addObjectValue(self, "__sub__", &__sub___long);
    self = addObjectValue(self, "__sub__argc", &twoArgc);

    // __mul__
    self = addObjectValue(self, "__mul__", &__mul___long);
    self = addObjectValue(self, "__mul__argc", &twoArgc);

    // __div__
    self = addObjectValue(self, "__div__", &__div___long);
    self = addObjectValue(self, "__div__argc", &twoArgc);

    // __pow__
    self = addObjectValue(self, "__pow__", &__pow___long);
    self = addObjectValue(self, "__pow__argc", &twoArgc);

    // __eq__
    self = addObjectValue(self, "__eq__", &__eq___long);
    self = addObjectValue(self, "__eq__argc", &twoArgc);

    // __lt__
    self = addObjectValue(self, "__lt__", &__lt___long);
    self = addObjectValue(self, "__lt__argc", &twoArgc);

    // __le__
    self = addObjectValue(self, "__le__", &__le___long);
    self = addObjectValue(self, "__le__argc", &twoArgc);

    // __gt__
    self = addObjectValue(self, "__gt__", &__gt___long);
    self = addObjectValue(self, "__gt__argc", &twoArgc);

    // __ge__
    self = addObjectValue(self, "__ge__", &__ge___long);
    self = addObjectValue(self, "__ge__argc", &twoArgc);

    // One argument methods

    // __pos__
    self = addObjectValue(self, "__pos__", &__pos___long);
    self = addObjectValue(self, "__pos__argc", &oneArgc);

    // __neg__
    self = addObjectValue(self, "__neg__", &__neg___long);
    self = addObjectValue(self, "__neg__argc", &oneArgc);

    // __disp__
    self = addObjectValue(self, "__disp__", &__disp___long);
    self = addObjectValue(self, "__disp__argc", &oneArgc);

    // __bool__
    self = addObjectValue(self, "__bool__", &__bool___long);
    self = addObjectValue(self, "__bool__argc", &oneArgc);

    // __int__
    self = addObjectValue(self, "__int__", &__int___long);
    self = addObjectValue(self, "__int__argc", &oneArgc);

    // // __string__
    // self = addObjectValue(self, "__string__", &__string___long);
    // self = addObjectValue(self, "__string__argc", &oneArgc);

    return self;
}
