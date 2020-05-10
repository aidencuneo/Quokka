Object q_function_print(Object * argv)
{
    // Iterate over args and print all of them with nothing in between

    int ret = 0;

    if (!strcmp(argv[0].name, "string"))
    {
        char * text = (char *)objectGetAttr(argv[0], "value");

        ret += strlen(text);
        printf("%s", text);
    }
    else if (!strcmp(argv[0].name, "int"))
    {
        int num = ((int *)objectGetAttr(argv[0], "value"))[0];
        char * text = intToStr(num);

        ret += strlen(text);
        printf("%s", text);
    }
    else
        error("'print' can only print standard Quokka types (for now)", line_num);

    return makeInteger(makeIntPtr(ret));
}

Object q_function_println(Object * argv)
{
    // Iterate over args and print all of them with a newline in between

    int ret = 0;

    if (!strcmp(argv[0].name, "string"))
    {
        char * text = (char *)objectGetAttr(argv[0], "value");

        ret += strlen(text);
        printf("%s\n", text);
    }
    else if (!strcmp(argv[0].name, "int"))
    {
        int num = ((int *)objectGetAttr(argv[0], "value"))[0];
        char * text = intToStr(num);

        ret += strlen(text);
        printf("%s\n", text);
    }
    else
        error("'println' can only print standard Quokka types (for now)", line_num);

    return makeInteger(makeIntPtr(ret));
}

Object q_function_input(Object * argv)
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


Object q_function_bool(Object * argv)
{
    if (!objectHasAttr(argv[0], "__bool__"))
    {
        char * err = malloc(6 + strlen(argv[0].name) + 34 + 1);
        strcpy(err, "type '");
        strcat(err, argv[0].name);
        strcat(err, "' can not be converted into a bool");
        error(err, line_num);
        exit(1);
    }
    return ((standard_func_def)objectGetAttr(argv[0], "__bool__"))(argv);
}


Object q_function_string(Object * argv)
{
    if (!objectHasAttr(argv[0], "__str__"))
    {
        char * err = malloc(6 + strlen(argv[0].name) + 36 + 1);
        strcpy(err, "type '");
        strcat(err, argv[0].name);
        strcat(err, "' can not be converted into a string");
        error(err, line_num);
        exit(1);
    }
    return ((standard_func_def)objectGetAttr(argv[0], "__str__"))(argv);
}

Object q_function_int(Object * argv)
{
    if (!objectHasAttr(argv[0], "__int__"))
    {
        char * err = malloc(6 + strlen(argv[0].name) + 36 + 1);
        strcpy(err, "type '");
        strcat(err, argv[0].name);
        strcat(err, "' can not be converted into an integer");
        error(err, line_num);
        exit(1);
    }
    return ((standard_func_def)objectGetAttr(argv[0], "__int__"))(argv);
}
