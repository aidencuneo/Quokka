Object * __add___string(int argc, Object ** argv)
{
    if (!strcmp(argv[1]->name, "string"))
    {
        char * first = objectGetAttr(argv[0], "value");
        char * secnd = objectGetAttr(argv[1], "value");

        char * third = malloc(strlen(first) + strlen(secnd) + 1);
        strcpy(third, first);
        strcat(third, secnd);

        return makeString(third, 1);
    }
    else
    {
        char * first = objectGetAttr(argv[0], "value");
        char * secnd = q_function_string(1, &argv[1])->values[0];

        char * third = malloc(strlen(first) + strlen(secnd) + 1);
        strcpy(third, first);
        strcat(third, secnd);

        return makeString(third, 1);
    }

    char * err = malloc(20 + strlen(argv[1]->name) + 30 + 1);
    strcpy(err, "types 'string' and '");
    strcat(err, argv[1]->name);
    strcat(err, "' are invalid operands for '+'");
    error(err, line_num);

    return makeNull();
}

Object * __mul___string(int argc, Object ** argv)
{
    if (!strcmp(argv[1]->name, "int"))
    {
        char * first = objectGetAttr(argv[0], "value");
        int * secnd = objectGetAttr(argv[1], "value");

        char * third = malloc(strlen(first) + 1);
        strcpy(third, "");

        for (int i = 0; i < secnd[0]; i++)
            mstrcat(&third, first);

        return makeString(third, 1);
    }

    char * err = malloc(20 + strlen(argv[1]->name) + 30 + 1);
    strcpy(err, "types 'string' and '");
    strcat(err, argv[1]->name);
    strcat(err, "' are invalid operands for '*'");
    error(err, line_num);

    return makeNull();
}

Object * __eq___string(int argc, Object ** argv)
{
    if (!strcmp(argv[1]->name, "string"))
    {
        char * first = argv[0]->values[0];
        char * secnd = argv[1]->values[0];

        if (!strcmp(first, secnd))
            return getIntConst(1);
    }

    return getIntConst(0);
}

Object * __index___string(int argc, Object ** argv)
{
    if (strcmp(argv[1]->name, "int"))
    {
        char * err = malloc(42 + strlen(argv[1]->name) + 1 + 1);
        strcpy(err, "string index argument must be 'int', not '");
        strcat(err, argv[1]->name);
        strcat(err, "'");
        error(err, line_num);
    }

    char * thisvalue = argv[0]->values[0];
    int ind = *(int *)argv[1]->values[0];
    int length = strlen(thisvalue);

    // If index is -1, retrieve length - 1
    if (ind < 0)
        ind = length + ind;

    if (ind >= length)
    {
        char * chst = malloc(1);
        chst[0] = '\0';

        return makeString(chst, 1);
    }

    if (ind < 0)
    {
        char * chst = malloc(1);
        chst[0] = '\0';

        return makeString(chst, 1);
    }

    char ch = thisvalue[ind];

    char * chst = malloc(2);
    chst[0] = ch;
    chst[1] = '\0';

    return makeString(chst, 1);
}

Object * __sizeof___string(int argc, Object ** argv)
{
    char * thisvalue = argv[0]->values[0];

    int * size = makeIntPtr(sizeof(argv[0]) + strlen(thisvalue));

    return makeInt(size, 1, 10);
}

Object * __copy___string(int argc, Object ** argv)
{
    char * thisvalue = argv[0]->values[0];

    return makeString(strdup(thisvalue), 1);
}

Object * __len___string(int argc, Object ** argv)
{
    long long len = strlen(argv[0]->values[0]);

    int * size = makeIntPtr(len);

    return makeInt(size, 1, 10);
}

Object * __disp___string(int argc, Object ** argv)
{
    char * selftext = argv[0]->values[0];
    char * rawst = makeRawString(selftext);

    return makeString(rawst, 1);
}

Object * __bool___string(int argc, Object ** argv)
{
    char * thisvalue = argv[0]->values[0];

    if (strlen(thisvalue))
        return getIntConst(1);
    return getIntConst(0);
}

