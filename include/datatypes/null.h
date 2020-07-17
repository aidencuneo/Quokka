Object * __eq___null(int argc, Object ** argv)
{
    return getIntConst(0);
}

Object * __disp___null(int argc, Object ** argv)
{
    return makeString("null", 0);
}

Object * __bool___null(int argc, Object ** argv)
{
    return getIntConst(0);
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

    return self;
}
