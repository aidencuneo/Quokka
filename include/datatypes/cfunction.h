Object * __free___cfunction(int argc, Object ** argv)
{
    int * argcptr = objectGetAttr(argv[0], "argc");
    free(argcptr);

    return makeNull();
}

Object * makeCFunction(void * func_pointer, int argmin, int argmax)
{
    int * argcptr = malloc(2 * sizeof(int));
    argcptr[0] = argmin;
    argcptr[1] = argmax;

    Object * self = objectPointer();

    self->name = "cfunction";

    // 2 Attributes
    self->names = malloc(2 * sizeof(char *));
    self->values = malloc(2 * sizeof(void *));

    // Values
    self = objectAddAttr(self, "__call__", func_pointer);
    self = objectAddAttr(self, "argc", argcptr);

    return self;
}
