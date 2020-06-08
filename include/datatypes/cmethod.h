Object * __call___cmethod(int argc, Object ** argv)
{
    Object * self = objectGetAttr(argv[0], "parent");
    char * func_pointer = objectGetAttr(argv[0], "value");

    Object ** arglist = malloc((argc + 1) * sizeof(Object *));
    arglist[0] = self;
    for (int i = 1; i < argc; i++)
        arglist[i] = argv[i];

    Object * res = ((standard_func_def)func_pointer)(argc, arglist);

    free(arglist);

    return res;
}

Object * __free___cmethod(int argc, Object ** argv)
{
    int * argminptr = objectGetAttr(argv[0], "__call__argmin");
    int * argmaxptr = objectGetAttr(argv[0], "__call__argmax");

    free(argminptr);
    free(argmaxptr);

    return makeNull();
}

Object * makeCMethod(Object * parent, void * method_pointer, int argmin, int argmax)
{
    int * argminptr = makeIntPtr(argmin);
    int * argmaxptr = makeIntPtr(argmax);

    Object * self = objectPointer();

    self->name = "cmethod";

    // 6 Attributes
    self->names = malloc(6 * sizeof(char *));
    self->values = malloc(6 * sizeof(void *));
    self->value_count = 0;

    // Values
    self = objectAddAttr(self, "value", method_pointer);
    self = objectAddAttr(self, "parent", parent);

    // __call__
    self = objectAddAttr(self, "__call__", &__call___cmethod);
    self = objectAddAttr(self, "__call__argmin", argminptr);
    self = objectAddAttr(self, "__call__argmax", argmaxptr);

    // __free__
    self = objectAddAttr(self, "__free__", &__free___cmethod);

    return self;
}
