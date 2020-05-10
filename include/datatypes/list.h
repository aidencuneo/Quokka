// Object __index___list(Object * argv)
// {
//     if (strcmp(argv[1].name, "int"))
//     {
//         char * err = malloc(40 + strlen(argv[1].name) + 1 + 1);
//         strcpy(err, "list index argument must be 'int', not '");
//         strcat(err, argv[1].name);
//         strcat(err, "'");
//         error(err, line_num);
//     }

//     int ind = ((int *)objectGetAttr(argv[1], "value"))[0];
//     int len = strlen((char *)objectGetAttr(argv[0], "value"));

//     if (ind >= len)
//         return makeString("");
//     else if (ind < 0)
//         return makeString("");

//     char ch = ((char *)objectGetAttr(argv[0], "value"))[ind];

//     char * chst = malloc(2);
//     chst[0] = ch;
//     chst[1] = '\0';

//     return makeString(chst);
// }

Object __copy___list(Object * argv)
{
    return makeList(
        ((int *)objectGetAttr(argv[0], "length"))[0],
        (Object *)objectGetAttr(argv[0], "value"));
}

// Object __bool___list(Object * argv)
// {
//     Object * thisvalue = ((Object *)objectGetAttr(argv[0], "value"));

//     if (thisvalue[0] != NULL)
//         return makeInteger(&truePtr);
//     return makeInteger(&falsePtr);
// }

Object makeList(int length, Object * value)
{
    Object self;

    Object * lst = malloc(length * sizeof(Object));

    for (int i = 0; i < length; i++)
        lst[i] = value[i];

    self = makeObject("list", lst);
    self = addObjectValue(self, "length", makeIntPtr(length));

    println(((int *)objectGetAttr(self, "length"))[0]);

    // Two argument methods

    // // __index__
    // self = addObjectValue(self, "__index__argc", &twoArgc);
    // self = addObjectValue(self, "__index__", &__index___list);

    // __copy__
    self = addObjectValue(self, "__copy__argc", &twoArgc);
    self = addObjectValue(self, "__copy__", &__copy___list);

    // One argument methods

    // // __bool__
    // self = addObjectValue(self, "__bool__argc", &oneArgc);
    // self = addObjectValue(self, "__bool__", &__bool___list);

    return self;
}
