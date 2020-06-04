Object * __call___function(int argc, Object ** argv)
{
    // Get function code
    char * filepath = objectGetAttr(argv[0], "filepath");
    char * code = objectGetAttr(argv[0], "value");

    // Set up the environment for the function call
    char * old_file = current_file;
    current_file = filepath;

    Object ** old_stack = malloc(stack_size * sizeof(Object *));
    for (int i = 0; i < stack_size; i++)
        old_stack[i] = stack[i];
    int old_stack_size = stack_size;

    stack_alloc = stack_alloc_size;
    stack = realloc(stack, stack_alloc * sizeof(Object *));
    stack_size = 0;

    // char ** old_locals_names = malloc(locals.count * sizeof(char *));
    // Object * old_locals_values = malloc(locals.count * sizeof(Object *));
    // for (int i = 0; i < locals.count; i++)
    // {
    //     old_locals_names[i] = locals.names[i];
    //     old_locals_values[i] = locals.values[i];
    // }
    // int old_locals_count = locals.count;

    // Function call will not affect currently defined locals
    int old_locals_offset = locals.offset;
    locals.offset = locals.count;
    // locals.count = 0;

    int old_bc_line = bc_line;
    int old_bc_line_count = bc_line_count;
    char ** old_bc_tokens = bc_tokens;
    int old_can_return = can_return;

    can_return = 1;

    // Vars
    int * intptr = makeIntPtr(argc);
    // pushTrash(intptr);

    addVar("argc", makeInt(intptr, 1));

    Object ** arglist = malloc(argc * sizeof(Object *));
    for (int i = 0; i < argc; i++)
        arglist[i] = argv[i + 1];

    addVar("argv", makeList(argc, arglist, 0));

    free(arglist);

    // Interpret
    quokka_interpret(code);

    // Reset filepath
    current_file = old_file;

    // int argcind = getVarIndex("argc");
    // int argvind = getVarIndex("argv");

    // if (argcind != -1)
    //     delLVarIndex(argcind);

    // if (argvind != -1)
    //     delLVarIndex(argvind);

    bc_line = old_bc_line;
    bc_line_count = old_bc_line_count;
    bc_tokens = old_bc_tokens;
    can_return = old_can_return;

    // Recreate and realign previous stack
    stack_alloc = stack_alloc_size;
    stack = realloc(stack, stack_alloc * sizeof(Object *));
    stack_size = 0;

    for (int i = 0; i < old_stack_size; i++)
    {
        pushTop(old_stack[i]);
    }

    free(old_stack);

    // Recreate and realign variable lists (only locals for now)
    locals.offset = old_locals_offset;
    // free(locals.names);
    // free(locals.values);

    // locals.count = old_locals_count;
    // locals.names = malloc((locals.count + 1) * sizeof(char *));
    // locals.values = malloc((locals.count + 1) * sizeof(Object *));

    // for (int i = 0; i < locals.count; i++)
    // {
    //     locals.names[i] = old_locals_names[i];
    //     locals.values[i] = old_locals_values[i];
    // }

    // free(old_locals_names);
    // free(old_locals_values);

    // If there's anything to return, return it
    if (ret_stack_size)
    {
        return popRetTop();
    }

    // Return null by default
    return makeNull();
}

Object * makeFunction(char * filepath, char ** bytecode, int argmin, int argmax)
{
    int * argminptr = makeIntPtr(argmin);
    int * argmaxptr = makeIntPtr(argmax);
    // pushTrash(argminptr);
    // pushTrash(argmaxptr);

    Object * self = objectPointer();

    self->name = "function";

    // 5 Attributes
    self->names = malloc(5 * sizeof(char *));
    self->values = malloc(5 * sizeof(void *));
    self->value_count = 0;

    // Values
    self = objectAddAttr(self, "value", *bytecode);
    self = objectAddAttr(self, "filepath", filepath);

    // __call__
    self = objectAddAttr(self, "__call__", &__call___function);
    self = objectAddAttr(self, "__call__argmin", argminptr);
    self = objectAddAttr(self, "__call__argmax", argmaxptr);

    return self;
}
