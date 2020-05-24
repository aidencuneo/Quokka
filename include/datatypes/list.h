Object __add___list(int argc, int * argv)
{
    if (!strcmp(mem[argv[1]].name, "list"))
    {
        Object * first = objectGetAttr(mem[argv[0]], "value");
        Object * secnd = objectGetAttr(mem[argv[1]], "value");

        int firstlen = ((int *)objectGetAttr(mem[argv[0]], "length"))[0];
        int secndlen = ((int *)objectGetAttr(mem[argv[1]], "length"))[0];

        Object * third = malloc((firstlen + secndlen) * sizeof(Object));

        // Add first list to final list
        for (int i = 0; i < firstlen; i++)
            third[i] = objectCopy(first[i]);

        // Add second list to final list
        for (int i = 0; i < secndlen; i++)
            third[firstlen + i] = objectCopy(secnd[i]);

        return makeList(firstlen + secndlen, third, 0);
    }

    char * err = malloc(18 + strlen(mem[argv[1]].name) + 30 + 1);
    strcpy(err, "types 'list' and '");
    strcat(err, mem[argv[1]].name);
    strcat(err, "' are invalid operands for '+'");
    error(err, line_num);

    return makeNull();
}

Object __index___list(int argc, int * argv)
{
    if (strcmp(mem[argv[1]].name, "int"))
    {
        char * err = malloc(40 + strlen(mem[argv[1]].name) + 1 + 1);
        strcpy(err, "list index argument must be 'int', not '");
        strcat(err, mem[argv[1]].name);
        strcat(err, "'");
        error(err, line_num);
    }

    int ind = ((int *)objectGetAttr(mem[argv[1]], "value"))[0];
    int length = ((int *)objectGetAttr(mem[argv[0]], "length"))[0];

    // If index is -1, retrieve length - 1
    if (ind < 0)
        ind = length + ind;

    // Check bounds
    if (ind >= length)
        return makeNull();
    if (ind < 0)
        return makeNull();

    Object obj = ((Object *)objectGetAttr(mem[argv[0]], "value"))[ind];
    pushMem(obj);

    int * obj_ptr = makeIntPtr(memsize - 1);

    Object ret = makeInt(obj_ptr);

    pushTrash(obj_ptr);
    pushMem(ret);

    return ret;
}

Object __sizeof___list(int argc, int * argv)
{
    int * thisvalue = objectGetAttr(mem[argv[0]], "value");
    int * length = objectGetAttr(mem[argv[0]], "length");

    int * size = makeIntPtr(sizeof(mem[argv[0]]) + (sizeof(thisvalue[0]) * length[0]));
    pushTrash(size);

    return makeInt(size);
}

Object __copy___list(int argc, int * argv)
{
    return makeList(
        ((int *)objectGetAttr(mem[argv[0]], "length"))[0],
        objectGetAttr(mem[argv[0]], "value"),
        0); // 0 so it doesn't flip
}

Object __len___list(int argc, int * argv)
{
    return makeInt(objectGetAttr(mem[argv[0]], "length"));
}

Object __bool___list(int argc, int * argv)
{
    int length = ((int *)objectGetAttr(mem[argv[0]], "length"))[0];

    if (length)
        return makeInt(&truePtr);
    return makeInt(&falsePtr);
}

Object __disp___list(int argc, int * argv)
{
    char * out = malloc(2);
    strcpy(out, "[");

    Object * lst = objectGetAttr(mem[argv[0]], "value");
    int lstlen = ((int *)objectGetAttr(mem[argv[0]], "length"))[0];

    for (int p = 0; p < lstlen; p++)
    {
        pushMem(lst[p]);
        int ind = memsize - 1;

        int * arglist = makeIntPtr(ind);

        Object disp = q_function_display(1, arglist);

        popKeepMem(ind);

        free(arglist);

        mstrcat(&out, objectGetAttr(disp, "value"));
        if (p + 1 < lstlen)
            mstrcat(&out, ", ");

        freeObject(disp);
    }

    mstrcat(&out, "]");

    pushTrash(out);

    return makeString(out);
}

Object makeList(int length, Object * value, int flipped)
{
    Object self;

    Object * lst = malloc(length * sizeof(Object));

    // Contents of this list will be freed after program execution
    pushTrash(lst);

    // Reverse items before creating list
    for (int i = 0; i < length; i++)
    {
        if (flipped)
            lst[length - i - 1] = value[i];
        else
            lst[i] = value[i];
    }

    free(value);

    int * len_ptr = makeIntPtr(length);
    pushTrash(len_ptr);

    self = makeObject("list", lst);
    self = addObjectValue(self, "length", len_ptr);

    // Two argument methods

    // __add__
    self = addObjectValue(self, "__add__", &__add___list);
    self = addObjectValue(self, "__add__argc", &twoArgc);

    // __index__
    self = addObjectValue(self, "__index__", &__index___list);
    self = addObjectValue(self, "__index__argc", &twoArgc);

    // One argument methods

    // __sizeof__
    self = addObjectValue(self, "__sizeof__", &__sizeof___list);
    self = addObjectValue(self, "__sizeof__argc", &oneArgc);

    // __copy__
    self = addObjectValue(self, "__copy__", &__copy___list);
    self = addObjectValue(self, "__copy__argc", &oneArgc);

    // __len__
    self = addObjectValue(self, "__len__", &__len___list);
    self = addObjectValue(self, "__len__argc", &oneArgc);

    // __bool__
    self = addObjectValue(self, "__bool__", &__bool___list);
    self = addObjectValue(self, "__bool__argc", &oneArgc);

    // __disp__
    self = addObjectValue(self, "__disp__", &__disp___list);
    self = addObjectValue(self, "__disp__argc", &oneArgc);

    // __string__
    self = addObjectValue(self, "__string__", &__disp___list);
    self = addObjectValue(self, "__string__argc", &oneArgc);

    return self;
}
