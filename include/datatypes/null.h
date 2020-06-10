Object * __sizeof___null(int argc, Object ** argv)
{
    int * size = makeIntPtr(sizeof(0));
    // pushTrash(size);

    return makeInt(size, 1);
}

Object * __disp___null(int argc, Object ** argv)
{
    return makeString("null", 0);
}

Object * __bool___null(int argc, Object ** argv)
{
    return makeInt(&falsePtr, 0);
}

Object * makeNull()
{
    return constants[0];
}

Object * makeNullRaw()
{
    Object * self = objectPointer();

    self->name = "null";

    // 7 Attributes
    self->names = malloc(7 * sizeof(char *));
    self->values = malloc(7 * sizeof(void *));
    self->value_count = 0;

    self = objectAddAttr(self, "value", &falsePtr);

    // One argument methods

    // __sizeof__
    self = objectAddAttr(self, "__sizeof__", &__sizeof___null);
    self = objectAddAttr(self, "__sizeof__argc", &oneArgc);

    // __disp__
    self = objectAddAttr(self, "__disp__", &__disp___null);
    self = objectAddAttr(self, "__disp__argc", &oneArgc);

    // __bool__
    self = objectAddAttr(self, "__bool__", &__bool___null);
    self = objectAddAttr(self, "__bool__argc", &oneArgc);

    return self;
}
