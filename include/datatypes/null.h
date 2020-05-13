Object __disp___null(int argc, Object * argv)
{
    return makeString("null");
}

Object __bool___null(int argc, Object * argv)
{
    return makeInteger(&falsePtr);
}

Object makeNull()
{
    Object self;

    self = makeObject("null", &falsePtr);

    // One argument methods

    // __disp__
    self = addObjectValue(self, "__disp__argc", &oneArgc);
    self = addObjectValue(self, "__disp__", &__disp___null);

    // __bool__
    self = addObjectValue(self, "__bool__argc", &oneArgc);
    self = addObjectValue(self, "__bool__", &__bool___null);

    return self;
}
