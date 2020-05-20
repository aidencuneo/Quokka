Object __call___function(int argc, Object * argv)
{
    // Get function code
    char * code = objectGetAttr(argv[0], "value");

    // Set up the environment for the function call
    Object * old_stack = malloc(stack_size * sizeof(Object));
    for (int i = 0; i < stack_size; i++)
        old_stack[i] = stack[i];
    int old_stack_size = stack_size;

    char ** old_locals_names = malloc(locals.count * sizeof(char *));
    Object * old_locals_values = malloc(locals.count * sizeof(Object));
    for (int i = 0; i < locals.count; i++)
    {
        old_locals_names[i] = locals.names[i];
        old_locals_values[i] = locals.values[i];
    }
    int old_locals_count = locals.count;

    int old_bc_line = bc_line;
    int old_bc_line_count = bc_line_count;
    char ** old_bc_tokens = bc_tokens;
    int old_can_return = can_return;

    can_return = 1;

    // Vars
    addVar("argc", makeInt(makeIntPtr(argc)));

    Object * arglist = malloc(argc * sizeof(Object));
    for (int i = 0; i < argc; i++)
        arglist[i] = argv[i + 1];

    addVar("argv", makeList(argc, arglist, 0));

    // Interpret
    quokka_interpret(strndup(code, strlen(code)));

    stack = realloc(stack, sizeof(Object));
    stack_size = 0;

    for (int i = 0; i < old_stack_size; i++)
        pushTop(old_stack[i]);

    // Try free(old_stack);

    free(locals.names);
    free(locals.values);

    locals.count = old_locals_count;
    locals.names = malloc((locals.count + 1) * sizeof(char *));
    locals.values = malloc((locals.count + 1) * sizeof(Object));

    for (int i = 0; i < locals.count; i++)
    {
        locals.names[i] = old_locals_names[i];
        locals.values[i] = old_locals_values[i];
    }

    bc_line = old_bc_line;
    bc_line_count = old_bc_line_count;
    bc_tokens = old_bc_tokens;
    can_return = old_can_return;

    if (ret_stack_size)
        return popRetTop();

    return makeNull();
}

Object makeFunction(char ** bytecode, int argmin, int argmax)
{
    Object self;

    self = makeObject("function", *bytecode);

    // __call__
    self = addObjectValue(self, "__call__", &__call___function);
    self = addObjectValue(self, "__call__argmin", makeIntPtr(argmin));
    self = addObjectValue(self, "__call__argmax", makeIntPtr(argmax));

    return self;
}
