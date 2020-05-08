Object __add___integer(Object * argv)
{
    if (strcmp(argv[1].name, "int"))
    {
        char * err = malloc(17 + strlen(argv[1].name) + 27 + 1);
        strcpy(err, "types 'int' and '");
        strcat(err, argv[1].name);
        strcat(err, "' can not be added with '+'");
        error(err, line_num);
        exit(1);
    }

    int * first = (int *)objectGetAttr(argv[0], "value");
    int * secnd = (int *)objectGetAttr(argv[1], "value");
    return makeInteger(makeIntPtr(first[0] + secnd[0]));
}

Object __sub___integer(Object * argv)
{
    if (strcmp(argv[1].name, "int"))
    {
        char * err = malloc(17 + strlen(argv[1].name) + 32 + 1);
        strcpy(err, "types 'int' and '");
        strcat(err, argv[1].name);
        strcat(err, "' can not be subtracted with '-'");
        error(err, line_num);
        exit(1);
    }

    int * first = (int *)objectGetAttr(argv[0], "value");
    int * secnd = (int *)objectGetAttr(argv[1], "value");
    return makeInteger(makeIntPtr(first[0] - secnd[0]));
}

Object __mul___integer(Object * argv)
{
    if (strcmp(argv[1].name, "int"))
    {
        char * err = malloc(17 + strlen(argv[1].name) + 32 + 1);
        strcpy(err, "types 'int' and '");
        strcat(err, argv[1].name);
        strcat(err, "' can not be multiplied with '*'");
        error(err, line_num);
        exit(1);
    }

    int * first = (int *)objectGetAttr(argv[0], "value");
    int * secnd = (int *)objectGetAttr(argv[1], "value");
    return makeInteger(makeIntPtr(first[0] * secnd[0]));
}

Object __div___integer(Object * argv)
{
    if (strcmp(argv[1].name, "int"))
    {
        char * err = malloc(17 + strlen(argv[1].name) + 29 + 1);
        strcpy(err, "types 'int' and '");
        strcat(err, argv[1].name);
        strcat(err, "' can not be divided with '/'");
        error(err, line_num);
        exit(1);
    }

    int * first = (int *)objectGetAttr(argv[0], "value");
    int * secnd = (int *)objectGetAttr(argv[1], "value");

    if (!secnd[0])
        return makeInteger(makeIntPtr(0));

    return makeInteger(makeIntPtr(first[0] / secnd[0]));
}

Object __eq___integer(Object * argv)
{
    if (strcmp(argv[1].name, "int"))
    {
        char * err = malloc(17 + strlen(argv[1].name) + 31 + 1);
        strcpy(err, "types 'int' and '");
        strcat(err, argv[1].name);
        strcat(err, "' can not be compared with '=='");
        error(err, line_num);
        exit(1);
    }

    int * first = (int *)objectGetAttr(argv[0], "value");
    int * secnd = (int *)objectGetAttr(argv[1], "value");

    if (!secnd[0])
        return makeInteger(makeIntPtr(0));

    if (first[0] == secnd[0])
        return makeInteger(&truePtr);
    return makeInteger(&falsePtr);
}

Object __lt___integer(Object * argv)
{
    if (strcmp(argv[1].name, "int"))
    {
        char * err = malloc(17 + strlen(argv[1].name) + 30 + 1);
        strcpy(err, "types 'int' and '");
        strcat(err, argv[1].name);
        strcat(err, "' can not be compared with '<'");
        error(err, line_num);
        exit(1);
    }

    int * first = (int *)objectGetAttr(argv[0], "value");
    int * secnd = (int *)objectGetAttr(argv[1], "value");

    if (!secnd[0])
        return makeInteger(makeIntPtr(0));

    if (first[0] < secnd[0])
        return makeInteger(&truePtr);
    return makeInteger(&falsePtr);
}

Object __gt___integer(Object * argv)
{
    if (strcmp(argv[1].name, "int"))
    {
        char * err = malloc(17 + strlen(argv[1].name) + 30 + 1);
        strcpy(err, "types 'int' and '");
        strcat(err, argv[1].name);
        strcat(err, "' can not be compared with '>'");
        error(err, line_num);
        exit(1);
    }

    int * first = (int *)objectGetAttr(argv[0], "value");
    int * secnd = (int *)objectGetAttr(argv[1], "value");

    if (!secnd[0])
        return makeInteger(makeIntPtr(0));

    if (first[0] > secnd[0])
        return makeInteger(&truePtr);
    return makeInteger(&falsePtr);
}

Object __bool___integer(Object * argv)
{
    int * thisvalue = ((int *)objectGetAttr(argv[0], "value"));

    if (thisvalue[0])
        return makeInteger(&truePtr);
    return makeInteger(&falsePtr);
}

Object __str___integer(Object * argv)
{
    int * thisvalue = ((int *)objectGetAttr(argv[0], "value"));
    return makeString(String(thisvalue[0]).value);
}

Object makeInteger(int * value)
{
    Object self;

    self = makeObject("int", value);

    // __add__
    self = addObjectValue(self, "__add__argc", &twoArgc);
    self = addObjectValue(self, "__add__", &__add___integer);

    // __sub__
    self = addObjectValue(self, "__sub__argc", &twoArgc);
    self = addObjectValue(self, "__sub__", &__sub___integer);

    // __mul__
    self = addObjectValue(self, "__mul__argc", &twoArgc);
    self = addObjectValue(self, "__mul__", &__mul___integer);

    // __div__
    self = addObjectValue(self, "__div__argc", &twoArgc);
    self = addObjectValue(self, "__div__", &__div___integer);

    // __eq__
    self = addObjectValue(self, "__eq__argc", &twoArgc);
    self = addObjectValue(self, "__eq__", &__eq___integer);

    // __lt__
    self = addObjectValue(self, "__lt__argc", &twoArgc);
    self = addObjectValue(self, "__lt__", &__lt___integer);

    // __gt__
    self = addObjectValue(self, "__gt__argc", &twoArgc);
    self = addObjectValue(self, "__gt__", &__gt___integer);

    // __bool__
    self = addObjectValue(self, "__bool__argc", &oneArgc);
    self = addObjectValue(self, "__bool__", &__bool___integer);

    // __str__
    self = addObjectValue(self, "__str__argc", &oneArgc);
    self = addObjectValue(self, "__str__", &__str___integer);

    return self;
}
