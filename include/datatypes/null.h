Object __bool___null(int argc, Object * argv)
{
    return makeInteger(&falsePtr);
}

Object makeNull()
{
    Object self;

    self = makeObject("null", &falsePtr);

    // One argument methods

    // __bool__
    self = addObjectValue(self, "__bool__argc", &oneArgc);
    self = addObjectValue(self, "__bool__", &__bool___null);

    return self;
}
