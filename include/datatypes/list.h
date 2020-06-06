Object * __setindex___list(int argc, Object ** argv)
{
    if (strcmp(argv[1]->name, "int"))
    {
        char * err = malloc(40 + strlen(argv[1]->name) + 1 + 1);
        strcpy(err, "list index argument must be 'int', not '");
        strcat(err, argv[1]->name);
        strcat(err, "'");
        error(err, line_num);
    }

    int ind = ((int *)objectGetAttr(argv[1], "value"))[0];
    int length = ((int *)objectGetAttr(argv[0], "length"))[0];

    // If index is -1, use length - 1
    if (ind < 0)
        ind = length + ind;

    // Check bounds
    if (ind >= length)
    {
        char * indstr = intToStr(ind);
        char * lenstr = intToStr(length);

        char * err = malloc(25 + strlen(indstr) + 4 + strlen(lenstr) + 1);
        strcpy(err, "list index out of range, ");
        strcat(err, indstr);
        strcat(err, " >= ");
        strcat(err, lenstr);

        free(indstr);
        free(lenstr);

        error(err, line_num);
    }
    if (ind < 0)
    {
        char * indstr = intToStr(ind);

        char * err = malloc(25 + strlen(indstr) + 4 + 1);
        strcpy(err, "list index out of range, ");
        strcat(err, indstr);
        strcat(err, " < 0");

        free(indstr);

        error(err, line_num);
    }

    // Set the index
    ((Object **)objectGetAttr(argv[0], "value"))[ind] = argv[2];

    // Push a COPY of this Object * to memory

    return argv[0];
}

Object * __add___list(int argc, Object ** argv)
{
    if (!strcmp(argv[1]->name, "list"))
    {
        Object ** first = objectGetAttr(argv[0], "value");
        Object ** secnd = objectGetAttr(argv[1], "value");

        int firstlen = ((int *)objectGetAttr(argv[0], "length"))[0];
        int secndlen = ((int *)objectGetAttr(argv[1], "length"))[0];

        Object ** third = malloc((firstlen + secndlen) * sizeof(Object *));

        // Add first list to final list
        for (int i = 0; i < firstlen; i++)
        {
            third[i] = first[i];
            third[i]->refs++;
        }

        // Add second list to final list
        for (int i = 0; i < secndlen; i++)
        {
            third[firstlen + i] = secnd[i];
            third[firstlen + i]->refs++;
        }

        Object * ret = makeList(firstlen + secndlen, third, 0);

        free(third);

        return ret;
    }

    char * err = malloc(18 + strlen(argv[1]->name) + 30 + 1);
    strcpy(err, "types 'list' and '");
    strcat(err, argv[1]->name);
    strcat(err, "' are invalid operands for '+'");
    error(err, line_num);

    return makeNull();
}

Object * __index___list(int argc, Object ** argv)
{
    if (strcmp(argv[1]->name, "int"))
    {
        char * err = malloc(40 + strlen(argv[1]->name) + 1 + 1);
        strcpy(err, "list index argument must be 'int', not '");
        strcat(err, argv[1]->name);
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
    if (ind < 0)
        return makeNull();

    Object * obj = ((Object **)objectGetAttr(argv[0], "value"))[ind];

    return obj;
}

Object * __sizeof___list(int argc, Object ** argv)
{
    Object ** thisvalue = objectGetAttr(argv[0], "value");
    int * length = objectGetAttr(argv[0], "length");

    int * size = makeIntPtr(sizeof(argv[0]) + (sizeof(thisvalue[0]) * length[0]));

    return makeInt(size, 1);
}

Object * __copy___list(int argc, Object ** argv)
{
    return makeList(
        ((int *)objectGetAttr(argv[0], "length"))[0],
        objectGetAttr(argv[0], "value"),
        0); // 0 so it doesn't flip
}

Object * __len___list(int argc, Object ** argv)
{
    int * length = objectGetAttr(argv[0], "length");

    return makeInt(makeIntPtr(length[0]), 1);
}

Object * __bool___list(int argc, Object ** argv)
{
    int length = ((int *)objectGetAttr(argv[0], "length"))[0];

    if (length)
        return makeInt(&truePtr, 0);
    return makeInt(&falsePtr, 0);
}

Object * __disp___list(int argc, Object ** argv)
{
    char * out = malloc(2);
    strcpy(out, "[");

    Object ** lst = objectGetAttr(argv[0], "value");
    int lstlen = ((int *)objectGetAttr(argv[0], "length"))[0];

    for (int p = 0; p < lstlen; p++)
    {
        Object ** arglist = makeArglist(lst[p]);

        Object * disp = q_function_display(1, arglist);

        free(arglist);

        mstrcat(&out, objectGetAttr(disp, "value"));
        if (p + 1 < lstlen)
            mstrcat(&out, ", ");

        freeObject(disp);
    }

    mstrcat(&out, "]");

    return makeString(out, 1);
}

Object * __free___list(int argc, Object ** argv)
{
    Object ** thisvalue = objectGetAttr(argv[0], "value");
    int * length = objectGetAttr(argv[0], "length");

    for (int i = 0; i < length[0]; i++)
    {
        // Unreference each Object inside this list
        objUnref(thisvalue[i]);
    }

    free(thisvalue);
    free(length);

    return makeNull();
}

Object * makeList(int length, Object ** value, int flipped)
{
    Object ** lst = malloc(length * sizeof(Object *));

    // Contents of this list will be freed after program execution
    // pushTrash(lst);

    // Reverse items before creating list
    for (int i = 0; i < length; i++)
    {
        if (flipped)
            lst[length - i - 1] = value[i];
        else
            lst[i] = value[i];
    }

    int * len_ptr = makeIntPtr(length);

    Object * self = objectPointer();

    self->name = "list";

    // 21 Attributes
    self->names = malloc(21 * sizeof(char *));
    self->values = malloc(21 * sizeof(void *));
    self->value_count = 0;

    // Values
    self = objectAddAttr(self, "value", lst);
    self = objectAddAttr(self, "length", len_ptr);

    // Three argument methods

    // __setindex__
    self = objectAddAttr(self, "__setindex__", &__setindex___list);
    self = objectAddAttr(self, "__setindex__argc", &threeArgc);

    // Two argument methods

    // __add__
    self = objectAddAttr(self, "__add__", &__add___list);
    self = objectAddAttr(self, "__add__argc", &twoArgc);

    // __index__
    self = objectAddAttr(self, "__index__", &__index___list);
    self = objectAddAttr(self, "__index__argc", &twoArgc);

    // One argument methods

    // __sizeof__
    self = objectAddAttr(self, "__sizeof__", &__sizeof___list);
    self = objectAddAttr(self, "__sizeof__argc", &oneArgc);

    // __copy__
    self = objectAddAttr(self, "__copy__", &__copy___list);
    self = objectAddAttr(self, "__copy__argc", &oneArgc);

    // __len__
    self = objectAddAttr(self, "__len__", &__len___list);
    self = objectAddAttr(self, "__len__argc", &oneArgc);

    // __bool__
    self = objectAddAttr(self, "__bool__", &__bool___list);
    self = objectAddAttr(self, "__bool__argc", &oneArgc);

    // __disp__
    self = objectAddAttr(self, "__disp__", &__disp___list);
    self = objectAddAttr(self, "__disp__argc", &oneArgc);

    // __string__
    self = objectAddAttr(self, "__string__", &__disp___list);
    self = objectAddAttr(self, "__string__argc", &oneArgc);

    // __free__
    self = objectAddAttr(self, "__free__", &__free___list);

    return self;
}