Object * __int___string(int argc, Object ** argv)
{
    char * thisvalue = argv[0]->values[0];

    int * toint = makeIntPtr(strtol(thisvalue, NULL, 10));

    return makeInt(toint, 1, 10);
}

Object * __string___string(int argc, Object ** argv)
{
    // Return a new string with the same contents as this one

    // (the rest of the stuff doesn't matter, since the string()
    // function call is expected to return a new string anyway,
    // so we won't need to go through the trouble of deep
    // copying this string)

    char * thisvalue = argv[0]->values[0];

    return makeString(strdup(thisvalue), 1);
}

Object * __free___string(int argc, Object ** argv)
{
    char * thisvalue = argv[0]->values[0];
    free(thisvalue);

    return NULL;
}

Object * upper_string(int argc, Object ** argv)
{
    char * thisvalue = argv[0]->values[0];
    int len = strlen(thisvalue);

    char * new_string = malloc(len + 1);

    int i;
    for (i = 0; i < len; i++)
        new_string[i] = toupper(thisvalue[i]);

    // NULL Byte
    new_string[i] = 0;

    return makeString(new_string, 1);
}

Object * lower_string(int argc, Object ** argv)
{
    char * thisvalue = argv[0]->values[0];
    int len = strlen(thisvalue);

    char * new_string = malloc(len + 1);

    int i;
    for (i = 0; i < len; i++)
        new_string[i] = tolower(thisvalue[i]);

    // NULL Byte
    new_string[i] = 0;

    return makeString(new_string, 1);
}

Object * isupper_string(int argc, Object ** argv)
{
    char * thisvalue = argv[0]->values[0];
    int len = strlen(thisvalue);

    for (int i = 0; i < len; i++)
        if (islower(thisvalue[i])) // If anything is lower, return false
            return getIntConst(0);

    return getIntConst(1);
}

Object * islower_string(int argc, Object ** argv)
{
    char * thisvalue = argv[0]->values[0];
    int len = strlen(thisvalue);

    for (int i = 0; i < len; i++)
        if (isupper(thisvalue[i])) // If anything is upper, return false
            return getIntConst(0);

    return getIntConst(1);
}

Object * strip_string(int argc, Object ** argv)
{
    char * checkchars = _strutil_whitespace;
    if (argc > 1)
        checkchars = q_function_string(1, &argv[1])->values[0];

    char * thisvalue = argv[0]->values[0];
    int len = strlen(thisvalue);
    char * newval = malloc(len + 1);

    int ind = 0;
    int start = 0;
    int stop = len - 1;

    while (start < len && stringHasChar(checkchars, thisvalue[start]))
        start++;
    while (stop >= 0 && stringHasChar(checkchars, thisvalue[stop]))
        stop--;

    for (int i = start; i <= stop; i++)
        newval[ind++] = thisvalue[i];

    newval[ind] = 0;

    return makeString(newval, 1);
}

Object * makeString(char * value, int is_malloc_ptr)
{
    // If length of string is 0, return the empty string constant
    // if (!value[0])
    // {
    //     if (is_malloc_ptr)
    //         free(value);
    //     return constants[1];
    // }

    return makeStringRaw(value, is_malloc_ptr);
}

Object * makeStringRaw(char * value, int is_malloc_ptr)
{
    Object * self = objectPointer();

    self->name = "string";

    // 1 to 2 Attributes
    int max_attr_count = 2;
    if (is_malloc_ptr)
    {
        self->names = malloc(max_attr_count * sizeof(char *));
        self->values = malloc(max_attr_count * sizeof(void *));
    }
    else
    {
        self->names = malloc((max_attr_count - 1) * sizeof(char *));
        self->values = malloc((max_attr_count - 1) * sizeof(void *));
    }

    self = objectAddAttr(self, "value", value);

    // __free__
    if (is_malloc_ptr)
        self = objectAddAttr(self, "__free__", &__free___string);

    // Regular methods (must go at end)

    return self;
}
