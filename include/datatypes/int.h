Object __add___int(int argc, int * argv)
{
    if (!strcmp(mem[argv[1]].name, "int"))
    {
        int * first = objectGetAttr(mem[argv[0]], "value");
        int * secnd = objectGetAttr(mem[argv[1]], "value");

        int * third = makeIntPtr(first[0] + secnd[0]);
        pushTrash(third);

        return makeInt(third);
    }
    else if (!strcmp(mem[argv[1]].name, "long"))
    {
        int * first = objectGetAttr(mem[argv[0]], "value");
        long long * secnd = objectGetAttr(mem[argv[1]], "value");

        long long * third = makeLLPtr(first[0] + secnd[0]);
        pushTrash(third);

        return makeLong(third);
    }

    char * err = malloc(17 + strlen(mem[argv[1]].name) + 30 + 1);
    strcpy(err, "types 'int' and '");
    strcat(err, mem[argv[1]].name);
    strcat(err, "' are invalid operands for '+'");
    error(err, line_num);
}

Object __sub___int(int argc, int * argv)
{
    if (!strcmp(mem[argv[1]].name, "int"))
    {
        int * first = objectGetAttr(mem[argv[0]], "value");
        int * secnd = objectGetAttr(mem[argv[1]], "value");
        
        int * third = makeIntPtr(first[0] - secnd[0]);
        pushTrash(third);

        return makeInt(third);
    }
    else if (!strcmp(mem[argv[1]].name, "long"))
    {
        int * first = objectGetAttr(mem[argv[0]], "value");
        long long * secnd = objectGetAttr(mem[argv[1]], "value");

        long long * third = makeLLPtr(first[0] - secnd[0]);
        pushTrash(third);

        return makeLong(third);
    }

    char * err = malloc(17 + strlen(mem[argv[1]].name) + 30 + 1);
    strcpy(err, "types 'int' and '");
    strcat(err, mem[argv[1]].name);
    strcat(err, "' are invalid operands for '-'");
    error(err, line_num);
}

Object __mul___int(int argc, int * argv)
{
    if (!strcmp(mem[argv[1]].name, "int"))
    {
        int * first = objectGetAttr(mem[argv[0]], "value");
        int * secnd = objectGetAttr(mem[argv[1]], "value");

        int * third = makeIntPtr(first[0] * secnd[0]);
        pushTrash(third);

        return makeInt(third);
    }
    else if (!strcmp(mem[argv[1]].name, "long"))
    {
        int * first = objectGetAttr(mem[argv[0]], "value");
        long long * secnd = objectGetAttr(mem[argv[1]], "value");

        long long * third = makeLLPtr(first[0] * secnd[0]);
        pushTrash(third);

        return makeLong(third);
    }

    char * err = malloc(17 + strlen(mem[argv[1]].name) + 30 + 1);
    strcpy(err, "types 'int' and '");
    strcat(err, mem[argv[1]].name);
    strcat(err, "' are invalid operands for '*'");
    error(err, line_num);
}

Object __div___int(int argc, int * argv)
{
    if (!strcmp(mem[argv[1]].name, "int"))
    {
        int * first = objectGetAttr(mem[argv[0]], "value");
        int * secnd = objectGetAttr(mem[argv[1]], "value");

        if (!secnd[0])
            return makeInt(makeIntPtr(0));

        int * third = makeIntPtr(first[0] / secnd[0]);
        pushTrash(third);

        return makeInt(third);
    }
    else if (!strcmp(mem[argv[1]].name, "long"))
    {
        int * first = objectGetAttr(mem[argv[0]], "value");
        long long * secnd = objectGetAttr(mem[argv[1]], "value");

        if (!secnd[0])
            return makeInt(&falsePtr);

        long long * third = makeLLPtr(first[0] / secnd[0]);
        pushTrash(third);

        return makeLong(third);
    }

    char * err = malloc(17 + strlen(mem[argv[1]].name) + 30 + 1);
    strcpy(err, "types 'int' and '");
    strcat(err, mem[argv[1]].name);
    strcat(err, "' are invalid operands for '/'");
    error(err, line_num);
}

