Object __add___list(int argc, Object * argv)
{
    if (strcmp(argv[1].name, "list"))
    {
        char * err = malloc(18 + strlen(argv[1].name) + 30 + 1);
        strcpy(err, "types 'list' and '");
        strcat(err, argv[1].name);
        strcat(err, "' are invalid operands for '+'");
        error(err, line_num);
    }

    Object * first = (Object *)objectGetAttr(argv[0], "value");
    Object * secnd = (Object *)objectGetAttr(argv[1], "value");

    int firstlen = ((int *)objectGetAttr(argv[0], "length"))[0];
    int secndlen = ((int *)objectGetAttr(argv[1], "length"))[0];

    Object * third = malloc((firstlen + secndlen) * sizeof(Object));

    // Add first list to final list
    for (int i = 0; i < firstlen; i++)
        third[i] = first[i];

    // Add second list to final list
    for (int i = 0; i < secndlen; i++)
        third[firstlen + i] = secnd[i];

    return makeList(firstlen + secndlen, third, 0);
}

Object __index___list(int argc, Object * argv)
{
    if (strcmp(argv[1].name, "int"))
    {
        char * err = malloc(40 + strlen(argv[1].name) + 1 + 1);
        strcpy(err, "list index argument must be 'int', not '");
        strcat(err, argv[1].name);
        strcat(err, "'");
        error(err, line_num);
    }

    int ind = ((int *)objectGetAttr(argv[1], "value"))[0];
    int length = ((int *)objectGetAttr(argv[0], "length"))[0];

    // If index is -1, retrieve length - 1
    if (ind < 0)
        ind = length + ind;

    // Check bounds
    if (ind >= length)
        return makeNull();
    else if (ind < 0)
        return makeNull();

    Object obj = ((Object *)objectGetAttr(argv[0], "value"))[ind];

    return obj;
}

Object __copy___list(int argc, Object * argv)
{
    return makeList(
        ((int *)objectGetAttr(argv[0], "length"))[0],
        (Object *)objectGetAttr(argv[0], "value"),
        0); // 0 so it doesn't flip
}

Object __bool___list(int argc, Object * argv)
{
    int length = ((int *)objectGetAttr(argv[0], "length"))[0];

    if (length)
        return makeInteger(&truePtr);
    return makeInteger(&falsePtr);
}

Object makeList(int length, Object * value, int flipped)
{
    Object self;

    Object * lst = malloc(length * sizeof(Object));

    // Reverse items before creating list
    for (int i = 0; i < length; i++)
    {
        if (flipped)
            lst[length - i - 1] = value[i];
        else
            lst[i] = value[i];
    }

    self = makeObject("list", lst);
    self = addObjectValue(self, "length", makeIntPtr(length));

    // Two argument methods

    // __add__
    self = addObjectValue(self, "__add__", &__add___list);
    self = addObjectValue(self, "__add__argc", &twoArgc);

    // __index__
    self = addObjectValue(self, "__index__", &__index___list);
    self = addObjectValue(self, "__index__argc", &twoArgc);

    // __copy__
    self = addObjectValue(self, "__copy__", &__copy___list);
    self = addObjectValue(self, "__copy__argc", &twoArgc);

    // One argument methods

    // __bool__
    self = addObjectValue(self, "__bool__", &__bool___list);
    self = addObjectValue(self, "__bool__argc", &oneArgc);

    return self;
}
