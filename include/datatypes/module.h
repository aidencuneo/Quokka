// Object * __disp___module(int argc, Object ** argv)
// {
//     return makeString("<module>", 0);
// }

Object * __free___module(int argc, Object ** argv)
{
    char * name = objectGetAttr(argv[0], "value");
    free(name);

    // Remember to change this number to match the
    // initial number of attributes in a module (int i)
    for (int i = 2; i < argv[0]->value_count; i++)
    {
        objUnref(argv[0]->values[i]);
    }

    return makeNull();
}

// Module name should be a malloc'd pointer
Object * makeModule(char * name, int item_count)
{
    Object * self = objectPointer();

    self->name = "module";

    // 2 Attributes
    self->names = malloc((2 + item_count) * sizeof(char *));
    self->values = malloc((2 + item_count) * sizeof(void *));
    self->value_count = 0;

    self = objectAddAttr(self, "value", name);

    // One argument methods

    // __free__
    self = objectAddAttr(self, "__free__", &__free___module);

    return self;
}
