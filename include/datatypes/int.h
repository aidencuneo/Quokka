Object __add___integer(Object * argv)
{
    if (strcmp(argv[1].name, "int"))
    {
        char * err = malloc(17 + strlen(argv[1].name) + 30 + 1);
        strcpy(err, "types 'int' and '");
        strcat(err, argv[1].name);
        strcat(err, "' are invalid operands for '+'");
        error(err, line_num);
    }

    int * first = (int *)objectGetAttr(argv[0], "value");
    int * secnd = (int *)objectGetAttr(argv[1], "value");
    return makeInteger(makeIntPtr(first[0] + secnd[0]));
}

Object __sub___integer(Object * argv)
{
    if (strcmp(argv[1].name, "int"))
    {
        char * err = malloc(17 + strlen(argv[1].name) + 30 + 1);
        strcpy(err, "types 'int' and '");
        strcat(err, argv[1].name);
        strcat(err, "' are invalid operands for '-'");
        error(err, line_num);
    }

    int * first = (int *)objectGetAttr(argv[0], "value");
    int * secnd = (int *)objectGetAttr(argv[1], "value");
    return makeInteger(makeIntPtr(first[0] - secnd[0]));
}

Object __mul___integer(Object * argv)
{
    if (strcmp(argv[1].name, "int"))
    {
        char * err = malloc(17 + strlen(argv[1].name) + 30 + 1);
        strcpy(err, "types 'int' and '");
        strcat(err, argv[1].name);
        strcat(err, "' are invalid operands for '*'");
        error(err, line_num);
    }

    int * first = (int *)objectGetAttr(argv[0], "value");
    int * secnd = (int *)objectGetAttr(argv[1], "value");
    return makeInteger(makeIntPtr(first[0] * secnd[0]));
}

Object __div___integer(Object * argv)
{
    if (strcmp(argv[1].name, "int"))
    {
        char * err = malloc(17 + strlen(argv[1].name) + 30 + 1);
        strcpy(err, "types 'int' and '");
        strcat(err, argv[1].name);
        strcat(err, "' are invalid operands for '/'");
        error(err, line_num);
    }

    int * first = (int *)objectGetAttr(argv[0], "value");
    int * secnd = (int *)objectGetAttr(argv[1], "value");

    if (!secnd[0])
        return makeInteger(makeIntPtr(0));

    return makeInteger(makeIntPtr(first[0] / secnd[0]));
}

Object __pow___integer(Object * argv)
{
    if (strcmp(argv[1].name, "int"))
    {
        char * err = malloc(17 + strlen(argv[1].name) + 31 + 1);
        strcpy(err, "types 'int' and '");
        strcat(err, argv[1].name);
        strcat(err, "' are invalid operands for '**'");
        error(err, line_num);
    }

    int * first = (int *)objectGetAttr(argv[0], "value");
    int * secnd = (int *)objectGetAttr(argv[1], "value");

    if (!secnd[0])
        return makeInteger(makeIntPtr(0));
    return makeInteger(makeIntPtr(ipowMath(first[0], secnd[0])));
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
    }

    int * first = (int *)objectGetAttr(argv[0], "value");
    int * secnd = (int *)objectGetAttr(argv[1], "value");

    if (!secnd[0])
        return makeInteger(makeIntPtr(0));

    if (first[0] < secnd[0])
        return makeInteger(&truePtr);
    return makeInteger(&falsePtr);
}

Object __le___integer(Object * argv)
{
    if (strcmp(argv[1].name, "int"))
    {
        char * err = malloc(17 + strlen(argv[1].name) + 31 + 1);
        strcpy(err, "types 'int' and '");
        strcat(err, argv[1].name);
        strcat(err, "' can not be compared with '<='");
        error(err, line_num);
    }

    int * first = (int *)objectGetAttr(argv[0], "value");
    int * secnd = (int *)objectGetAttr(argv[1], "value");

    if (!secnd[0])
        return makeInteger(makeIntPtr(0));

    if (first[0] <= secnd[0])
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
    }

    int * first = (int *)objectGetAttr(argv[0], "value");
    int * secnd = (int *)objectGetAttr(argv[1], "value");

    if (!secnd[0])
        return makeInteger(makeIntPtr(0));

    if (first[0] > secnd[0])
        return makeInteger(&truePtr);
    return makeInteger(&falsePtr);
}

Object __ge___integer(Object * argv)
{
    if (strcmp(argv[1].name, "int"))
    {
        char * err = malloc(17 + strlen(argv[1].name) + 31 + 1);
        strcpy(err, "types 'int' and '");
        strcat(err, argv[1].name);
        strcat(err, "' can not be compared with '>='");
        error(err, line_num);
    }

    int * first = (int *)objectGetAttr(argv[0], "value");
    int * secnd = (int *)objectGetAttr(argv[1], "value");

    if (!secnd[0])
        return makeInteger(makeIntPtr(0));

    if (first[0] >= secnd[0])
        return makeInteger(&truePtr);
    return makeInteger(&falsePtr);
}

Object __pos___integer(Object * argv)
{
    return argv[0];
}

Object __neg___integer(Object * argv)
{
    int * first = (int *)objectGetAttr(argv[0], "value");

    return makeInteger(makeIntPtr(-first[0]));
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
    return makeString(intToStr(thisvalue[0]));
}

Object makeInteger(int * value)
{
    Object self;

    self = makeObject("int", value);

    // Two argument methods

    // __add__
    self = addObjectValue(self, "__add__", &__add___integer);
    self = addObjectValue(self, "__add__argc", &twoArgc);

    // __sub__
    self = addObjectValue(self, "__sub__", &__sub___integer);
    self = addObjectValue(self, "__sub__argc", &twoArgc);

    // __mul__
    self = addObjectValue(self, "__mul__", &__mul___integer);
    self = addObjectValue(self, "__mul__argc", &twoArgc);

    // __div__
    self = addObjectValue(self, "__div__", &__div___integer);
    self = addObjectValue(self, "__div__argc", &twoArgc);

    // __pow__
    self = addObjectValue(self, "__pow__", &__pow___integer);
    self = addObjectValue(self, "__pow__argc", &twoArgc);

    // __eq__
    self = addObjectValue(self, "__eq__", &__eq___integer);
    self = addObjectValue(self, "__eq__argc", &twoArgc);

    // __lt__
    self = addObjectValue(self, "__lt__", &__lt___integer);
    self = addObjectValue(self, "__lt__argc", &twoArgc);

    // __le__
    self = addObjectValue(self, "__le__", &__le___integer);
    self = addObjectValue(self, "__le__argc", &twoArgc);

    // __gt__
    self = addObjectValue(self, "__gt__", &__gt___integer);
    self = addObjectValue(self, "__gt__argc", &twoArgc);

    // __ge__
    self = addObjectValue(self, "__ge__", &__ge___integer);
    self = addObjectValue(self, "__ge__argc", &twoArgc);

    // One argument methods

    // __pos__
    self = addObjectValue(self, "__pos__", &__pos___integer);
    self = addObjectValue(self, "__pos__argc", &oneArgc);

    // __neg__
    self = addObjectValue(self, "__neg__", &__neg___integer);
    self = addObjectValue(self, "__neg__argc", &oneArgc);

    // __bool__
    self = addObjectValue(self, "__bool__", &__bool___integer);
    self = addObjectValue(self, "__bool__argc", &oneArgc);

    // __str__
    self = addObjectValue(self, "__str__", &__str___integer);
    self = addObjectValue(self, "__str__argc", &oneArgc);

    return self;
}
