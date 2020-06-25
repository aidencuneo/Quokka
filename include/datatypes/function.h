Object * __call___function(int argc, Object ** argv)
{
    // Get function code
    char * filepath = objectGetAttr(argv[0], "filepath");
    char * code = objectGetAttr(argv[0], "value");

    // Set up the environment for the function call
    char * old_file = current_file;
    current_file = filepath;

    int old_can_return = can_return;
    can_return = 1;

    // Variables
    int old_locals_count = locals.count;
    int old_locals_offset = locals.offset;
    locals.offset = locals.count;

    // Arguments
    int * intptr = makeIntPtr(argc);

    addVar("argc", makeInt(intptr, 1, 1));
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

    // End
    current_file = old_file;
    can_return = old_can_return;

    // Unreference all the new variables that got created
    // during this function call
    for (int i = old_locals_count; i < locals.count; i++)
        objUnref(locals.values[i]);

    locals.count = old_locals_count;
    locals.offset = old_locals_offset;

    // If there's anything to return, return it
    if (ret_stack_size)
    {
        Object * ret = popRetTop();
        ret->refs--;

        return ret;
    }

    // Return null by default
    return makeNull();
}

Object * __free___function(int argc, Object ** argv)
{
    char * value = objectGetAttr(argv[0], "value");
    char * filepath = objectGetAttr(argv[0], "filepath");

    free(value);
    free(filepath);

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
