#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// CLI options (bools):
int verbose = 0;
int export_bytecode = 0;

int line_num;

#include "../include/quokka.h"
#include "../include/compile.h"
#include "../include/functions.h"
#include "../include/interpret.h"

int main(int argc, char ** argv)
{
    char ** args = malloc(argc * sizeof(char *));
    int newargc = 0;

    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-V") || !strcmp(argv[i], "--verbose"))
            verbose = 1;
        else if (!strcmp(argv[i], "-c") || !strcmp(argv[i], "--compile"))
            export_bytecode = 1;
        else
        {
            args[newargc] = argv[i];
            newargc++;
        }
    }

    if (verbose) println("--START--\n");

    if (argc < 2)
    {
        println("Input file path not given, no data to compile.");

        free(args);
        return 1;
    }

    // Full path directing to first file to compile
    char * fullname = getrealpath(argv[1]);
    if (fullname == 0)
    {
        println("Input file path not found or not accessible.");

        free(args);
        return 1;
    }

    // Dirname of first file to compile
    char * dirname = strndup(fullname, strlen(fullname) - strlen(strrchr(fullname, '/')));

    // File name of first file to compile (no dirname)
    char * fname = strrchr(fullname, '/') + 1;

    current_file = fname;

    chdir(dirname);

    char * bytecode;

    // If an already compiled .qc file is entered as the first argument,
    // then just retrieve the bytecode and interpret it
    if (endswith(fname, ".qc"))
        bytecode = readfile(fname);
    else
    {
        compile_init();
        bytecode = quokka_compile_fname(fname);
    }

    if (verbose) println("\n--BYTECODE--\n");
    if (verbose) println(bytecode);

    if (export_bytecode)
    {
        char * barefile = strndup(fullname, strlen(fullname) - strlen(strrchr(fullname, '.')));
        char * outputfile = malloc(strlen(barefile) + 3 + 1);

        strcpy(outputfile, barefile);
        strcat(outputfile, ".qc");

        FILE * fp = fopen(outputfile, "wb");
        if (export_bytecode)
            fprintf(fp, "%s", bytecode);
        fclose(fp);

        free(outputfile);

        return 0;
    }

    if (verbose) println("\n--OUTPUT--");
    interp_init();
    quokka_interpret(bytecode);

    if (verbose) println("--SUCCESS--");

    return 0;
}
