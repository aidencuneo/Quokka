#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../include/quokka.h"
#include "../include/compile.h"
#include "../include/interpret.h"

int main(int argc, char ** argv)
{
    if (argc > 2)
    {
        if (strcmp(argv[2], "-v") == 0)
            verbose = 1;
    }

    if (verbose) println("--START--\n");

    if (argc < 2)
    {
        println("Input file path not given, no data to compile.");
        return 1;
    }

    // Full path directing to first file to compile
    char * fullname = getrealpath(argv[1]);
    if (fullname == 0)
    {
        println("Input file path not found or not accessible.");
        return 1;
    }

    // Dirname of first file to compile
    char * dirname = strndup(fullname, strlen(fullname) - strlen(strrchr(fullname, '/')));

    // File name of first file to compile (no dirname)
    char * fname = strrchr(fullname, '/') + 1;

    current_file = fname;

    // Move CWD to dirname
    chdir(dirname);

    char * bytecode = quokka_compile_fname(fname);

    if (compilation_error)
        return 1;

    if (verbose) println("\n--BYTECODE--\n");
    println(bytecode);

    interp_init();
    char * output = quokka_interpret(bytecode);
    if (verbose) println("\n--OUTPUT--");
    println(output);

    if (verbose) println("--SUCCESS--");

    return 0;
}
