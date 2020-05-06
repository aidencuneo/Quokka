void interp_init();
char * quokka_interpret_line(char * linetext);
char * quokka_interpret_line_tokens(char ** line);
char * quokka_interpret_tokens(char ** tokens);
char * quokka_interpret(char * bytecode);

// Important stuff
Varlist vars;
Object * stack;
int stack_size;

// Function stuff
typedef Object (*standard_func_def)(Object * argv);
int oneArgc;

void interp_init()
{
    stack = malloc(sizeof(char *));
    stack_size = 0;

    oneArgc = 1;

    Object printObj = emptyObject("function");
    printObj = addObjectValue(printObj, "__call__", (&qprint));
    printObj = addObjectValue(printObj, "_argc", &oneArgc);
    addVar("print", printObj);

    Object printlnObj = emptyObject("function");
    printlnObj = addObjectValue(printlnObj, "__call__", (&qprintln));
    printlnObj = addObjectValue(printlnObj, "_argc", &oneArgc);
    addVar("println", printlnObj);
}

Object emptyObject(char * name)
{
    Object self;

    self.name = name;
    self.value_count = 0;

    self.names = malloc(sizeof(char *));
    self.values = malloc(sizeof(void *));

    return self;
}

Object makeObject(char * name, void * value)
{
    Object self;

    self.name = name;
    self.value_count = 0;

    self.names = malloc(sizeof(char *));
    self.values = malloc(sizeof(void *));

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
        Object item = makeObject("string", __slice_string__(String(line[1]), 1, 1).value);

        pushTop(item);
    }
    else if (!strcmp(line[0], "LOAD_INT"))
    {
        int * n = malloc(sizeof(int));
        n[0] = strtol(line[1], (char **)NULL, 10);
        Object item = makeObject("int", n);

        pushTop(item);
    }
    else if (!strcmp(line[0], "STORE_NAME"))
    {
        addVar(line[1], popTop());
    }
    else if (!strcmp(line[0], "LOAD_NAME"))
    {
        pushTop(getVar(line[1]));
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
            error("not callable", line_num);

        int funcargs = 0;
        if (objectHasAttr(func, "_argc"))
            funcargs = ((int *)objectGetAttr(func, "_argc"))[0];

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
            strcat(err, " more position argument");
            if (diff != 1)
                strcat(err, "s");

            free(diffstr);
            error(err, line_num);
        }

        ((standard_func_def)objectGetAttr(func, "__call__"))(arglist);

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
