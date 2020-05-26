Object __sizeof___null(int argc, Object * argv)
{
    int * size = makeIntPtr(sizeof(0));
    pushTrash(size);

    return makeInt(size);
}

Object __disp___null(int argc, Object * argv)
{
    return makeString("null");
}

Object __bool___null(int argc, Object * argv)
{
    return makeInt(&falsePtr);
}

Object makeNull()
{
    Object self;

    self = makeObject("null", &falsePtr);

    // One argument methods

    // __sizeof__
    self = addObjectValue(self, "__sizeof__", &__sizeof___null);
    self = addObjectValue(self, "__sizeof__argc", &oneArgc);

    // __disp__
    self = addObjectValue(self, "__disp__", &__disp___null);
    self = addObjectValue(self, "__disp__argc", &oneArgc);

    // __bool__
    self = addObjectValue(self, "__bool__", &__bool___null);
    self = addObjectValue(self, "__bool__argc", &oneArgc);

    return self;
}
