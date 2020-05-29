Object __add___string(int argc, Object * argv)
{
    if (!strcmp(argv[1].name, "string"))
    {
        char * first = objectGetAttr(argv[0], "value");
        char * secnd = objectGetAttr(argv[1], "value");

        char * third = malloc(strlen(first) + strlen(secnd) + 1);
        strcpy(third, first);
        strcat(third, secnd);

        pushTrash(third);

        return makeString(third);
    }
    else if (!strcmp(argv[1].name, "null"))
    {
        Object * arglist = makeArglist(argv[1]);

        char * first = objectGetAttr(argv[0], "value");
        char * secnd = objectGetAttr(q_function_string(1, arglist), "value");

        char * third = malloc(strlen(first) + strlen(secnd) + 1);
        strcpy(third, first);
        strcat(third, secnd);

        free(arglist);

        pushTrash(third);

        return makeString(third);
    }

    char * err = malloc(20 + strlen(argv[1].name) + 30 + 1);
    strcpy(err, "types 'string' and '");
    strcat(err, argv[1].name);
    strcat(err, "' are invalid operands for '+'");
    error(err, line_num);

    return makeNull();
}

Object __mul___string(int argc, Object * argv)
{
    if (!strcmp(argv[1].name, "int"))
    {
        char * first = objectGetAttr(argv[0], "value");
        int * secnd = objectGetAttr(argv[1], "value");

        char * third = malloc(strlen(first) + 1);
        strcpy(third, "");

        for (int i = 0; i < secnd[0]; i++)
        {
            mstrcat(&third, first);
        }

        pushTrash(third);

        return makeString(third);
    }

    char * err = malloc(20 + strlen(argv[1].name) + 30 + 1);
    strcpy(err, "types 'string' and '");
    strcat(err, argv[1].name);
    strcat(err, "' are invalid operands for '*'");
    error(err, line_num);

    return makeNull();
}

Object __eq___string(int argc, Object * argv)
{
    if (strcmp(argv[1].name, "string"))
    {
        char * err = malloc(20 + strlen(argv[1].name) + 31 + 1);
        strcpy(err, "types 'string' and '");
        strcat(err, argv[1].name);
        strcat(err, "' can not be compared with '=='");
        error(err, line_num);
    }

    char * first = objectGetAttr(argv[0], "value");
    char * secnd = objectGetAttr(argv[1], "value");

    if (!strcmp(first, secnd))
        return makeInt(&truePtr);
    return makeInt(&falsePtr);
}

Object __index___string(int argc, Object * argv)
{
    if (strcmp(argv[1].name, "int"))
    {
        char * err = malloc(42 + strlen(argv[1].name) + 1 + 1);
        strcpy(err, "string index argument must be 'int', not '");
        strcat(err, argv[1].name);
        strcat(err, "'");
        error(err, line_num);
    }

    int ind = ((int *)objectGetAttr(argv[1], "value"))[0];
    int length = strlen((char *)objectGetAttr(argv[0], "value"));

    // If index is -1, retrieve length - 1
    if (ind < 0)
        ind = length + ind;

    if (ind >= length)
    {
        char * chst = malloc(1);
        chst[0] = '\0';

        pushTrash(chst);

        return makeString(chst);
    }

    if (ind < 0)
    {
        char * chst = malloc(1);
        chst[0] = '\0';

        pushTrash(chst);

        return makeString(chst);
    }

    char ch = ((char *)objectGetAttr(argv[0], "value"))[ind];

    char * chst = malloc(2);
    chst[0] = ch;
    chst[1] = '\0';

    pushTrash(chst);

    return makeString(chst);
}

Object __sizeof___string(int argc, Object * argv)
{
    char * thisvalue = ((char *)objectGetAttr(argv[0], "value"));

    int * size = makeIntPtr(sizeof(argv[0]) + strlen(thisvalue));
    pushTrash(size);

    return makeInt(size);
}

Object __copy___string(int argc, Object * argv)
{
    return makeString((char *)objectGetAttr(argv[0], "value"));
}

Object __len___string(int argc, Object * argv)
{
    long long len = strlen((char *)objectGetAttr(argv[0], "value"));

    int * size = makeIntPtr(len);
    pushTrash(size);

    return makeInt(size);
}

Object __disp___string(int argc, Object * argv)
{
    char * selftext = (char *)objectGetAttr(argv[0], "value");
    char * rawst = makeRawString(selftext);
    pushTrash(rawst);

    return makeString(rawst);
}

Object __bool___string(int argc, Object * argv)
{
    char * thisvalue = ((char *)objectGetAttr(argv[0], "value"));

    if (strlen(thisvalue))
        return makeInt(&truePtr);
    return makeInt(&falsePtr);
}

Object __int___string(int argc, Object * argv)
{
    char * thisvalue = ((char *)objectGetAttr(argv[0], "value"));

    int * toint = makeIntPtr(strtol(thisvalue, NULL, 10));
    pushTrash(toint);

    return makeInt(toint);
}

Object __long___string(int argc, Object * argv)
{
    char * thisvalue = ((char *)objectGetAttr(argv[0], "value"));

    long long * tolong = makeLLPtr(strtoll(thisvalue, NULL, 10));
    pushTrash(tolong);

    return makeLong(tolong);
}

Object __string___string(int argc, Object * argv)
{
    // Return a new string with the same contents as this one

    // (the rest of the stuff doesn't matter, since the string()
    // function call is expected to return a new string anyway,
    // so we won't need to go through the trouble of deep
    // copying this string)

    char * thisvalue = ((char *)objectGetAttr(argv[0], "value"));

    char * valcopy = strdup(thisvalue);
    pushTrash(valcopy);

    return makeString(valcopy);
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

    // __sizeof__
    self = addObjectValue(self, "__sizeof__", &__sizeof___string);
    self = addObjectValue(self, "__sizeof__argc", &oneArgc);

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

    // __int__
    self = addObjectValue(self, "__int__", &__int___string);
    self = addObjectValue(self, "__int__argc", &oneArgc);

    // __long__
    self = addObjectValue(self, "__long__", &__long___string);
    self = addObjectValue(self, "__long__argc", &oneArgc);

    // __string__
    self = addObjectValue(self, "__string__", &__string___string);
    self = addObjectValue(self, "__string__argc", &oneArgc);

    return self;
}
