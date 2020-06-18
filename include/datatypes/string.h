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
        Object ** arglist = makeArglist(argv[1]);

        char * first = objectGetAttr(argv[0], "value");
        char * secnd = objectGetAttr(q_function_string(1, arglist), "value");

        char * third = malloc(strlen(first) + strlen(secnd) + 1);
        strcpy(third, first);
        strcat(third, secnd);

        free(arglist);

        return makeString(third, 1);
    }

    char * err = malloc(20 + strlen(argv[1]->name) + 30 + 1);
    strcpy(err, "types 'string' and '");
    strcat(err, argv[1]->name);
    strcat(err, "' are invalid operands for '+'");
    error(err, line_num);

    return makeNull();
}

Object * __inadd___string(int argc, Object ** argv)
{
    if (!strcmp(argv[1]->name, "string"))
    {
        int firstind = objectGetAttrIndex(argv[0], "value");
        char * first = argv[0]->values[firstind];
        char * secnd = objectGetAttr(argv[1], "value");

        first = realloc(first, strlen(first) + strlen(secnd) + 1);
        strcat(first, secnd);

        argv[0]->values[firstind] = first;
    }
    else
    {
        Object ** arglist = makeArglist(argv[1]);

        int firstind = objectGetAttrIndex(argv[0], "value");
        char * first = argv[0]->values[firstind];
        char * secnd = objectGetAttr(q_function_string(1, arglist), "value");

        first = realloc(first, strlen(first) + strlen(secnd) + 1);
        strcat(first, secnd);

        argv[0]->values[firstind] = first;

        free(arglist);
    }

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
        {
            mstrcat(&third, first);
        }

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
    if (strcmp(argv[1]->name, "string"))
    {
        char * err = malloc(20 + strlen(argv[1]->name) + 31 + 1);
        strcpy(err, "types 'string' and '");
        strcat(err, argv[1]->name);
        strcat(err, "' can not be compared with '=='");
        error(err, line_num);
    }

    char * first = objectGetAttr(argv[0], "value");
    char * secnd = objectGetAttr(argv[1], "value");

    if (!strcmp(first, secnd))
        return makeInt(&truePtr, 0);
    return makeInt(&falsePtr, 0);
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

    int ind = ((int *)objectGetAttr(argv[1], "value"))[0];
    int length = strlen((char *)objectGetAttr(argv[0], "value"));

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

    char ch = ((char *)objectGetAttr(argv[0], "value"))[ind];

    char * chst = malloc(2);
    chst[0] = ch;
    chst[1] = '\0';

    return makeString(chst, 1);
}

Object * __sizeof___string(int argc, Object ** argv)
{
    char * thisvalue = ((char *)objectGetAttr(argv[0], "value"));

    int * size = makeIntPtr(sizeof(argv[0]) + strlen(thisvalue));

    return makeInt(size, 1);
}

Object * __copy___string(int argc, Object ** argv)
{
    char * thisvalue = (char *)objectGetAttr(argv[0], "value");

    return makeString(strdup(thisvalue), 1);
}

Object * __len___string(int argc, Object ** argv)
{
    long long len = strlen((char *)objectGetAttr(argv[0], "value"));

    int * size = makeIntPtr(len);

    return makeInt(size, 1);
}

Object * __disp___string(int argc, Object ** argv)
{
    char * selftext = (char *)objectGetAttr(argv[0], "value");
    char * rawst = makeRawString(selftext);

    return makeString(rawst, 1);
}

Object * __bool___string(int argc, Object ** argv)
{
    char * thisvalue = ((char *)objectGetAttr(argv[0], "value"));

    if (strlen(thisvalue))
        return makeInt(&truePtr, 0);
    return makeInt(&falsePtr, 0);
}

Object * __int___string(int argc, Object ** argv)
{
    char * thisvalue = ((char *)objectGetAttr(argv[0], "value"));

    int * toint = makeIntPtr(strtol(thisvalue, NULL, 10));

    return makeInt(toint, 1);
}

Object * __long___string(int argc, Object ** argv)
{
    char * thisvalue = ((char *)objectGetAttr(argv[0], "value"));

    long long * tolong = makeLLPtr(strtoll(thisvalue, NULL, 10));

    return makeLong(tolong, 1);
}

Object * __string___string(int argc, Object ** argv)
{
    // Return a new string with the same contents as this one

    // (the rest of the stuff doesn't matter, since the string()
    // function call is expected to return a new string anyway,
    // so we won't need to go through the trouble of deep
    // copying this string)

    char * thisvalue = ((char *)objectGetAttr(argv[0], "value"));

    return makeString(strdup(thisvalue), 1);
}

Object * __free___string(int argc, Object ** argv)
{
    int method_start = objectGetAttrIndex(argv[0], "upper");

    for (int i = method_start; i < argv[0]->value_count; i++)
    {
        char * name = ((Object *)argv[0]->values[i])->name;

        // If current value is not a method, exit loop
        if (strcmp(name, "method") &&
            strcmp(name, "cmethod"))
            break;

        freeObject(argv[0]->values[i]);
    }

    return makeNull();
}

Object * __free_malloc___string(int argc, Object ** argv)
{
    char * thisvalue = objectGetAttr(argv[0], "value");
    free(thisvalue);

    return __free___string(argc, argv);
}

Object * upper_string(int argc, Object ** argv)
{
    char * thisvalue = objectGetAttr(argv[0], "value");
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
    char * thisvalue = objectGetAttr(argv[0], "value");
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
    char * thisvalue = objectGetAttr(argv[0], "value");
    int len = strlen(thisvalue);

    for (int i = 0; i < len; i++)
        if (islower(thisvalue[i])) // If anything is lower, return false
            return makeInt(&falsePtr, 0);

    return makeInt(&truePtr, 0);
}

Object * islower_string(int argc, Object ** argv)
{
    char * thisvalue = objectGetAttr(argv[0], "value");
    int len = strlen(thisvalue);

    for (int i = 0; i < len; i++)
        if (isupper(thisvalue[i])) // If anything is upper, return false
            return makeInt(&falsePtr, 0);

    return makeInt(&truePtr, 0);
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

    // 6 Attributes
    int attr_count = 6;
    self->names = malloc(attr_count * sizeof(char *));
    self->values = malloc(attr_count * sizeof(void *));

    self = objectAddAttr(self, "value", value);

    if (is_malloc_ptr)
        // __free__
        self = objectAddAttr(self, "__free__", &__free_malloc___string);
    else
        // __free__
        self = objectAddAttr(self, "__free__", &__free___string);

    // Regular methods (must go at end)

    // upper
    Object * upper_string_cfunc = makeCMethod(self, &upper_string, 0, 0);
    upper_string_cfunc->refs++;
    self = objectAddAttr(self, "upper", upper_string_cfunc);

    // lower
    Object * lower_string_cfunc = makeCMethod(self, &lower_string, 0, 0);
    lower_string_cfunc->refs++;
    self = objectAddAttr(self, "lower", lower_string_cfunc);

    // isupper
    Object * isupper_string_cfunc = makeCMethod(self, &isupper_string, 0, 0);
    isupper_string_cfunc->refs++;
    self = objectAddAttr(self, "isupper", isupper_string_cfunc);

    // islower
    Object * islower_string_cfunc = makeCMethod(self, &islower_string, 0, 0);
    islower_string_cfunc->refs++;
    self = objectAddAttr(self, "islower", islower_string_cfunc);

    return self;
}
