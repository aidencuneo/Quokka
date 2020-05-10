Object q_function_println(int argc, Object * argv);

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
            println("STARTED");

            Object lst = ((Object *)objectGetAttr(argv[i], "value"))[0];
            println("got list");
            int lstlen = ((int *)objectGetAttr(argv[i], "length"))[0];
            println("got len");
            println(lstlen);

            ret += 2;
            print("[");
            println("p");

            for (int p = 0; p < lstlen; p++)
            {
                Object * arglist = malloc(sizeof(Object));
                Object * lstobj = ((Object *)objectGetAttr(lst, "value"));
                println("YO");

                q_function_println(1, lstobj);
                println("YO");
            }

            Object printlen = q_function_print(lstlen, objectGetAttr(lst, "value"));
            println("printed");

            ret += ((int *)objectGetAttr(printlen, "value"))[0] + 2;
            ret += 2;

            print("]");
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
        if (!strcmp(argv[i].name, "string"))
        {
            char * text = (char *)objectGetAttr(argv[i], "value");

            ret += strlen(text);
            printf("%s\n", text);
        }
        else if (!strcmp(argv[i].name, "int"))
        {
            int num = ((int *)objectGetAttr(argv[i], "value"))[0];
            char * text = intToStr(num);

            ret += strlen(text);
            printf("%s\n", text);
        }
        else if (!strcmp(argv[i].name, "list"))
        {
            // Print a list, using the print function on each item in it.
        }
        else
            error("'println' can only print standard Quokka types (for now)", line_num);
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
    if (!objectHasAttr(argv[0], "__str__"))
    {
        char * err = malloc(6 + strlen(argv[0].name) + 36 + 1);
        strcpy(err, "type '");
        strcat(err, argv[0].name);
        strcat(err, "' can not be converted into a string");
        error(err, line_num);
    }
    return ((standard_func_def)objectGetAttr(argv[0], "__str__"))(1, argv);
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
