Object * q_function_println(int argc, Object ** argv);
Object * q_function_string(int argc, Object ** argv);

Object * q_function_display(int argc, Object ** argv)
{
    char * out;

    void * func = objOperDisp(argv[0]);
    if (func != NULL)
    {
        Object * strtext = ((standard_func_def)func)(1, &argv[0]);

        if (strcmp(strtext->name, "string"))
        {
            char * err = malloc(25 + strlen(strtext->name) + 27 + 1);
            strcpy(err, "__disp__ method of type '");
            strcat(err, strtext->name);
            strcat(err, "' must return type 'string'");
            error(err, line_num);
        }

        out = strdup(strtext->values[0]);

        freeObject(strtext);
    }
    else
        out = neatObjAddress(argv[0]);

    return makeString(out, 1);
}

Object * q_function_print(int argc, Object ** argv)
{
    char * printsep = " ";

    for (int i = 0; i < argc; i++)
    {
        Object * obj = argv[i];
        void * func;

        if ((func = objOperString(obj)) != NULL)
        {
            Object * strtext = ((standard_func_def)func)(1, argv);

            if (strcmp(strtext->name, "string"))
            {
                char * err = malloc(27 + strlen(obj->name) + 27 + 1);
                strcpy(err, "__string__ method of type '");
                strcat(err, argv[0]->name);
                strcat(err, "' must return type 'string'");
                error(err, line_num);
            }

            char * text = strtext->values[0];

            printf("%s", text);

            freeObject(strtext);
        }
        else if ((func = objOperDisp(obj)) != NULL)
        {
            Object * strtext = ((standard_func_def)func)(1, argv);

            if (strcmp(strtext->name, "string"))
            {
                char * err = malloc(25 + strlen(obj->name) + 27 + 1);
                strcpy(err, "__disp__ method of type '");
                strcat(err, argv[0]->name);
                strcat(err, "' must return type 'string'");
                error(err, line_num);
            }

            char * text = strtext->values[0];

            printf("%s", text);

            freeObject(strtext);
        }
        else
        {
            char * text = neatObjAddress(obj);
            printf("%s", text);
            free(text);
        }

        if (i + 1 < argc)
            printf("%s", printsep);
    }

    return makeNull();
}

Object * q_function_println(int argc, Object ** argv)
{
    char * printsep = " ";

    for (int i = 0; i < argc; i++)
    {
        void * func;

        if ((func = objOperString(argv[i])) != NULL)
        {
            Object * temp = argv[0];
            argv[0] = argv[i];

            Object * strtext = ((standard_func_def)func)(1, argv);

            argv[i] = argv[0];
            argv[0] = temp;

            if (strcmp(strtext->name, "string"))
            {
                char * err = malloc(27 + strlen(argv[i]->name) + 27 + 1);
                strcpy(err, "__string__ method of type '");
                strcat(err, argv[i]->name);
                strcat(err, "' must return type 'string'");
                error(err, line_num);
            }

            char * text = strtext->values[0];

            printf("%s", text);

            freeObject(strtext);
        }
        else if ((func = objOperDisp(argv[i])) != NULL)
        {
            Object * temp = argv[0];
            argv[0] = argv[i];

            Object * strtext = ((standard_func_def)func)(1, argv);

            argv[i] = argv[0];
            argv[0] = temp;

            if (strcmp(strtext->name, "string"))
            {
                char * err = malloc(25 + strlen(argv[i]->name) + 27 + 1);
                strcpy(err, "__disp__ method of type '");
                strcat(err, argv[i]->name);
                strcat(err, "' must return type 'string'");
                error(err, line_num);
            }

            char * text = strtext->values[0];

            printf("%s", text);

            freeObject(strtext);
        }
        else
        {
            char * text = neatObjAddress(argv[i]);
            printf("%s", text);
            free(text);
        }

        if (i + 1 < argc)
            printf("%s", printsep);
        else
            printf("\n");
    }

    return makeNull();
}

