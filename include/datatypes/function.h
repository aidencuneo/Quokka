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

    addVar("argc", makeInt(intptr, 1));
    int argc_var = locals.count - 1;

    Object ** arglist = malloc(argc * sizeof(Object *));
    for (int i = 0; i < argc; i++)
    {
        arglist[i] = argv[i + 1];
        arglist[i]->refs++;
    }

    addVar("argv", makeList(argc, arglist, 0));
    int argv_var = locals.count - 1;

    free(arglist);

    // Interpret
    quokka_interpret(code);

    // Clear argc and argv
    // addVar("argc", makeNull());
    // addVar("argv", makeNull());

    // Reset filepath
    current_file = old_file;

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
        pushTopM(old_stack[i]);
    }

    free(old_stack);

    // Recreate and realign variable lists (only locals for now)

    locals.offset = old_locals_offset;

    // If there's anything to return, return it
    if (ret_stack_size)
    {
        return popRetTop();
    }

    // Return null by default
    Object * null_obj = makeNull();
    null_obj->refs++;

    return null_obj;
}

Object * __free___function(int argc, Object ** argv)
{
    char * filepath = objectGetAttr(argv[0], "filepath");
    char * value = objectGetAttr(argv[0], "value");

    free(filepath);
    free(value);

    int * argminptr = objectGetAttr(argv[0], "__call__argmin");
    int * argmaxptr = objectGetAttr(argv[0], "__call__argmax");

    free(argminptr);
    free(argmaxptr);

    return makeNull();
}

Object * makeFunction(char * filepath, char ** bytecode, int argmin, int argmax)
{
    int * argminptr = makeIntPtr(argmin);
    int * argmaxptr = makeIntPtr(argmax);

    Object * self = objectPointer();

    self->name = "function";

    // 6 Attributes
    self->names = malloc(6 * sizeof(char *));
    self->values = malloc(6 * sizeof(void *));
    self->value_count = 0;

    // Values
    self = objectAddAttr(self, "value", *bytecode);
    self = objectAddAttr(self, "filepath", filepath);

    // __call__
    self = objectAddAttr(self, "__call__", &__call___function);
    self = objectAddAttr(self, "__call__argmin", argminptr);
    self = objectAddAttr(self, "__call__argmax", argmaxptr);

    // __free__
    self = objectAddAttr(self, "__free__", &__free___function);

    return self;
}
