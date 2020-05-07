Object __add___string(Object * argv)
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

Object __eq___string(Object * argv)
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

Object makeString(char * value)
{
    Object self;

    self = makeObject("string", value);

    // __add__
    self = addObjectValue(self, "__add__argc", &twoArgc);
    self = addObjectValue(self, "__add__", &__add___string);

    // __eq__
    self = addObjectValue(self, "__eq__argc", &twoArgc);
    self = addObjectValue(self, "__eq__", &__eq___string);

    return self;
}
