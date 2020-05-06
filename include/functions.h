Object qprint(Object * argv)
{
    if (!strcmp(argv[0].name, "string"))
    {
        print((char *)objectGetAttr(argv[0], "value"));
    }
    else if (!strcmp(argv[0].name, "int"))
    {
        print(((int *)objectGetAttr(argv[0], "value"))[0]);
    }
    else
    {
        error("'print' can only print standard Quokka types", line_num);
        exit(1);
    }
}

Object qprintln(Object * argv)
{
    if (!strcmp(argv[0].name, "string"))
    {
        println((char *)objectGetAttr(argv[0], "value"));
    }
    else if (!strcmp(argv[0].name, "int"))
    {
        println(((int *)objectGetAttr(argv[0], "value"))[0]);
    }
    else
    {
        error("'println' can only print standard Quokka types", line_num);
        exit(1);
    }
}
