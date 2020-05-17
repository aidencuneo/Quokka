Object __call___function(int argc, Object * argv)
{
    char * code = objectGetAttr(argv[0], "value");

    int old_bc_line = bc_line;
    int old_bc_line_count = bc_line_count;
    char ** old_bc_tokens = bc_tokens;

    quokka_interpret(strndup(code, strlen(code)));

    bc_line = old_bc_line;
    bc_line_count = old_bc_line_count;
    bc_tokens = old_bc_tokens;

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
