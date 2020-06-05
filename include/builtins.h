Object * q_function_println(int argc, Object ** argv);
Object * q_function_string(int argc, Object ** argv);

Object * q_function_display(int argc, Object ** argv)
{
    char * out = malloc(1);
    strcpy(out, "");

    Object * obj = argv[0];

    if (objectHasAttr(obj, "__disp__"))
    {
        Object ** arglist = makeArglist(argv[0]);
        Object * strtext = ((standard_func_def)objectGetAttr(obj, "__disp__"))(1, arglist);

        if (strcmp(strtext->name, "string"))
        {
            char * err = malloc(25 + strlen(strtext->name) + 27 + 1);
            strcpy(err, "__disp__ method of type '");
            strcat(err, strtext->name);
            strcat(err, "' must return type 'string'");
            error(err, line_num);
        }

        char * text = objectGetAttr(strtext, "value");

        free(arglist);

        mstrcat(&out, text);

        freeObject(strtext);

        // pushTrash(text);
    }
    else
    {
        char * obj_addr = strObjAddress(obj);

        char * temp = malloc(2 + strlen(obj->name) + 12 + strlen(obj_addr) + 1 + 1);
        strcpy(temp, "<'");
        strcat(temp, obj->name);
        strcat(temp, "' Object at ");
        strcat(temp, obj_addr);
        strcat(temp, ">");

        mstrcat(&out, temp);

        free(obj_addr);
        free(temp);
    }

    return makeString(out, 1);
}

Object * q_function_print(int argc, Object ** argv)
{
    int ret = 0;

    for (int i = 0; i < argc; i++)
    {
        Object * obj = argv[i];

        if (objectHasAttr(obj, "__string__"))
        {
            Object ** arglist = makeArglist(argv[i]);
            Object * strtext = q_function_string(1, arglist);

            char * text = objectGetAttr(strtext, "value");

            free(arglist);

            ret += strlen(text);
            printf("%s", text);

            freeObject(strtext);
        }
        else
        {
            Object ** arglist = makeArglist(argv[i]);
            Object * strtext = q_function_display(1, arglist);

            free(arglist);

            char * text = (char *)objectGetAttr(strtext, "value");

            ret += strlen(text);
            printf("%s", text);

            freeObject(strtext);
        }

        if (i + 1 < argc)
        {
            printf(" ");
            ret++;
        }
    }

    long long * llptr = makeLLPtr(ret);

    return makeLong(llptr, 1);
}

Object * q_function_println(int argc, Object ** argv)
{
    int ret = 0;

    for (int i = 0; i < argc; i++)
    {
        Object * obj = argv[i];

        if (objectHasAttr(obj, "__string__"))
        {
            Object ** arglist = makeArglist(argv[i]);
            Object * strtext = q_function_string(1, arglist);

            free(arglist);

            char * text = objectGetAttr(strtext, "value");

            ret += strlen(text);
            printf("%s", text);

            freeObject(strtext);
        }
        else
        {
            Object ** arglist = makeArglist(argv[i]);
            Object * strtext = q_function_display(1, arglist);

            free(arglist);

            char * text = objectGetAttr(strtext, "value");

            ret += strlen(text);
            printf("%s", text);

            freeObject(strtext);
        }

        if (i + 1 < argc)
        {
            printf(" ");
            ret++;
        }
        else
        {
            printf("\n");
            ret++;
        }
    }

    long long * llptr = makeLLPtr(ret);

    return makeLong(llptr, 1);
}

Object * q_function_input(int argc, Object ** argv)
{
    if (argc)
    {
        Object ** arglist = makeArglist(argv[0]);
        q_function_print(1, arglist);
        free(arglist);
    }

    char * buffer = malloc(1);
    strcpy(buffer, "");
    int buflen = 0;

    char last = 0;

    while (last != '\n' && last != '\r')
    {
        last = getchar();

        buffer = realloc(buffer, buflen + 1);
        buffer[buflen] = last;

        buflen++;
    }

    buffer[buflen - 1] = '\0';

    return makeString(buffer, 1);
}


