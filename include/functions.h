Object q_function_println(int argc, Object * argv);

Object q_function_display(int argc, Object * argv)
{
    char * out = malloc(1);
    strcpy(out, "");

    if (objectHasAttr(argv[0], "__disp__"))
    {
        Object * arglist = makeArglist(argv[0]);
        Object strtext = ((standard_func_def)objectGetAttr(argv[0], "__disp__"))(1, arglist);

        if (strcmp(strtext.name, "string"))
        {
            char * err = malloc(25 + strlen(strtext.name) + 27 + 1);
            strcpy(err, "__disp__ method of type '");
            strcat(err, strtext.name);
            strcat(err, "' must return type 'string'");
            error(err, line_num);
        }

        char * text = (char *)objectGetAttr(strtext, "value");

        free(arglist);
        freeObject(strtext);

        mstrcat(&out, text);
    }
    else
    {
        mstrcattrip(&out, "<", argv[0].name);
        mstrcat(&out, ">");
    }

    return makeString(out);
}

Object q_function_print(int argc, Object * argv)
{
    int ret = 0;

    for (int i = 0; i < argc; i++)
    {
        if (objectHasAttr(argv[i], "__string__"))
        {
            Object * arglist = makeArglist(argv[i]);
            Object strtext = ((standard_func_def)objectGetAttr(argv[i], "__string__"))(1, arglist);

            if (!strcmp(objectGetAttr(strtext, "value"), "string"))
            {
                char * err = malloc(27 + strlen(strtext.name) + 27 + 1);
                strcpy(err, "__string__ method of type '");
                strcat(err, strtext.name);
                strcat(err, "' must return type 'string'");
                error(err, line_num);
            }

            char * text = (char *)objectGetAttr(strtext, "value");

            free(arglist);

            ret += strlen(text);
            printf("%s", text);
        }
        else
        {
            Object * arglist = makeArglist(argv[i]);
            Object strtext = q_function_display(1, arglist);

            free(arglist);

            char * text = (char *)objectGetAttr(strtext, "value");
            
            ret += strlen(text);
            printf("%s", text);
        }

        if (i + 1 < argc)
        {
            printf(" ");
            ret++;
        }
    }

    return makeInteger(makeIntPtr(ret));
}

Object q_function_println(int argc, Object * argv)
{
    int ret = 0;

    for (int i = 0; i < argc; i++)
    {
        if (objectHasAttr(argv[i], "__string__"))
        {
            Object * arglist = makeArglist(argv[i]);
            Object strtext = ((standard_func_def)objectGetAttr(argv[i], "__string__"))(1, arglist);

            if (!strcmp(objectGetAttr(strtext, "value"), "string"))
            {
                char * err = malloc(24 + strlen(strtext.name) + 27 + 1);
                strcpy(err, "__string__ method of type '");
                strcat(err, strtext.name);
                strcat(err, "' must return type 'string'");
                error(err, line_num);
            }

            char * text = (char *)objectGetAttr(strtext, "value");

            free(arglist);

            ret += strlen(text);
            printf("%s", text);
        }
        else
        {
            Object * arglist = makeArglist(argv[i]);
            Object strtext = q_function_display(1, arglist);

            free(arglist);

            char * text = (char *)objectGetAttr(strtext, "value");
            
            ret += strlen(text);
            printf("%s", text);
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

    return makeInteger(makeIntPtr(ret));
}

Object q_function_input(int argc, Object * argv)
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

    return makeString(buffer);
}


Object q_function_bool(int argc, Object * argv)
{
    if (!objectHasAttr(argv[0], "__bool__"))
    {
        char * err = malloc(6 + strlen(argv[0].name) + 34 + 1);
        strcpy(err, "type '");
        strcat(err, argv[0].name);
        strcat(err, "' can not be converted into a bool");
        error(err, line_num);
    }

    Object ret = ((standard_func_def)objectGetAttr(argv[0], "__bool__"))(1, argv);

    if (strcmp(ret.name, "bool"))
    {
        char * err = malloc(25 + strlen(argv[0].name) + 25 + 1);
        strcpy(err, "__bool__ method of type '");
        strcat(err, argv[0].name);
        strcat(err, "' must return type 'bool'");
        error(err, line_num);
    }

    return ret;
}


Object q_function_string(int argc, Object * argv)
{
    if (objectHasAttr(argv[0], "__string__"))
    {
        Object ret = ((standard_func_def)objectGetAttr(argv[0], "__string__"))(1, argv);

        if (strcmp(ret.name, "string"))
        {
            char * err = malloc(27 + strlen(argv[0].name) + 27 + 1);
            strcpy(err, "__string__ method of type '");
            strcat(err, argv[0].name);
            strcat(err, "' must return type 'string'");
            error(err, line_num);
        }

        return ret;
    }
    else if (objectHasAttr(argv[0], "__disp__"))
    {
        Object ret = ((standard_func_def)objectGetAttr(argv[0], "__disp__"))(1, argv);

        if (strcmp(ret.name, "string"))
        {
            char * err = malloc(25 + strlen(argv[0].name) + 27 + 1);
            strcpy(err, "__disp__ method of type '");
            strcat(err, argv[0].name);
            strcat(err, "' must return type 'string'");
            error(err, line_num);
        }

        return ret;
    }

    char * err = malloc(6 + strlen(argv[0].name) + 36 + 1);
    strcpy(err, "type '");
    strcat(err, argv[0].name);
    strcat(err, "' can not be converted into a string");
    error(err, line_num);
}

Object q_function_int(int argc, Object * argv)
{
    if (!objectHasAttr(argv[0], "__int__"))
    {
        char * err = malloc(6 + strlen(argv[0].name) + 36 + 1);
        strcpy(err, "type '");
        strcat(err, argv[0].name);
        strcat(err, "' can not be converted into an integer");
        error(err, line_num);
    }

    Object ret = ((standard_func_def)objectGetAttr(argv[0], "__int__"))(1, argv);

    if (strcmp(ret.name, "int"))
    {
        char * err = malloc(24 + strlen(argv[0].name) + 24 + 1);
        strcpy(err, "__int__ method of type '");
        strcat(err, argv[0].name);
        strcat(err, "' must return type 'int'");
        error(err, line_num);
    }

    return ret;
}

Object q_function_type(int argc, Object * argv)
{
    return makeString(argv[0].name);
}

Object q_function_len(int argc, Object * argv)
{
    if (!objectHasAttr(argv[0], "__len__"))
    {
        char * err = malloc(6 + strlen(argv[0].name) + 45 + 1);
        strcpy(err, "type '");
        strcat(err, argv[0].name);
        strcat(err, "' doesn't have a method for length retrieving");
        error(err, line_num);
    }

    return ((standard_func_def)objectGetAttr(argv[0], "__len__"))(1, argv);
}

Object q_function_exec(int argc, Object * argv)
{
    if (strcmp(argv[0].name, "string"))
    {
        char * err = malloc(45 + strlen(argv[0].name) + 1 + 1);
        strcpy(err, "exec argument must be of type 'string', not '");
        strcat(err, argv[0].name);
        strcat(err, "'");
        error(err, line_num);
    }

    char * rawcode = (char *)objectGetAttr(argv[0], "value");
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
