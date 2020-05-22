Object __add___string(int argc, int * argv)
{
    if (!strcmp(mem[argv[1]].name, "string"))
    {
        char * first = objectGetAttr(mem[argv[0]], "value");
        char * secnd = objectGetAttr(mem[argv[1]], "value");

        char * third = malloc(strlen(first) + strlen(secnd) + 1);
        strcpy(third, first);
        strcat(third, secnd);

        pushTrash(third);

        return makeString(third);
    }
    else if (!strcmp(mem[argv[1]].name, "null"))
    {
        int * arglist = makeIntPtr(argv[1]);

        char * first = objectGetAttr(mem[argv[0]], "value");
        char * secnd = objectGetAttr(q_function_string(1, arglist), "value");

        char * third = malloc(strlen(first) + strlen(secnd) + 1);
        strcpy(third, first);
        strcat(third, secnd);

        free(arglist);

        pushTrash(third);

        return makeString(third);
    }

    char * err = malloc(20 + strlen(mem[argv[1]].name) + 30 + 1);
    strcpy(err, "types 'string' and '");
    strcat(err, mem[argv[1]].name);
    strcat(err, "' are invalid operands for '+'");
    error(err, line_num);
}

Object __mul___string(int argc, int * argv)
{
    if (!strcmp(mem[argv[1]].name, "int"))
    {
        char * first = objectGetAttr(mem[argv[0]], "value");
        int * secnd = objectGetAttr(mem[argv[1]], "value");

        char * third = malloc(strlen(first) + 1);
        strcpy(third, first);

        for (int i = 0; i < secnd[0]; i++)
        {
            mstrcat(&third, first);
        }

        pushTrash(third);

        return makeString(third);
    }

    char * err = malloc(20 + strlen(mem[argv[1]].name) + 30 + 1);
    strcpy(err, "types 'string' and '");
    strcat(err, mem[argv[1]].name);
    strcat(err, "' are invalid operands for '*'");
    error(err, line_num);
}

Object __eq___string(int argc, int * argv)
{
    if (strcmp(mem[argv[1]].name, "string"))
    {
        char * err = malloc(20 + strlen(mem[argv[1]].name) + 31 + 1);
        strcpy(err, "types 'string' and '");
        strcat(err, mem[argv[1]].name);
        strcat(err, "' can not be compared with '=='");
        error(err, line_num);
    }

    char * first = objectGetAttr(mem[argv[0]], "value");
    char * secnd = objectGetAttr(mem[argv[1]], "value");

    if (!strcmp(first, secnd))
        return makeInt(&truePtr);
    return makeInt(&falsePtr);
}

Object __index___string(int argc, int * argv)
{
    if (strcmp(mem[argv[1]].name, "int"))
    {
        char * err = malloc(42 + strlen(mem[argv[1]].name) + 1 + 1);
        strcpy(err, "string index argument must be 'int', not '");
        strcat(err, mem[argv[1]].name);
        strcat(err, "'");
        error(err, line_num);
    }

    int ind = ((int *)objectGetAttr(mem[argv[1]], "value"))[0];
    int length = strlen((char *)objectGetAttr(mem[argv[0]], "value"));

    // If index is -1, retrieve length - 1
    if (ind < 0)
        ind = length + ind;

    if (ind >= length)
        return makeString("");
    if (ind < 0)
        return makeString("");

    char ch = ((char *)objectGetAttr(mem[argv[0]], "value"))[ind];

    char * chst = malloc(2);
    chst[0] = ch;
    chst[1] = '\0';

    pushTrash(chst);

    return makeString(chst);
}

Object __copy___string(int argc, int * argv)
{
    return makeString((char *)objectGetAttr(mem[argv[0]], "value"));
}

Object __len___string(int argc, int * argv)
{
    long long len = strlen((char *)objectGetAttr(mem[argv[0]], "value"));

    return makeLong(makeLLPtr(len));
}

Object __disp___string(int argc, int * argv)
{
    char * selftext = (char *)objectGetAttr(mem[argv[0]], "value");
    char * rawst = makeRawString(selftext);
    pushTrash(rawst);

    return makeString(rawst);
}

Object __bool___string(int argc, int * argv)
{
    char * thisvalue = ((char *)objectGetAttr(mem[argv[0]], "value"));

    if (strlen(thisvalue))
        return makeInt(&truePtr);
    return makeInt(&falsePtr);
}

Object __string___string(int argc, int * argv)
{
    return mem[argv[0]];
}

Object makeString(char * value)
{
    Object self;

    self = makeObject("string", value);

    // Two argument methods

    // __add__
    self = addObjectValue(self, "__add__", &__add___string);
    self = addObjectValue(self, "__add__argc", &twoArgc);

    // __mul__
    self = addObjectValue(self, "__mul__", &__mul___string);
    self = addObjectValue(self, "__mul__argc", &twoArgc);

    // __eq__
    self = addObjectValue(self, "__eq__", &__eq___string);
    self = addObjectValue(self, "__eq__argc", &twoArgc);

    // __index__
    self = addObjectValue(self, "__index__", &__index___string);
    self = addObjectValue(self, "__index__argc", &twoArgc);

    // One argument methods

    // __copy__
    self = addObjectValue(self, "__copy__", &__copy___string);
    self = addObjectValue(self, "__copy__argc", &oneArgc);

    // __len__
    self = addObjectValue(self, "__len__", &__len___string);
    self = addObjectValue(self, "__len__argc", &oneArgc);

    // __disp__
    self = addObjectValue(self, "__disp__", &__disp___string);
    self = addObjectValue(self, "__disp__argc", &oneArgc);

    // __bool__
    self = addObjectValue(self, "__bool__", &__bool___string);
    self = addObjectValue(self, "__bool__argc", &oneArgc);

    // __string__
    self = addObjectValue(self, "__string__", &__string___string);
    self = addObjectValue(self, "__string__argc", &oneArgc);

    return self;
}
