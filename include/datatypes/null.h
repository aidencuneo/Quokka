Object * __sizeof___null(int argc, Object ** argv)
{
    int * size = makeIntPtr(sizeof(argv[0]));
    // int * size = makeIntPtr(sizeof(0));

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

    // 0 Attributes
    self->names = malloc(0);
    self->values = malloc(0);
    self->value_count = 0;

    // self = objectAddAttr(self, "value", &falsePtr);

    // One argument methods

    return self;
}
