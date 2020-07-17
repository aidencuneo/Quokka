Object * _os_chdir(int argc, Object ** argv)
{
    char * text;

    // If first argument is not a string, try to convert it into one
    if (strcmp(argv[1]->name, "string"))
    {
        Object * tostring = q_function_string(1, &argv[1]);
        text = objectGetAttr(tostring, "value");
        // free(tostring);
    }
    else
    {
        text = objectGetAttr(argv[1], "value");
    }

    char * directory = getrealpath(text);

    if (!directory)
    {
        char * err = malloc(11 + strlen(text) + 11 + 1);
        strcpy(err, "directory '");
        strcat(err, text);
        strcat(err, "' not found");
        error(err, line_num);
    }

    chdir(directory);

    return makeNull();
}

Object * _os_cls(int argc, Object ** argv)
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    return makeNull();
}

Object * _os_cwd(int argc, Object ** argv)
{
    return makeString(getrealpath("."), 1);
}

Object * _os_system(int argc, Object ** argv)
{
    // If first argument is a string, use it
    if (!strcmp(argv[1]->name, "string"))
    {
        char * text = objectGetAttr(argv[1], "value");

        return makeInt(makeIntPtr(system(text)), 1, 10);
    }

    // If first argument is not a string, try to convert it into one
    Object * tostring = q_function_string(1, &argv[1]);

    char * text = tostring->values[0];

    Object * exit_code = makeInt(makeIntPtr(system(text)), 1, 10);

    freeObject(tostring);

    return exit_code;
}

// Import os module (with packing)
Object * _os_import_module()
{
    char * name = malloc(2 + 1);
    strcpy(name, "os");

    // (name, attribute_count)
    Object * _os_module = makeModule(name, 4);

    // chdir
    Object * _os_chdir_method = makeCMethod(_os_module, &_os_chdir, 1, 1);
    _os_chdir_method->refs++;
    objectAddAttr(_os_module, "chdir", _os_chdir_method);

    // cls
    Object * _os_cls_method = makeCMethod(_os_module, &_os_cls, 0, 0);
    _os_cls_method->refs++;
    objectAddAttr(_os_module, "cls", _os_cls_method);

    // cwd
    Object * _os_cwd_method = makeCMethod(_os_module, &_os_cwd, 0, 0);
    _os_cwd_method->refs++;
    objectAddAttr(_os_module, "cwd", _os_cwd_method);

    // system
    Object * _os_system_method = makeCMethod(_os_module, &_os_system, 1, 1);
    _os_system_method->refs++;
    objectAddAttr(_os_module, "system", _os_system_method);

    return _os_module;
}

/* This module can not be imported without packing */

// Import os module (without packing)
// void _os_import_standard() {}
