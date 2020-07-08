Object * __neg___file(int argc, Object ** argv)
{
    int * opened = objectGetAttr(argv[0], "opened");

    if (opened[0])
    {
        FILE * f = argv[0]->values[0];
        fclose(f);
    }

    opened[0] = 0;

    return makeNull();
}

Object * __bool___file(int argc, Object ** argv)
{
    return makeInt(objectGetAttr(argv[0], "opened"), 0, 10);
}

Object * __free___file(int argc, Object ** argv)
{
    int * opened = objectGetAttr(argv[0], "opened");

    if (opened[0])
    {
        FILE * f = argv[0]->values[0];
        fclose(f);
    }

    free((int *)opened);

    return makeNull();
}

Object * exists_file(int argc, Object ** argv)
{
    return makeInt(objectGetAttr(argv[0], "opened"), 0, 10);
}

Object * read_file(int argc, Object ** argv)
{
    FILE * f = argv[0]->values[0];
    int * opened = objectGetAttr(argv[0], "opened");
    char * mode = objectGetAttr(argv[0], "mode");

    if (!f)
        opened[0] = 0;

    if (!opened[0] || (
        strcmp(mode, "r")   &&
        strcmp(mode, "rb")  &&
        strcmp(mode, "r+")  &&
        strcmp(mode, "rb+") &&
        strcmp(mode, "r+b")))
    {
        return makeNull();
    }

    char * buffer;
    int length;

    fseek(f, 0, SEEK_END);
    length = ftell(f);
    fseek(f, 0, SEEK_SET);

    buffer = malloc(length + 1);
    strcpy(buffer, "");

    if (buffer)
    {
        fread(buffer, 1, length, f);
        buffer[length] = 0;
    }

    return makeString(buffer, 1);
}

Object * write_file(int argc, Object ** argv)
{
    FILE * f = argv[0]->values[0];
    int * opened = objectGetAttr(argv[0], "opened");
    char * mode = objectGetAttr(argv[0], "mode");

    Object ** arglist = makeArglist(argv[1]);
    Object * strtext = q_function_string(1, arglist);
    free(arglist);

    if (!f)
        opened[0] = 0;

    if (!opened[0] || (
        strcmp(mode, "w")   &&
        strcmp(mode, "wb")  &&
        strcmp(mode, "w+")  &&
        strcmp(mode, "wb+") &&
        strcmp(mode, "w+b")))
    {
        return makeNull();
    }

    fprintf(f, "%s", (char *)strtext->values[0]);

    fseek(f, 0, SEEK_END);
    int length = ftell(f);
    fseek(f, 0, SEEK_SET);

    return makeInt(makeIntPtr(length), 1, 10);
}

Object * close_file(int argc, Object ** argv)
{
    int * opened = objectGetAttr(argv[0], "opened");

    if (opened[0])
    {
        FILE * f = argv[0]->values[0];
        fclose(f);
    }

    opened[0] = 0;

    return makeNull();
}

Object * makeFile(char * path, char * mode)
{
    Object * self = objectPointer();

    self->name = "file";

    // 3 Attributes
    self->names = malloc(3 * sizeof(char *));
    self->values = malloc(3 * sizeof(void *));

    FILE * f = fopen(path, mode);

    self = objectAddAttr(self, "value", f);
    self = objectAddAttr(self, "mode", mode);
    self = objectAddAttr(self, "opened", makeIntPtr(f != NULL));

    return self;
}
