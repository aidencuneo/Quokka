void interp_init();
char * quokka_interpret_line(char * linetext);
char * quokka_interpret_line_tokens(char ** line);
char * quokka_interpret_tokens(char ** tokens);
char * quokka_interpret(char * bytecode);

// Important stuff
Varlist vars;

Object * methods;
int method_count;

Object * stack;
int stack_size;

// Function stuff
int oneArgc;
int twoArgc;

void interp_init()
{
    stack = malloc(sizeof(Object));
    stack_size = 0;

    oneArgc = 1;
    twoArgc = 2;

    Object printFunction = makeObject("function", &oneArgc);
    printFunction = addObjectValue(printFunction, "__call__", &q_function_print);
    printFunction = addObjectValue(printFunction, "__call__argc", &oneArgc);
    addVar("print", printFunction);

    Object printlnFunction = makeObject("function", &oneArgc);
    printlnFunction = addObjectValue(printlnFunction, "__call__", &q_function_println);
    printlnFunction = addObjectValue(printlnFunction, "__call__argc", &oneArgc);
    addVar("println", printlnFunction);

    Object stringFunction = makeObject("function", &oneArgc);
    stringFunction = addObjectValue(stringFunction, "__call__", &q_function_string);
    stringFunction = addObjectValue(stringFunction, "__call__argc", &oneArgc);
    addVar("string", stringFunction);
}

Object emptyObject(char * name)
{
    Object self;

    self.name = name;

    self.names = malloc(sizeof(char *));
    self.values = malloc(sizeof(void *));
    self.value_count = 0;

    return self;
}

Object makeObject(char * name, void * value)
{
    Object self;

    self.name = name;

    self.names = malloc(sizeof(char *));
    self.values = malloc(sizeof(void *));
    self.value_count = 0;

    self = addObjectValue(self, "value", value);

    return self;
}

Object addObjectValue(Object obj, char * name, void * value)
{
    obj.names = realloc(obj.names, (obj.value_count + 1) * sizeof(char *));
    obj.values = realloc(obj.values, (obj.value_count + 1) * sizeof(void *));

    obj.names[obj.value_count] = name;
    obj.values[obj.value_count] = value;

    obj.value_count++;

    return obj;
}

int objectHasAttr(Object obj, char * name)
{
    for (int i = 0; i < obj.value_count; i++)
        if (!strcmp(obj.names[i], name))
            return 1;

    return 0;
}

void * objectGetAttr(Object obj, char * name)
{
    for (int i = 0; i < obj.value_count; i++)
        if (!strcmp(obj.names[i], name))
            return obj.values[i];
}

void pushTop(Object obj)
{
    stack = realloc(stack, (stack_size + 1) * sizeof(Object));
    stack[stack_size] = obj;
    stack_size++;
}

Object popTop()
{
    stack = realloc(stack, stack_size * sizeof(Object));
    stack_size--;

    return stack[stack_size];
}

void addVar(char * name, Object obj)
{
    vars.names = realloc(vars.names, (vars.count + 1) * sizeof(char *));
    vars.values = realloc(vars.values, (vars.count + 1) * sizeof(Object));

    vars.names[vars.count] = name;
    vars.values[vars.count] = obj;

    vars.count++;
}

Object getVar(char * name)
{
    for (int i = 0; i < vars.count; i++)
        if (!strcmp(vars.names[i], name))
            return vars.values[i];

    char * err = malloc(1 + strlen(name) + 16 + 1);
    strcpy(err, "'");
    strcat(err, name);
    strcat(err, "' is not defined");
    error(err, line_num);
}

void addMethod(Object method)
{
    methods = realloc(methods, (method_count + 1) * sizeof(Object));
    methods[method_count] = method;
    method_count++;
}

//
/// datatypes
//

