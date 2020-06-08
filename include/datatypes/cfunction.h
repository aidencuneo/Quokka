Object * __call___cfunction(int argc, Object ** argv)
{
    char * func_pointer = objectGetAttr(argv[0], "value");

    Object ** arglist = malloc(argc * sizeof(Object *));
    for (int i = 0; i < argc; i++)
        arglist[i] = argv[i + 1];

    Object * res = ((standard_func_def)func_pointer)(argc, arglist);

    free(arglist);

    return res;
}

Object * __free___cfunction(int argc, Object ** argv)
{
    int * argminptr = objectGetAttr(argv[0], "__call__argmin");
    int * argmaxptr = objectGetAttr(argv[0], "__call__argmax");

    free(argminptr);
    free(argmaxptr);

    return makeNull();
}

Object * makeCFunction(void * func_pointer, int argmin, int argmax)
{
    int * argminptr = makeIntPtr(argmin);
    int * argmaxptr = makeIntPtr(argmax);

    Object * self = objectPointer();

    self->name = "cfunction";

    // 5 Attributes
    self->names = malloc(5 * sizeof(char *));
    self->values = malloc(5 * sizeof(void *));
    self->value_count = 0;

    // Values
    self = objectAddAttr(self, "value", func_pointer);

    // __call__
    self = objectAddAttr(self, "__call__", &__call___cfunction);
    self = objectAddAttr(self, "__call__argmin", argminptr);
    self = objectAddAttr(self, "__call__argmax", argmaxptr);

    // __free__
    self = objectAddAttr(self, "__free__", &__free___cfunction);

    return self;
}