Object __pow___int(int argc, int * argv)
{
    if (!strcmp(mem[argv[1]].name, "int"))
    {
        int * first = objectGetAttr(mem[argv[0]], "value");
        int * secnd = objectGetAttr(mem[argv[1]], "value");

        int * third = makeIntPtr(ipowMath(first[0], secnd[0]));
        pushTrash(third);

        return makeInt(third);
    }
    else if (!strcmp(mem[argv[1]].name, "long"))
    {
        int * first = objectGetAttr(mem[argv[0]], "value");
        long long * secnd = objectGetAttr(mem[argv[1]], "value");

        long long * third = makeLLPtr(lpowMath(first[0], secnd[0]));
        pushTrash(third);

        return makeLong(third);
    }

    char * err = malloc(17 + strlen(mem[argv[1]].name) + 31 + 1);
    strcpy(err, "types 'int' and '");
    strcat(err, mem[argv[1]].name);
    strcat(err, "' are invalid operands for '**'");
    error(err, line_num);
}

Object __eq___int(int argc, int * argv)
{
    if (!strcmp(mem[argv[1]].name, "int"))
    {
        int * first = objectGetAttr(mem[argv[0]], "value");
        int * secnd = objectGetAttr(mem[argv[1]], "value");

        if (first[0] == secnd[0])
            return makeInt(&truePtr);
        return makeInt(&falsePtr);
    }
    else if (!strcmp(mem[argv[1]].name, "long"))
    {
        int * first = objectGetAttr(mem[argv[0]], "value");
        long long * secnd = objectGetAttr(mem[argv[1]], "value");

        if (first[0] == secnd[0])
            return makeInt(&truePtr);
        return makeInt(&falsePtr);
    }

    char * err = malloc(17 + strlen(mem[argv[1]].name) + 31 + 1);
    strcpy(err, "types 'int' and '");
    strcat(err, mem[argv[1]].name);
    strcat(err, "' can not be compared with '=='");
    error(err, line_num);
}

Object __lt___int(int argc, int * argv)
{
    if (!strcmp(mem[argv[1]].name, "int"))
    {
        int * first = objectGetAttr(mem[argv[0]], "value");
        int * secnd = objectGetAttr(mem[argv[1]], "value");

        if (first[0] < secnd[0])
            return makeInt(&truePtr);
        return makeInt(&falsePtr);
    }
    else if (!strcmp(mem[argv[1]].name, "long"))
    {
        int * first = objectGetAttr(mem[argv[0]], "value");
        long long * secnd = objectGetAttr(mem[argv[1]], "value");

        if (first[0] < secnd[0])
            return makeInt(&truePtr);
        return makeInt(&falsePtr);
    }

    char * err = malloc(17 + strlen(mem[argv[1]].name) + 30 + 1);
    strcpy(err, "types 'int' and '");
    strcat(err, mem[argv[1]].name);
    strcat(err, "' can not be compared with '<'");
    error(err, line_num);
}

Object __le___int(int argc, int * argv)
{
    if (!strcmp(mem[argv[1]].name, "int"))
    {
        int * first = objectGetAttr(mem[argv[0]], "value");
        int * secnd = objectGetAttr(mem[argv[1]], "value");

        if (first[0] <= secnd[0])
            return makeInt(&truePtr);
        return makeInt(&falsePtr);
    }
    else if (!strcmp(mem[argv[1]].name, "long"))
    {
        int * first = objectGetAttr(mem[argv[0]], "value");
        long long * secnd = objectGetAttr(mem[argv[1]], "value");

        if (first[0] <= secnd[0])
            return makeInt(&truePtr);
        return makeInt(&falsePtr);
    }

    char * err = malloc(17 + strlen(mem[argv[1]].name) + 31 + 1);
    strcpy(err, "types 'int' and '");
    strcat(err, mem[argv[1]].name);
    strcat(err, "' can not be compared with '<='");
    error(err, line_num);
}

Object __gt___int(int argc, int * argv)
{
    if (!strcmp(mem[argv[1]].name, "int"))
    {
        int * first = objectGetAttr(mem[argv[0]], "value");
        int * secnd = objectGetAttr(mem[argv[1]], "value");

        if (first[0] > secnd[0])
            return makeInt(&truePtr);
        return makeInt(&falsePtr);
    }
    else if (!strcmp(mem[argv[1]].name, "long"))
    {
        int * first = objectGetAttr(mem[argv[0]], "value");
        long long * secnd = objectGetAttr(mem[argv[1]], "value");

        if (first[0] > secnd[0])
            return makeInt(&truePtr);
        return makeInt(&falsePtr);
    }

    char * err = malloc(17 + strlen(mem[argv[1]].name) + 30 + 1);
    strcpy(err, "types 'int' and '");
    strcat(err, mem[argv[1]].name);
    strcat(err, "' can not be compared with '>'");
    error(err, line_num);
}

