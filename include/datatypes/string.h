Object __add___string(int argc, Object * argv)
{
    if (strcmp(argv[1].name, "string"))
    {
        char * err = malloc(66 + strlen(argv[1].name) + 1 + 1);
        strcpy(err, "only 'string' and 'string' can be concatenated, not 'string' and '");
        strcat(err, argv[1].name);
        strcat(err, "'");
        error(err, line_num);
        exit(1);
    }

    char * first = (char *)objectGetAttr(argv[0], "value");
    char * secnd = (char *)objectGetAttr(argv[1], "value");

    char * third = malloc(strlen(first) + strlen(secnd) + 1);
    strcpy(third, first);
    strcat(third, secnd);

    return makeString(third);
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
        exit(1);
    }

    char * first = (char *)objectGetAttr(argv[0], "value");
    char * secnd = (char *)objectGetAttr(argv[1], "value");

    if (!strcmp(first, secnd))
        return makeInteger(&truePtr);
    return makeInteger(&falsePtr);
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
        exit(1);
    }

    int ind = ((int *)objectGetAttr(argv[1], "value"))[0];
    int len = strlen((char *)objectGetAttr(argv[0], "value"));

    if (ind >= len)
        return makeString("");
    else if (ind < 0)
        return makeString("");

    char ch = ((char *)objectGetAttr(argv[0], "value"))[ind];

    char * chst = malloc(2);
    chst[0] = ch;
    chst[1] = '\0';

    return makeString(chst);
}

Object __copy___string(int argc, Object * argv)
{
    return makeString((char *)objectGetAttr(argv[0], "value"));
}

Object __disp___string(int argc, Object * argv)
{
    char * selftext = (char *)objectGetAttr(argv[0], "value");

    return makeString(makeRawString(selftext));
}

Object __bool___string(int argc, Object * argv)
{
    char * thisvalue = ((char *)objectGetAttr(argv[0], "value"));

    if (strlen(thisvalue))
        return makeInteger(&truePtr);
    return makeInteger(&falsePtr);
}

Object __str___string(int argc, Object * argv)
{
    return argv[0];
}

Object makeString(char * value)
{
    Object self;

    self = makeObject("string", value);

    // Two argument methods

    // __add__
    self = addObjectValue(self, "__add__", &__add___string);
    self = addObjectValue(self, "__add__argc", &twoArgc);

    // __eq__
    self = addObjectValue(self, "__eq__", &__eq___string);
    self = addObjectValue(self, "__eq__argc", &twoArgc);

    // __index__
    self = addObjectValue(self, "__index__", &__index___string);
    self = addObjectValue(self, "__index__argc", &twoArgc);

    // __copy__
    self = addObjectValue(self, "__copy__", &__copy___string);
    self = addObjectValue(self, "__copy__argc", &twoArgc);

    // One argument methods

    // __disp__
    self = addObjectValue(self, "__disp__", &__disp___string);
    self = addObjectValue(self, "__disp__argc", &oneArgc);

    // __bool__
    self = addObjectValue(self, "__bool__", &__bool___string);
    self = addObjectValue(self, "__bool__argc", &oneArgc);

    // __str__
    self = addObjectValue(self, "__str__", &__str___string);
    self = addObjectValue(self, "__str__argc", &oneArgc);

    return self;
}
