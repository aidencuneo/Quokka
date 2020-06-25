#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <signal.h>
#include <inttypes.h>
#include <errno.h>

// CLI options (bools):
int verbose = 0;
int export_bytecode = 0;
int execute_code = 1;

char * full_file_name;
char * full_dir_name;
char * main_bytecode;
int line_num;

#include "../include/quokka.h"
#include "../include/signalhandlers.h"
#include "../include/compile.h"
#include "../include/builtins.h"
#include "../include/interpret.h"

int main(int argc, char ** argv)
{
    // NOTES
    // continue keyword will be rep
    // break keyword will be cut

    // Assign signal handlers
    signal(1, sighupHandler);
    signal(2, sigintHandler);
    signal(3, sigquitHandler);
    signal(4, sigillHandler);
    signal(5, sigtrapHandler);
    // signal(6, sigabrtHandler);
    signal(7, sigbusHandler);
    signal(8, sigfpeHandler);
    // signal(11, sigsegvHandler);
    signal(13, sigpipeHandler);
    signal(15, sigtermHandler);
    signal(16, sigstkfltHandler);
    signal(20, sigtstpHandler);
    signal(24, sigxcpuHandler);
    signal(25, sigxfszHandler);
    signal(30, sigpwrHandler);
    signal(31, sigsysHandler);

    if (sizeof(int) == 2)
        SHIFT = 16;

    char ** args = malloc(argc * sizeof(char *));
    int newargc = 0;

    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--version"))
        {
            printf("Quokka %s\n", VERSION);
            return 0;
        }
        if (!strcmp(argv[i], "-V") || !strcmp(argv[i], "--verbose"))
            verbose = 1;
        else if (!strcmp(argv[i], "-c") || !strcmp(argv[i], "--compile"))
            export_bytecode = 1;
        else if (!strcmp(argv[i], "-C") || !strcmp(argv[i], "--compile-only"))
        {
            export_bytecode = 1;
            execute_code = 0;
        }
        else
        {
            args[newargc] = argv[i];
            newargc++;
        }
    }

    if (verbose) println("--START--\n");

    if (argc < 2 || !newargc)
    {
        free(args);
        quokka_run_cli_interpreter();
        return 0;
    }

    // Full path directing to first file to compile
    full_file_name = getrealpath(args[0]);
    if (!full_file_name)
    {
        println("Input file path not found or not accessible.");

        free(args);
        return 1;
    }

    // Dirname of first file to compile
    full_dir_name = strndup(full_file_name, strlen(full_file_name) - strlen(strrchr(full_file_name, '/')));

    // File name of first file to compile (no full_dir_name)
    char * fname = strrchr(full_file_name, '/') + 1;

    current_file = fname;

    chdir(full_dir_name);

    // Free CLI args
    free(args);

    // If an already compiled .qc file is entered as the first argument,
    // then just retrieve the bytecode and interpret it
    if (endswith(fname, ".qc"))
        main_bytecode = readfile(fname);
    else
    {
        resetTrash();

        // Compile Quokka script into Quokka bytecode
        main_bytecode = quokka_compile_fname(fname);

        emptyTrash();
    }

    if (verbose) println("\n--BYTECODE--\n");
    if (verbose) println(main_bytecode);

    if (export_bytecode)
    {
        char * barefile = strndup(full_file_name, strlen(full_file_name) - strlen(strrchr(full_file_name, '.')));
        char * outputfile = malloc(strlen(barefile) + 3 + 1);

        strcpy(outputfile, barefile);
        strcat(outputfile, ".qc");

        FILE * fp = fopen(outputfile, "wb");
        if (export_bytecode)
            fprintf(fp, "%s", main_bytecode);
        fclose(fp);

        free(barefile);
        free(outputfile);
    }

    if (execute_code && main_bytecode)
    {
        if (verbose) println("\n--OUTPUT--");

        resetTrash();

        interp_init();
        _quokka_interpret(main_bytecode);

        freeStack();
        freeRetStack();
        freeVars();
        freeIntConsts();
        freeConsts();

        emptyTrash();

        if (verbose) println("--SUCCESS--");
    }

    // Free scope stack (from compilation)
    free(scpstk);
    free(scps);
    free(scplines);

    free(full_file_name);
    free(full_dir_name);
    free(main_bytecode);

    return 0;
}
