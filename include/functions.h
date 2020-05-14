Object q_function_println(int argc, Object * argv);

Object q_function_display(int argc, Object * argv)
{
    char * out = malloc(1);
    strcpy(out, "");

    if (objectHasAttr(argv[0], "__disp__"))
    {
        Object * arglist = makeArglist(argv[0]);
        Object strtext = ((standard_func_def)objectGetAttr(argv[0], "__disp__"))(1, arglist);

        if (!strcmp(objectGetAttr(strtext, "value"), "string"))
        {
            char * err = malloc(25 + strlen(strtext.name) + 26 + 1);
            strcpy(err, "__disp__ method of type '");
            strcat(err, strtext.name);
            strcat(err, "' does not return a string");
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
        if (!strcmp(argv[i].name, "string"))
        {
            char * text = (char *)objectGetAttr(argv[i], "value");

            ret += strlen(text);
            printf("%s", text);
        }
        else if (!strcmp(argv[i].name, "int"))
        {
            int num = ((int *)objectGetAttr(argv[i], "value"))[0];
            char * text = intToStr(num);

            ret += strlen(text);
            printf("%s", text);
        }
        else if (!strcmp(argv[i].name, "list"))
        {
            Object * lst = ((Object *)objectGetAttr(argv[i], "value"));
            int lstlen = ((int *)objectGetAttr(argv[i], "length"))[0];

            print("[");
            ret++;

            for (int p = 0; p < lstlen; p++)
            {
                Object * arglist = malloc(sizeof(Object));
                arglist[0] = lst[p];

                Object printlen = q_function_print(1, arglist);
                ret += ((int *)objectGetAttr(printlen, "value"))[0];

                if (p + 1 < lstlen)
                {
                    print(", ");
                    ret += 2;
                }

                free(arglist);
            }

            print("]");
            ret += 1;
        }
        else if (!strcmp(argv[i].name, "null"))
        {
            print("null");
            ret += 4;
        }
        else
            error("'print' can only print standard Quokka types (for now)", line_num);
    }

    return makeInteger(makeIntPtr(ret));
}

Object q_function_println(int argc, Object * argv)
{
    int ret = 0;

    for (int i = 0; i < argc; i++)
    {
        if (objectHasAttr(argv[i], "__str__"))
        {
            Object * arglist = malloc(sizeof(Object));
            arglist[0] = argv[i];
            Object strtext = ((standard_func_def)objectGetAttr(argv[i], "__str__"))(1, arglist);

            if (!strcmp(objectGetAttr(strtext, "value"), "string"))
            {
                char * err = malloc(24 + strlen(strtext.name) + 26 + 1);
                strcpy(err, "__str__ method of type '");
                strcat(err, strtext.name);
                strcat(err, "' does not return a string");
                error(err, line_num);
            }

            char * text = (char *)objectGetAttr(strtext, "value");

            free(arglist);

            ret += strlen(text);
            printf("%s", text);

            freeObject(strtext);
        }
        else
        {
            Object * arglist = makeArglist(argv[i]);
            Object strtext = q_function_display(1, arglist);

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
    return ((standard_func_def)objectGetAttr(argv[0], "__bool__"))(1, argv);
}


Object q_function_string(int argc, Object * argv)
{
    if (objectHasAttr(argv[0], "__str__"))
        return ((standard_func_def)objectGetAttr(argv[0], "__str__"))(1, argv);
    else if (objectHasAttr(argv[0], "__disp__"))
        return ((standard_func_def)objectGetAttr(argv[0], "__disp__"))(1, argv);

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
    return ((standard_func_def)objectGetAttr(argv[0], "__int__"))(1, argv);
}

Object q_function_type(int argc, Object * argv)
{
    return makeString(argv[0].name);
}
