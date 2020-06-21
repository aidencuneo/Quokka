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

    // 1 Attribute
    self->names = malloc((1 + item_count) * sizeof(char *));
    self->values = malloc((1 + item_count) * sizeof(void *));
    self->value_count = 0;

    self = objectAddAttr(self, "value", name);

    return self;
}