Object __add___integer(Object * argv)
{
    if (strcmp(argv[1].name, "int"))
    {
        char * err = malloc(17 + strlen(argv[1].name) + 27 + 1);
        strcpy(err, "types 'int' and '");
        strcat(err, argv[1].name);
        strcat(err, "' can not be added with '+'");
        error(err, line_num);
        exit(1);
    }

    int * first = (int *)objectGetAttr(argv[0], "value");
    int * secnd = (int *)objectGetAttr(argv[1], "value");
    return makeInteger(makeIntPtr(first[0] + secnd[0]));
}

Object __sub___integer(Object * argv)
{
    if (strcmp(argv[1].name, "int"))
    {
        char * err = malloc(17 + strlen(argv[1].name) + 32 + 1);
        strcpy(err, "types 'int' and '");
        strcat(err, argv[1].name);
        strcat(err, "' can not be subtracted with '-'");
        error(err, line_num);
        exit(1);
    }

    int * first = (int *)objectGetAttr(argv[0], "value");
    int * secnd = (int *)objectGetAttr(argv[1], "value");
    return makeInteger(makeIntPtr(first[0] - secnd[0]));
}

Object __mul___integer(Object * argv)
{
    if (strcmp(argv[1].name, "int"))
    {
        char * err = malloc(17 + strlen(argv[1].name) + 32 + 1);
        strcpy(err, "types 'int' and '");
        strcat(err, argv[1].name);
        strcat(err, "' can not be multiplied with '*'");
        error(err, line_num);
        exit(1);
    }

    int * first = (int *)objectGetAttr(argv[0], "value");
    int * secnd = (int *)objectGetAttr(argv[1], "value");
    return makeInteger(makeIntPtr(first[0] * secnd[0]));
}

Object __div___integer(Object * argv)
{
    if (strcmp(argv[1].name, "int"))
    {
        char * err = malloc(17 + strlen(argv[1].name) + 29 + 1);
        strcpy(err, "types 'int' and '");
        strcat(err, argv[1].name);
        strcat(err, "' can not be divided with '/'");
        error(err, line_num);
        exit(1);
    }

    int * first = (int *)objectGetAttr(argv[0], "value");
    int * secnd = (int *)objectGetAttr(argv[1], "value");

    if (!secnd[0])
        return makeInteger(makeIntPtr(0));

    return makeInteger(makeIntPtr(first[0] / secnd[0]));
}

Object __str___integer(Object * argv)
{
    int * thisvalue = ((int *)objectGetAttr(argv[0], "value"));
    return makeString(String(thisvalue[0]).value);
}

Object makeInteger(int * value)
{
    Object self;

    self = makeObject("int", value);

    // __add__
    self = addObjectValue(self, "__add__argc", &twoArgc);
    self = addObjectValue(self, "__add__", &__add___integer);

    // __sub__
    self = addObjectValue(self, "__sub__argc", &twoArgc);
    self = addObjectValue(self, "__sub__", &__sub___integer);

    // __mul__
    self = addObjectValue(self, "__mul__argc", &twoArgc);
    self = addObjectValue(self, "__mul__", &__mul___integer);

    // __div__
    self = addObjectValue(self, "__div__argc", &twoArgc);
    self = addObjectValue(self, "__div__", &__div___integer);

    // __str__
    self = addObjectValue(self, "__str__argc", &oneArgc);
    self = addObjectValue(self, "__str__", &__str___integer);

    return self;
}

Object __add___string(Object * argv)
{
    if (strcmp(argv[1].name, "string"))
    {
        char * err = malloc(66 + strlen(argv[1].name) + 1 + 1);
        strcpy(err, "only 'string' and 'string' can be concatenated, not 'string' and '");
        strcat(err, argv[1].name);
        strcat(err, "'");
        error(err, line_num);
        exit(1);
    }

    char * first = (char *)objectGetAttr(argv[0], "value");
    char * secnd = (char *)objectGetAttr(argv[1], "value");

    char * third = malloc(strlen(first) + strlen(secnd) + 1);
    strcpy(third, first);
    strcat(third, secnd);

    return makeString(third);
}

