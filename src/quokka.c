#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// CLI options (bools):
int verbose = 0;
int export_neat_bytecode = 0;
int export_compact_bytecode = 0;

int line_num;

#include "../include/quokka.h"
#include "../include/compile.h"
#include "../include/functions.h"
#include "../include/interpret.h"
#include "../include/bytecode.h"

int main(int argc, char ** argv)
{
    char ** args = malloc(argc * sizeof(char *));
    int newargc = 0;

    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-V") || !strcmp(argv[i], "--verbose"))
            verbose = 1;
        else if (!strcmp(argv[i], "-C") || !strcmp(argv[i], "--compile-neat"))
            export_neat_bytecode = 1;
        else if (!strcmp(argv[i], "-c") || !strcmp(argv[i], "--compile-raw"))
            export_compact_bytecode = 1;
        else
        {
            args[newargc] = argv[i];
            newargc++;
        }
    }

    for (int i = 0; i < newargc; i++)
        println(args[i]);

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

    chdir(dirname);

    char * bytecode = quokka_compile_fname(fname);

    if (compilation_error)
        return 1;

    if (verbose) println("\n--BYTECODE--\n");
    if (verbose) println(bytecode);

    if (export_neat_bytecode || export_compact_bytecode)
    {
        char * barefile = strndup(fullname, strlen(fullname) - strlen(strrchr(fullname, '.')));
        char * outputfile = malloc(strlen(barefile) + 3 + 1);

        strcpy(outputfile, barefile);
        strcat(outputfile, ".qc");

        FILE * fp = fopen(outputfile, "wb");
        if (export_compact_bytecode)
            fprintf(fp, "%s", compactBytecode(bytecode));
        else if (export_neat_bytecode)
            fprintf(fp, "%s", bytecode);
        fclose(fp);
    }

    if (verbose) println("\n--OUTPUT--");
    interp_init();
    char * output = quokka_interpret(bytecode);
    if (strlen(output))
        println(output);

    if (verbose) println("--SUCCESS--");

    return 0;
}
