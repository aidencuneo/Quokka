Object q_function_println(int argc, int * argv);

Object q_function_display(int argc, int * argv)
{
    char * out = malloc(1);
    strcpy(out, "");

    Object obj = mem[argv[0]];

    if (objectHasAttr(obj, "__disp__"))
    {
        int * arglist = makeIntPtr(argv[0]);
        Object strtext = ((standard_func_def)objectGetAttr(obj, "__disp__"))(1, arglist);

        if (strcmp(strtext.name, "string"))
        {
            char * err = malloc(25 + strlen(strtext.name) + 27 + 1);
            strcpy(err, "__disp__ method of type '");
            strcat(err, strtext.name);
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
        mstrcattrip(&out, "<", obj.name);
        mstrcat(&out, ">");
    }

    return makeString(out);
}

Object q_function_print(int argc, int * argv)
{
    int ret = 0;

    for (int i = 0; i < argc; i++)
    {
        Object obj = mem[argv[i]];

        if (objectHasAttr(obj, "__string__"))
        {
            int * arglist = makeIntPtr(argv[i]);
            Object strtext = ((standard_func_def)objectGetAttr(obj, "__string__"))(1, arglist);

            if (strcmp(strtext.name, "string"))
            {
                char * err = malloc(27 + strlen(strtext.name) + 27 + 1);
                strcpy(err, "__string__ method of type '");
                strcat(err, strtext.name);
                strcat(err, "' must return type 'string'");
                error(err, line_num);
            }

            char * text = objectGetAttr(strtext, "value");

            free(arglist);

            ret += strlen(text);
            printf("%s", text);
        }
        else
        {
            int * arglist = makeIntPtr(argv[i]);
            Object strtext = q_function_display(1, arglist);

            free(arglist);

            char * text = (char *)objectGetAttr(strtext, "value");
            
            ret += strlen(text);
            printf("%s", text);

            free(text);
            freeObject(strtext);
        }

        if (i + 1 < argc)
        {
            printf(" ");
            ret++;
        }
    }

    // long long * llptr = makeLLPtr(ret);
    // pushTrash(llptr);

    // return makeLong(llptr);

    return makeNull();
}

Object q_function_println(int argc, int * argv)
{
    int ret = 0;

    for (int i = 0; i < argc; i++)
    {
        Object obj = mem[argv[i]];

        if (objectHasAttr(obj, "__string__"))
        {
            int * arglist = makeIntPtr(argv[i]);
            Object strtext = ((standard_func_def)objectGetAttr(obj, "__string__"))(1, arglist);

            if (strcmp(strtext.name, "string"))
            {
                char * err = malloc(24 + strlen(strtext.name) + 27 + 1);
                strcpy(err, "__string__ method of type '");
                strcat(err, strtext.name);
                strcat(err, "' must return type 'string'");
                error(err, line_num);
            }

            char * text = objectGetAttr(strtext, "value");

            free(arglist);

            ret += strlen(text);
            printf("%s", text);
        }
        else
        {
            int * arglist = makeIntPtr(argv[i]);
            Object strtext = q_function_display(1, arglist);

            free(arglist);

            char * text = objectGetAttr(strtext, "value");
            
            ret += strlen(text);
            printf("%s", text);

            free(text);
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

    // long long * llptr = makeLLPtr(ret);
    // pushTrash(llptr);

    // return makeLong(llptr);

    return makeNull();
}

Object q_function_input(int argc, int * argv)
{
    char * buffer = malloc(1);
    strcpy(buffer, "");
    int buflen = 0;

    char last = 0;

    while (last != '\n' && last != '\r')
    {
        last = getchar();

        buffer = realloc(buffer, strlen(buffer) + 1);
        buffer[buflen] = last;

        buflen++;
    }

    buffer[buflen - 1] = '\0';

    pushTrash(buffer);

    return makeString(buffer);
}


Object q_function_bool(int argc, int * argv)
{
    if (!objectHasAttr(mem[argv[0]], "__bool__"))
    {
        char * err = malloc(6 + strlen(mem[argv[0]].name) + 34 + 1);
        strcpy(err, "type '");
        strcat(err, mem[argv[0]].name);
        strcat(err, "' can not be converted into a bool");
        error(err, line_num);
    }

    Object ret = ((standard_func_def)objectGetAttr(mem[argv[0]], "__bool__"))(1, argv);

    if (strcmp(ret.name, "int"))
    {
        char * err = malloc(25 + strlen(mem[argv[0]].name) + 24 + 1);
        strcpy(err, "__bool__ method of type '");
        strcat(err, mem[argv[0]].name);
        strcat(err, "' must return type 'int'");
        error(err, line_num);
    }

    return ret;
}


Object q_function_string(int argc, int * argv)
{
    if (objectHasAttr(mem[argv[0]], "__string__"))
    {
        Object ret = ((standard_func_def)objectGetAttr(mem[argv[0]], "__string__"))(1, argv);
        println(ret.name);

        if (strcmp(ret.name, "string"))
        {
            char * err = malloc(27 + strlen(mem[argv[0]].name) + 27 + 1);
            strcpy(err, "__string__ method of type '");
            strcat(err, mem[argv[0]].name);
            strcat(err, "' must return type 'string'");
            error(err, line_num);
        }

        return ret;
    }
    else if (objectHasAttr(mem[argv[0]], "__disp__"))
    {
        Object ret = ((standard_func_def)objectGetAttr(mem[argv[0]], "__disp__"))(1, argv);

        if (strcmp(ret.name, "string"))
        {
            char * err = malloc(25 + strlen(mem[argv[0]].name) + 27 + 1);
            strcpy(err, "__disp__ method of type '");
            strcat(err, mem[argv[0]].name);
            strcat(err, "' must return type 'string'");
            error(err, line_num);
        }

        return ret;
    }

    char * err = malloc(6 + strlen(mem[argv[0]].name) + 36 + 1);
    strcpy(err, "type '");
    strcat(err, mem[argv[0]].name);
    strcat(err, "' can not be converted into a string");
    error(err, line_num);

    return makeNull();
}

Object q_function_int(int argc, int * argv)
{
    if (!objectHasAttr(mem[argv[0]], "__int__"))
    {
        char * err = malloc(6 + strlen(mem[argv[0]].name) + 36 + 1);
        strcpy(err, "type '");
        strcat(err, mem[argv[0]].name);
        strcat(err, "' can not be converted into an integer");
        error(err, line_num);
    }

    Object ret = ((standard_func_def)objectGetAttr(mem[argv[0]], "__int__"))(1, argv);

    if (strcmp(ret.name, "int"))
    {
        char * err = malloc(24 + strlen(mem[argv[0]].name) + 24 + 1);
        strcpy(err, "__int__ method of type '");
        strcat(err, mem[argv[0]].name);
        strcat(err, "' must return type 'int'");
        error(err, line_num);
    }

    return ret;
}

Object q_function_long(int argc, int * argv)
{
    // Try __long__
    if (objectHasAttr(mem[argv[0]], "__long__"))
    {
        Object ret = ((standard_func_def)objectGetAttr(mem[argv[0]], "__long__"))(1, argv);

        if (strcmp(ret.name, "long"))
        {
            char * err = malloc(25 + strlen(mem[argv[0]].name) + 25 + 1);
            strcpy(err, "__long__ method of type '");
            strcat(err, mem[argv[0]].name);
            strcat(err, "' must return type 'long'");
            error(err, line_num);
        }

        return ret;
    }
    // Try __int__ if __long__ can't be found
    else if (objectHasAttr(mem[argv[0]], "__int__"))
    {
        Object ret = ((standard_func_def)objectGetAttr(mem[argv[0]], "__int__"))(1, argv);

        if (strcmp(ret.name, "int"))
        {
            char * err = malloc(24 + strlen(mem[argv[0]].name) + 24 + 1);
            strcpy(err, "__int__ method of type '");
            strcat(err, mem[argv[0]].name);
            strcat(err, "' must return type 'int'");
            error(err, line_num);
        }

        return ret;
    }

    char * err = malloc(6 + strlen(mem[argv[0]].name) + 34 + 1);
    strcpy(err, "type '");
    strcat(err, mem[argv[0]].name);
    strcat(err, "' can not be converted into a long");
    error(err, line_num);

    return makeNull();
}

Object q_function_type(int argc, int * argv)
{
    return makeString(mem[argv[0]].name);
}

Object q_function_len(int argc, int * argv)
{
    if (!objectHasAttr(mem[argv[0]], "__len__"))
    {
        char * err = malloc(6 + strlen(mem[argv[0]].name) + 45 + 1);
        strcpy(err, "type '");
        strcat(err, mem[argv[0]].name);
        strcat(err, "' doesn't have a method for length retrieving");
        error(err, line_num);
    }

    return ((standard_func_def)objectGetAttr(mem[argv[0]], "__len__"))(1, argv);
}

Object q_function_exec(int argc, int * argv)
{
    if (strcmp(mem[argv[0]].name, "string"))
    {
        char * err = malloc(45 + strlen(mem[argv[0]].name) + 1 + 1);
        strcpy(err, "exec argument must be of type 'string', not '");
        strcat(err, mem[argv[0]].name);
        strcat(err, "'");
        error(err, line_num);
    }

    char * rawcode = (char *)objectGetAttr(mem[argv[0]], "value");
    char * code = malloc(strlen(rawcode) + 1 + 1);
    strcpy(code, rawcode);
    strcat(code, "\n");

    char * bytecode = quokka_compile_raw(code, -1, 0);

    int old_bc_line = bc_line;
    int old_bc_line_count = bc_line_count;

    quokka_interpret(bytecode);

    bc_line = old_bc_line;
    bc_line_count = old_bc_line_count;

    return makeNull();
}

Object q_function_sizeof(int argc, int * argv)
{
    if (objectHasAttr(mem[argv[0]], "__sizeof__"))
    {
        return ((standard_func_def)objectGetAttr(mem[argv[0]], "__sizeof__"))(1, argv);
    }

    int * size = makeIntPtr(sizeof(mem[argv[0]]));
    pushTrash(size);

    return makeInt(size);
}