Object makeString(char * value)
{
    Object self;

    self = makeObject("string", value);

    self = addObjectValue(self, "__add__argc", &twoArgc);
    self = addObjectValue(self, "__add__", &__add___string);

    return self;
}

Object makeMethod(Object (*func)(Object * argv), int * argc)
{
    Object self;

    self = makeObject("method", argc);
    self = addObjectValue(self, "__call__", &func);

    return self;
}

char * quokka_interpret_line(char * linetext)
{
    char ** line = quokka_line_tok(linetext);
    return quokka_interpret_line_tokens(line);
}

char * quokka_interpret_line_tokens(char ** line)
{
    if (!strlen(line[0]))
        return 0;

    char * output = malloc(1);
    strcpy(output, "");

    if (verbose) println(line[0]);

    if (!strcmp(line[0], "LOAD_STRING"))
    {
        Object item = makeString(makeLiteralString(line[1]));

        pushTop(item);
    }
    else if (!strcmp(line[0], "LOAD_INT"))
    {
        Object item = makeInteger(makeIntPtrFromStr(line[1]));

        pushTop(item);
    }
    else if (!strcmp(line[0], "LOAD_NAME"))
    {
        pushTop(getVar(line[1]));
    }
    else if (!strcmp(line[0], "BINARY_ADD"))
    {
        Object secnd = popTop();
        Object first = popTop();

        if (!objectHasAttr(first, "__add__"))
        {
            char * err = malloc(6 + strlen(first.name) + 37 + 1);
            strcpy(err, "type '");
            strcat(err, first.name);
            strcat(err, "' does not have a method for addition");
            error(err, line_num);
            exit(1);
        }

        if (!objectHasAttr(first, "__add__argc"))
        {
            char * err = malloc(28 + strlen(first.name) + 56 + 1);
            strcpy(err, "the __add__ method of type '");
            strcat(err, first.name);
            strcat(err, "' is missing an argument limit, this should never happen");
            error(err, line_num);
            exit(1);
        }

        int funcargc = ((int *)objectGetAttr(first, "__add__argc"))[0];
        if (funcargc != 2)
            error("__add__ function requires an invalid amount of arguments, should be 2", line_num);

        Object arglist[2];
        arglist[0] = first;
        arglist[1] = secnd;

        pushTop(((standard_func_def)objectGetAttr(first, "__add__"))(arglist));
    }
    else if (!strcmp(line[0], "BINARY_SUB"))
    {
        Object secnd = popTop();
        Object first = popTop();

        if (!objectHasAttr(first, "__sub__"))
        {
            char * err = malloc(6 + strlen(first.name) + 37 + 1);
            strcpy(err, "type '");
            strcat(err, first.name);
            strcat(err, "' does not have a method for subtraction");
            error(err, line_num);
            exit(1);
        }

        if (!objectHasAttr(first, "__sub__argc"))
        {
            char * err = malloc(28 + strlen(first.name) + 56 + 1);
            strcpy(err, "the __sub__ method of type '");
            strcat(err, first.name);
            strcat(err, "' is missing an argument limit, this should never happen");
            error(err, line_num);
            exit(1);
        }

        int funcargc = ((int *)objectGetAttr(first, "__sub__argc"))[0];
        if (funcargc != 2)
            error("__sub__ function requires an invalid amount of arguments, should be 2", line_num);

        Object arglist[2];
        arglist[0] = first;
        arglist[1] = secnd;

        pushTop(((standard_func_def)objectGetAttr(first, "__sub__"))(arglist));
    }
    else if (!strcmp(line[0], "BINARY_MUL"))
    {
        Object secnd = popTop();
        Object first = popTop();

        if (!objectHasAttr(first, "__mul__"))
        {
            char * err = malloc(6 + strlen(first.name) + 43 + 1);
            strcpy(err, "type '");
            strcat(err, first.name);
            strcat(err, "' does not have a method for multiplication");
            error(err, line_num);
            exit(1);
        }

        if (!objectHasAttr(first, "__mul__argc"))
        {
            char * err = malloc(28 + strlen(first.name) + 56 + 1);
            strcpy(err, "the __mul__ method of type '");
            strcat(err, first.name);
            strcat(err, "' is missing an argument limit, this should never happen");
            error(err, line_num);
            exit(1);
        }

        int funcargc = ((int *)objectGetAttr(first, "__mul__argc"))[0];
        if (funcargc != 2)
            error("__mul__ function requires an invalid amount of arguments, should be 2", line_num);

        Object arglist[2];
        arglist[0] = first;
        arglist[1] = secnd;

        pushTop(((standard_func_def)objectGetAttr(first, "__mul__"))(arglist));
    }
    else if (!strcmp(line[0], "BINARY_DIV"))
    {
        Object secnd = popTop();
        Object first = popTop();

        if (!objectHasAttr(first, "__div__"))
        {
            char * err = malloc(6 + strlen(first.name) + 37 + 1);
            strcpy(err, "type '");
            strcat(err, first.name);
            strcat(err, "' does not have a method for division");
            error(err, line_num);
            exit(1);
        }

        if (!objectHasAttr(first, "__div__argc"))
        {
            char * err = malloc(28 + strlen(first.name) + 56 + 1);
            strcpy(err, "the __div__ method of type '");
            strcat(err, first.name);
            strcat(err, "' is missing an argument limit, this should never happen");
            error(err, line_num);
            exit(1);
        }

        int funcargc = ((int *)objectGetAttr(first, "__div__argc"))[0];
        if (funcargc != 2)
            error("__div__ function requires an invalid amount of arguments, should be 2", line_num);

        Object arglist[2];
        arglist[0] = first;
        arglist[1] = secnd;

        pushTop(((standard_func_def)objectGetAttr(first, "__div__"))(arglist));
    }
    else if (!strcmp(line[0], "STORE_NAME"))
    {
        addVar(line[1], popTop());
    }
    else if (!strcmp(line[0], "CALL_FUNCTION"))
    {
        int argcount = strtol(line[1], (char **)NULL, 10);
        Object * arglist = malloc(argcount * sizeof(Object));

        for (int i = 0; i < argcount; i++)
        {
            arglist[i] = popTop();
        }

        Object func = popTop();

        if (!objectHasAttr(func, "__call__"))
        {
            error("not callable", line_num);
            exit(1);
        }

        int funcargs = 0;
        funcargs = ((int *)objectGetAttr(func, "__call__argc"))[0];

        if (argcount > funcargs)
            error("function received too many arguments", line_num);
        if (argcount < funcargs)
        {
            int diff = argcount - funcargs;
            char * diffstr = malloc(11);
            sprintf(diffstr, "%d", diff);

            char * err;
            if (diff != 1)
                err = malloc(18 + strlen(diffstr) + 23 + 1 + 1);
            else err = malloc(18 + strlen(diffstr) + 23 + 1);

            strcpy(err, "function requires ");
            strcat(err, strndup(diffstr, strlen(diffstr)));
            strcat(err, " more argument");
            if (diff != 1)
                strcat(err, "s");

            free(diffstr);
            error(err, line_num);
        }

        pushTop(((standard_func_def)objectGetAttr(func, "__call__"))(arglist));
    }

    return output;
}

char * quokka_interpret_tokens(char ** tokens)
{
    char * output = malloc(1);
    strcpy(output, "");

    int size = arrsize(tokens);
    for (int i = 0; i < size; i++)
    {
        if (tokens[i] == NULL)
            continue;

        if (isinteger(tokens[i]))
        {
            line_num = strtol(tokens[i], (char **)NULL, 10) - 1;
            continue;
        }

        strcat(output, quokka_interpret_line(tokens[i]));
    }

    free(tokens);

    return output;
}

char * quokka_interpret(char * bytecode)
{
    char ** tokens = malloc(2048 * sizeof(char *));
    ntokenise(tokens, bytecode, "\n");

    char * res = quokka_interpret_tokens(tokens);

    return res;
}
