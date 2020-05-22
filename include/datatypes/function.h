Object __call___function(int argc, int * argv)
{
    // Get function code
    char * code = objectGetAttr(mem[argv[0]], "value");

    // Set up the environment for the function call
    int * old_stack = malloc(stack_size * sizeof(int));
    for (int i = 0; i < stack_size; i++)
        old_stack[i] = stack[i];
    int old_stack_size = stack_size;

    char ** old_locals_names = malloc(locals.count * sizeof(char *));
    int * old_locals_values = malloc(locals.count * sizeof(int));
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
    int * intptr = makeIntPtr(argc);
    pushTrash(intptr);

    addVar("argc", makeInt(intptr));

    int * arglist = malloc(argc * sizeof(int));
    for (int i = 0; i < argc; i++)
        arglist[i] = argv[i + 1];

    addVar("argv", makeList(argc, arglist, 0));

    // Interpret
    quokka_interpret(code);

    stack = realloc(stack, sizeof(int));
    stack_size = 0;

    for (int i = 0; i < old_stack_size; i++)
        pushTopIndex(old_stack[i]);

    free(old_stack);

    free(locals.names);
    free(locals.values);

    locals.count = old_locals_count;
    locals.names = malloc((locals.count + 1) * sizeof(char *));
    locals.values = malloc((locals.count + 1) * sizeof(int));

    for (int i = 0; i < locals.count; i++)
    {
        locals.names[i] = old_locals_names[i];
        locals.values[i] = old_locals_values[i];
    }

    free(old_locals_names);
    free(old_locals_values);

    bc_line = old_bc_line;
    bc_line_count = old_bc_line_count;
    bc_tokens = old_bc_tokens;
    can_return = old_can_return;

    if (ret_stack_size)
    {
        int * ret = makeIntPtr(popRetTop());
        pushTrash(ret);

        return makeInt(ret);
    }

    // Return null
    pushMem(makeNull());

    int * ret = makeIntPtr(memsize - 1);
    pushTrash(ret);

    return makeInt(ret);
}

Object makeFunction(char ** bytecode, int argmin, int argmax)
{
    int * argminptr = makeIntPtr(argmin);
    int * argmaxptr = makeIntPtr(argmax);
    pushTrash(argminptr);
    pushTrash(argmaxptr);

    Object self;

    self = makeObject("function", *bytecode);

    // __call__
    self = addObjectValue(self, "__call__", &__call___function);
    self = addObjectValue(self, "__call__argmin", argminptr);
    self = addObjectValue(self, "__call__argmax", argmaxptr);

    return self;
}
