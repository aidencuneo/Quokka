Object * quokka_import_module(char ** module_name, char * path)
{
    *module_name = strdup(path);

    char * import_path_rel = malloc(strlen(path) + 2 + 1);
    strcpy(import_path_rel, path);

    if (!endswith(path, ".q"))
        strcat(import_path_rel, ".q");

    char * import_path = getrealpath(import_path_rel);

    if (!import_path)
    {
        char * err = malloc(49 + strlen(import_path_rel) + 1 + 1);
        strcpy(err, "(import) file path not found or not accessible: '");
        strcat(err, import_path_rel);
        strcat(err, "'");
        error(err, line_num);
    }

    // Set up the environment for the calls
    char * old_file = current_file;
    current_file = import_path;

    // Compile imported file
    char * imported_bytecode = quokka_compile_fname(import_path);

    // Variables
    int old_locals_count = locals.count;
    int old_globals_count = globals.count;

    // Interpret the imported file
    quokka_interpret(imported_bytecode);

    // Generate a module using the variables that were created inside the imported script
    Object * imported_module = makeModule(*module_name,
        (locals.count - old_locals_count) + (globals.count - old_globals_count));

    for (int i = old_locals_count; i < locals.count; i++)
    {
        imported_module = objectAddAttr(imported_module, locals.names[i], locals.values[i]);

        Object * null_obj = makeNull();
        null_obj->refs++;
        locals.values[i] = null_obj;
        locals.names[i] = "";
    }

    for (int i = old_globals_count; i < globals.count; i++)
    {
        imported_module = objectAddAttr(imported_module, globals.names[i], globals.values[i]);

        Object * null_obj = makeNull();
        null_obj->refs++;
        globals.values[i] = null_obj;
        globals.names[i] = "";
    }

    locals.count = old_locals_count;
    globals.count = old_globals_count;

    free(imported_bytecode);

    current_file = old_file;

    free(import_path_rel);
    free(import_path);

    return imported_module;
}

void quokka_import_standard(char * path)
{
    char * import_path_rel = malloc(strlen(path) + 2 + 1);
    strcpy(import_path_rel, path);

    if (!endswith(path, ".q"))
        strcat(import_path_rel, ".q");

    char * import_path = getrealpath(import_path_rel);

    if (!import_path)
    {
        char * err = malloc(49 + strlen(import_path_rel) + 1 + 1);
        strcpy(err, "(import) file path not found or not accessible: '");
        strcat(err, import_path_rel);
        strcat(err, "'");
        error(err, line_num);
    }

    // Set up the environment for the calls
    char * old_file = current_file;
    current_file = import_path;

    // Compile imported file
    char * imported_bytecode = quokka_compile_fname(import_path);

    // Interpret the imported file
    quokka_interpret(imported_bytecode);

    free(imported_bytecode);

    current_file = old_file;

    free(import_path_rel);
    free(import_path);
}