Object * q_function_bool(int argc, Object ** argv)
{
    if (!objectHasAttr(argv[0], "__bool__"))
    {
        char * err = malloc(6 + strlen(argv[0]->name) + 34 + 1);
        strcpy(err, "type '");
        strcat(err, argv[0]->name);
        strcat(err, "' can not be converted into a bool");
        error(err, line_num);
    }

    Object * ret = ((standard_func_def)objectGetAttr(argv[0], "__bool__"))(1, argv);

    if (strcmp(ret->name, "int"))
    {
        char * err = malloc(25 + strlen(argv[0]->name) + 24 + 1);
        strcpy(err, "__bool__ method of type '");
        strcat(err, argv[0]->name);
        strcat(err, "' must return type 'int'");
        error(err, line_num);
    }

    return ret;
}


Object * q_function_string(int argc, Object ** argv)
{
    if (objectHasAttr(argv[0], "__string__"))
    {
        Object * ret = ((standard_func_def)objectGetAttr(argv[0], "__string__"))(1, argv);

        if (strcmp(ret->name, "string"))
        {
            char * err = malloc(27 + strlen(argv[0]->name) + 27 + 1);
            strcpy(err, "__string__ method of type '");
            strcat(err, argv[0]->name);
            strcat(err, "' must return type 'string'");
            error(err, line_num);
        }

        return ret;
    }
    else if (objectHasAttr(argv[0], "__disp__"))
    {
        Object * ret = ((standard_func_def)objectGetAttr(argv[0], "__disp__"))(1, argv);

        if (strcmp(ret->name, "string"))
        {
            char * err = malloc(25 + strlen(argv[0]->name) + 27 + 1);
            strcpy(err, "__disp__ method of type '");
            strcat(err, argv[0]->name);
            strcat(err, "' must return type 'string'");
            error(err, line_num);
        }

        return ret;
    }

    char * err = malloc(6 + strlen(argv[0]->name) + 36 + 1);
    strcpy(err, "type '");
    strcat(err, argv[0]->name);
    strcat(err, "' can not be converted into a string");
    error(err, line_num);

    return makeNull();
}

Object * q_function_int(int argc, Object ** argv)
{
    if (!objectHasAttr(argv[0], "__int__"))
    {
        char * err = malloc(6 + strlen(argv[0]->name) + 36 + 1);
        strcpy(err, "type '");
        strcat(err, argv[0]->name);
        strcat(err, "' can not be converted into an integer");
        error(err, line_num);
    }

    Object * ret = ((standard_func_def)objectGetAttr(argv[0], "__int__"))(1, argv);

    if (strcmp(ret->name, "int"))
    {
        char * err = malloc(24 + strlen(argv[0]->name) + 24 + 1);
        strcpy(err, "__int__ method of type '");
        strcat(err, argv[0]->name);
        strcat(err, "' must return type 'int'");
        error(err, line_num);
    }

    return ret;
}

Object * q_function_long(int argc, Object ** argv)
{
    // Try __long__
    if (objectHasAttr(argv[0], "__long__"))
    {
        Object * ret = ((standard_func_def)objectGetAttr(argv[0], "__long__"))(1, argv);

        if (strcmp(ret->name, "long"))
        {
            char * err = malloc(25 + strlen(argv[0]->name) + 25 + 1);
            strcpy(err, "__long__ method of type '");
            strcat(err, argv[0]->name);
            strcat(err, "' must return type 'long'");
            error(err, line_num);
        }

        return ret;
    }
    // Try __int__ if __long__ can't be found
    else if (objectHasAttr(argv[0], "__int__"))
    {
        Object * ret = ((standard_func_def)objectGetAttr(argv[0], "__int__"))(1, argv);

        if (strcmp(ret->name, "int"))
        {
            char * err = malloc(24 + strlen(argv[0]->name) + 24 + 1);
            strcpy(err, "__int__ method of type '");
            strcat(err, argv[0]->name);
            strcat(err, "' must return type 'int'");
            error(err, line_num);
        }

        return ret;
    }

    char * err = malloc(6 + strlen(argv[0]->name) + 34 + 1);
    strcpy(err, "type '");
    strcat(err, argv[0]->name);
    strcat(err, "' can not be converted into a long");
    error(err, line_num);

    return makeNull();
}

Object * q_function_type(int argc, Object ** argv)
{
    return makeString(argv[0]->name, 0);
}