Object __ge___int(int argc, int * argv)
{
    if (!strcmp(mem[argv[1]].name, "int"))
    {
        int * first = objectGetAttr(mem[argv[0]], "value");
        int * secnd = objectGetAttr(mem[argv[1]], "value");

        if (first[0] >= secnd[0])
            return makeInt(&truePtr);
        return makeInt(&falsePtr);
    }
    else if (!strcmp(mem[argv[1]].name, "long"))
    {
        int * first = objectGetAttr(mem[argv[0]], "value");
        long long * secnd = objectGetAttr(mem[argv[1]], "value");

        if (first[0] >= secnd[0])
            return makeInt(&truePtr);
        return makeInt(&falsePtr);
    }

    char * err = malloc(17 + strlen(mem[argv[1]].name) + 31 + 1);
    strcpy(err, "types 'int' and '");
    strcat(err, mem[argv[1]].name);
    strcat(err, "' can not be compared with '>='");
    error(err, line_num);
}

Object __pos___int(int argc, int * argv)
{
    int * first = objectGetAttr(mem[argv[0]], "value");

    int * ret = makeIntPtr(first[0]);
    pushTrash(ret);

    return makeInt(ret);
}

Object __neg___int(int argc, int * argv)
{
    int * first = objectGetAttr(mem[argv[0]], "value");

    int * ret = makeIntPtr(-first[0]);
    pushTrash(ret);

    return makeInt(ret);
}

Object __disp___int(int argc, int * argv)
{
    int * thisvalue = objectGetAttr(mem[argv[0]], "value");

    char * tostr = intToStr(thisvalue[0]);
    pushTrash(tostr);

    return makeString(tostr);
}

Object __bool___int(int argc, int * argv)
{
    int * thisvalue = objectGetAttr(mem[argv[0]], "value");

    if (thisvalue[0])
        return makeInt(&truePtr);
    return makeInt(&falsePtr);
}

Object __int___int(int argc, int * argv)
{
    int * first = objectGetAttr(mem[argv[0]], "value");

    int * ret = makeIntPtr(first[0]);
    pushTrash(ret);

    return makeInt(ret);
}

Object makeInt(int * value)
{
    Object self;

    self = makeObject("int", value);

    // Two argument methods

    // __add__
    self = addObjectValue(self, "__add__", &__add___int);
    self = addObjectValue(self, "__add__argc", &twoArgc);

    // __sub__
    self = addObjectValue(self, "__sub__", &__sub___int);
    self = addObjectValue(self, "__sub__argc", &twoArgc);

    // __mul__
    self = addObjectValue(self, "__mul__", &__mul___int);
    self = addObjectValue(self, "__mul__argc", &twoArgc);

    // __div__
    self = addObjectValue(self, "__div__", &__div___int);
    self = addObjectValue(self, "__div__argc", &twoArgc);

    // __pow__
    self = addObjectValue(self, "__pow__", &__pow___int);
    self = addObjectValue(self, "__pow__argc", &twoArgc);

    // __eq__
    self = addObjectValue(self, "__eq__", &__eq___int);
    self = addObjectValue(self, "__eq__argc", &twoArgc);

    // __lt__
    self = addObjectValue(self, "__lt__", &__lt___int);
    self = addObjectValue(self, "__lt__argc", &twoArgc);

    // __le__
    self = addObjectValue(self, "__le__", &__le___int);
    self = addObjectValue(self, "__le__argc", &twoArgc);

    // __gt__
    self = addObjectValue(self, "__gt__", &__gt___int);
    self = addObjectValue(self, "__gt__argc", &twoArgc);

    // __ge__
    self = addObjectValue(self, "__ge__", &__ge___int);
    self = addObjectValue(self, "__ge__argc", &twoArgc);

    // One argument methods

    // __pos__
    self = addObjectValue(self, "__pos__", &__pos___int);
    self = addObjectValue(self, "__pos__argc", &oneArgc);

    // __neg__
    self = addObjectValue(self, "__neg__", &__neg___int);
    self = addObjectValue(self, "__neg__argc", &oneArgc);

    // __disp__
    self = addObjectValue(self, "__disp__", &__disp___int);
    self = addObjectValue(self, "__disp__argc", &oneArgc);

    // __bool__
    self = addObjectValue(self, "__bool__", &__bool___int);
    self = addObjectValue(self, "__bool__argc", &oneArgc);

    // __int__
    self = addObjectValue(self, "__int__", &__int___int);
    self = addObjectValue(self, "__int__argc", &oneArgc);

    // // __string__
    // self = addObjectValue(self, "__string__", &__string___int);
    // self = addObjectValue(self, "__string__argc", &oneArgc);

    return self;
}
