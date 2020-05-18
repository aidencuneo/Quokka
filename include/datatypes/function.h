Object __call___function(int argc, Object * argv)
{
    char * code = objectGetAttr(argv[0], "value");

    Object * old_stack = malloc(stack_size * sizeof(Object));
    for (int i = 0; i < stack_size; i++)
        old_stack[i] = stack[i];
    int old_stack_size = stack_size;

    int old_bc_line = bc_line;
    int old_bc_line_count = bc_line_count;
    char ** old_bc_tokens = bc_tokens;
    int old_can_return = can_return;

    can_return = 1;

    quokka_interpret(strndup(code, strlen(code)));

    stack = realloc(stack, sizeof(Object));
    stack_size = 0;

    for (int i = 0; i < old_stack_size; i++)
        pushTop(old_stack[i]);

    bc_line = old_bc_line;
    bc_line_count = old_bc_line_count;
    bc_tokens = old_bc_tokens;
    can_return = old_can_return;

    if (ret_stack_size)
        return popRetTop();

    return makeNull();
}

Object makeFunction(char ** bytecode)
{
    Object self;

    self = makeObject("function", *bytecode);

    // __call__
    self = addObjectValue(self, "__call__", &__call___function);
    // self = addObjectValue(self, "__call__argmin", ?);
    // self = addObjectValue(self, "__call__argmax", ?);

    return self;
}