Object * q_function_len(int argc, Object ** argv)
{
    if (!objectHasAttr(argv[0], "__len__"))
    {
        char * err = malloc(6 + strlen(argv[0]->name) + 45 + 1);
        strcpy(err, "type '");
        strcat(err, argv[0]->name);
        strcat(err, "' doesn't have a method for length retrieving");
        error(err, line_num);
    }

    return ((standard_func_def)objectGetAttr(argv[0], "__len__"))(1, argv);
}

Object * q_function_exec(int argc, Object ** argv)
{
    if (strcmp(argv[0]->name, "string"))
    {
        char * err = malloc(45 + strlen(argv[0]->name) + 1 + 1);
        strcpy(err, "exec argument must be of type 'string', not '");
        strcat(err, argv[0]->name);
        strcat(err, "'");
        error(err, line_num);
    }

    char * rawcode = (char *)objectGetAttr(argv[0], "value");
    char * code = malloc(strlen(rawcode) + 1 + 1);
    strcpy(code, rawcode);
    strcat(code, "\n");

    char * bytecode = quokka_compile_raw(code, 0);

    int old_bc_line = bc_line;
    int old_bc_line_count = bc_line_count;

    quokka_interpret(bytecode);

    bc_line = old_bc_line;
    bc_line_count = old_bc_line_count;

    return makeNull();
}

Object * q_function_exit(int argc, Object ** argv)
{
    // freeVars();
    // freeStack();
    // freeRetStack();
    // emptyTrash();

    // // Free scope stack (from compilation)
    // free(scpstk);
    // free(scps);
    // free(scplines);

    // free(full_file_name);
    // free(full_dir_name);
    // free(main_bytecode);

    cleanupAll();

    exit(1);

    // The following line is simply to prevent a warning that may
    // appear when compiling Quokka with some GCC or Clang versions
    return makeNull();
}

Object * q_function_sizeof(int argc, Object ** argv)
{
    if (objectHasAttr(argv[0], "__sizeof__"))
        return ((standard_func_def)objectGetAttr(argv[0], "__sizeof__"))(1, argv);

    int * size = makeIntPtr(sizeof(argv[0]));
    // pushTrash(size);

    return makeInt(size, 1);
}

Object * q_function_min(int argc, Object ** argv)
{
    Object ** lst = objectGetAttr(argv[0], "value");
    int lstlen = ((int *)objectGetAttr(argv[0], "length"))[0];

    int smallest;

    for (int i = 0; i < lstlen; i++)
    {
        Object * item = lst[i];

        if (strcmp(item->name, "int"))
        {
            Object ** arglist = makeArglist(item);
            item = q_function_int(1, arglist);
            free(arglist);
        }

        int itemint = ((int *)objectGetAttr(item, "value"))[0];

        if (itemint < smallest || !i)
            smallest = itemint;
    }

    int * intptr = makeIntPtr(smallest);
    // pushTrash(intptr);

    return makeInt(intptr, 1);
}

Object * q_function_max(int argc, Object ** argv)
{
    Object ** lst = objectGetAttr(argv[0], "value");
    int lstlen = ((int *)objectGetAttr(argv[0], "length"))[0];

    int largest;

    for (int i = 0; i < lstlen; i++)
    {
        Object * item = lst[i];

        if (strcmp(item->name, "int"))
        {
            Object ** arglist = makeArglist(item);
            item = q_function_int(1, arglist);
            free(arglist);
        }

        int itemint = ((int *)objectGetAttr(item, "value"))[0];

        if (itemint > largest || !i)
            largest = itemint;
    }

    int * intptr = makeIntPtr(largest);
    // pushTrash(intptr);

    return makeInt(intptr, 1);
}

Object * q_function_tochar(int argc, Object ** argv)
{
    if (strcmp(argv[0]->name, "int"))
        error("tochar first argument must be of type 'int'", line_num);

    int val = ((int *)objectGetAttr(argv[0], "value"))[0];

    char * charptr = malloc(2);
    charptr[0] = val;
    charptr[1] = 0;

    return makeString(charptr, 1);
}

Object * q_function_charcode(int argc, Object ** argv)
{
    if (strcmp(argv[0]->name, "string"))
        error("charcode first argument must be of type 'string'", line_num);

    char * val = objectGetAttr(argv[0], "value");

    int * charptr = makeIntPtr(val[0]);

    return makeInt(charptr, 1);
}