Object * q_function_bool(int argc, Object ** argv)
{
    void * func = objOperBool(argv[0]);
    if (func == NULL)
    {
        char * err = malloc(6 + strlen(argv[0]->name) + 34 + 1);
        strcpy(err, "type '");
        strcat(err, argv[0]->name);
        strcat(err, "' can not be converted into a bool");
        error(err, line_num);
    }

    Object * ret = ((standard_func_def)func)(1, argv);

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
    void * func;

    // Try __string__
    func = objOperString(argv[0]);
    if (func != NULL)
    {
        Object * ret = ((standard_func_def)func)(1, argv);

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
    // Try __disp__ if __string__ can't be found
    func = objOperDisp(argv[0]);
    if (func != NULL)
    {
        Object * ret = ((standard_func_def)func)(1, argv);

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
    if (!strcmp(argv[0]->name, "int"))
        return argv[0];

    void * func;

    func = objOperInt(argv[0]);
    if (func == NULL)
    {
        char * err = malloc(6 + strlen(argv[0]->name) + 38 + 1);
        strcpy(err, "type '");
        strcat(err, argv[0]->name);
        strcat(err, "' can not be converted into an integer");
        error(err, line_num);
    }

    Object * ret = ((standard_func_def)func)(1, argv);

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

// Object * q_function_long(int argc, Object ** argv)
// {
//     if (!strcmp(argv[0]->name, "long"))
//         return argv[0];

//     void * func;

//     // Try __long__
//     func = objOperLong(argv[0]);
//     if (func != NULL)
//     {
//         Object * ret = ((standard_func_def)func)(1, argv);

//         if (strcmp(ret->name, "long"))
//         {
//             char * err = malloc(25 + strlen(argv[0]->name) + 25 + 1);
//             strcpy(err, "__long__ method of type '");
//             strcat(err, argv[0]->name);
//             strcat(err, "' must return type 'long'");
//             error(err, line_num);
//         }

//         return ret;
//     }
//     // Try __int__ if __long__ can't be found
//     func = objOperInt(argv[0]);
//     if (func != NULL)
//     {
//         Object * ret = ((standard_func_def)func)(1, argv);

//         if (strcmp(ret->name, "int"))
//         {
//             char * err = malloc(24 + strlen(argv[0]->name) + 24 + 1);
//             strcpy(err, "__int__ method of type '");
//             strcat(err, argv[0]->name);
//             strcat(err, "' must return type 'int'");
//             error(err, line_num);
//         }

//         return ret;
//     }

//     char * err = malloc(6 + strlen(argv[0]->name) + 34 + 1);
//     strcpy(err, "type '");
//     strcat(err, argv[0]->name);
//     strcat(err, "' can not be converted into a long");
//     error(err, line_num);

//     return makeNull();
// }

Object * q_function_type(int argc, Object ** argv)
{
    return makeString(argv[0]->name, 0);
}

Object * q_function_len(int argc, Object ** argv)
{
    void * func = objOperLen(argv[0]);
    if (func == NULL)
    {
        char * err = malloc(6 + strlen(argv[0]->name) + 45 + 1);
        strcpy(err, "type '");
        strcat(err, argv[0]->name);
        strcat(err, "' doesn't have a method for length retrieving");
        error(err, line_num);
    }

    return ((standard_func_def)func)(1, argv);
}

Object * q_function_sizeof(int argc, Object ** argv)
{
    void * func = objOperSizeof(argv[0]);
    if (func != NULL)
        return ((standard_func_def)func)(1, argv);

    int * size = makeIntPtr(sizeof(argv[0]));
    return makeInt(size, 1, 10);
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

    char * rawcode = argv[0]->values[0];
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
    cleanupAll();
    exit(1);

    // The following line is just to prevent a warning that may
    // appear when compiling Quokka with some GCC or Clang versions
    return makeNull();
}

Object * q_function_min(int argc, Object ** argv)
{
    if (strcmp(argv[0]->name, "list"))
    {
        char * err = malloc(42 + strlen(argv[0]->name) + 1 + 1);
        strcpy(err, "min argument must be of type 'list', not '");
        strcat(err, argv[0]->name);
        strcat(err, "'");
        error(err, line_num);
    }

    Object ** lst = argv[0]->values[0];
    int lstlen = *(int *)objectGetAttr(argv[0], "length");

    int smallest = 0;

    for (int i = 0; i < lstlen; i++)
    {
        Object * item = lst[i];

        if (strcmp(item->name, "int"))
            item = q_function_int(1, &item);

        int itemint = *(int *)objectGetAttr(item, "value");

        if (itemint < smallest || !i)
            smallest = itemint;
    }

    int * intptr = makeIntPtr(smallest);

    return makeInt(intptr, 1, 10);
}

Object * q_function_max(int argc, Object ** argv)
{
    if (strcmp(argv[0]->name, "list"))
    {
        char * err = malloc(42 + strlen(argv[0]->name) + 1 + 1);
        strcpy(err, "max argument must be of type 'list', not '");
        strcat(err, argv[0]->name);
        strcat(err, "'");
        error(err, line_num);
    }

    Object ** lst = argv[0]->values[0];
    int lstlen = ((int *)objectGetAttr(argv[0], "length"))[0];

    int largest = 0;

    for (int i = 0; i < lstlen; i++)
    {
        Object * item = lst[i];

        if (strcmp(item->name, "int"))
            item = q_function_int(1, &item);

        int itemint = ((int *)objectGetAttr(item, "value"))[0];

        if (itemint > largest || !i)
            largest = itemint;
    }

    int * intptr = makeIntPtr(largest);

    return makeInt(intptr, 1, 10);
}

Object * q_function_tochar(int argc, Object ** argv)
{
    if (strcmp(argv[0]->name, "int"))
        error("tochar argument must be of type 'int'", line_num);

    int val = ((int *)argv[0]->values[0])[0];

    char * charptr = malloc(2);
    charptr[0] = val;
    charptr[1] = 0;

    return makeString(charptr, 1);
}

Object * q_function_charcode(int argc, Object ** argv)
{
    if (strcmp(argv[0]->name, "string"))
        error("charcode argument must be of type 'string'", line_num);

    char * val = argv[0]->values[0];

    int * charptr = makeIntPtr(val[0]);

    return makeInt(charptr, 1, 10);
}

Object * q_function_input(int argc, Object ** argv)
{
    if (argc)
        q_function_print(1, &argv[0]);

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

Object * q_function_open(int argc, Object ** argv)
{
    char * path;
    char * mode = "r";

    if (argc == 2)
    {
        if (strcmp(argv[1]->name, "string"))
        {
            char * err = malloc(39 + strlen(argv[0]->name) + 1 + 1);
            strcpy(err, "file mode argument can not be of type '");
            strcat(err, argv[0]->name);
            strcat(err, "'");
            error(err, line_num);
        }

        mode = argv[1]->values[0];

        if (strcmp(mode, "r")   &&
            strcmp(mode, "rb")  &&
            strcmp(mode, "w")   &&
            strcmp(mode, "wb")  &&
            strcmp(mode, "a")   &&
            strcmp(mode, "ab")  &&
            strcmp(mode, "r+")  &&
            strcmp(mode, "rb+") &&
            strcmp(mode, "r+b") &&
            strcmp(mode, "w+")  &&
            strcmp(mode, "wb+") &&
            strcmp(mode, "w+b") &&
            strcmp(mode, "a+")  &&
            strcmp(mode, "ab+") &&
            strcmp(mode, "a+b"))
        {
            char * err = malloc(11 + strlen(mode) + 14 + 1);
            strcpy(err, "file mode '");
            strcat(err, mode);
            strcat(err, "' is not valid");
            error(err, line_num);
        }
    }

    if (strcmp(argv[0]->name, "string"))
    {
        char * err = malloc(44 + strlen(argv[0]->name) + 1 + 1);
        strcpy(err, "path to open must be of type 'string', not '");
        strcat(err, argv[0]->name);
        strcat(err, "'");
        error(err, line_num);
    }

    path = argv[0]->values[0];

    return makeFile(path, mode);
}
