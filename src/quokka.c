#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <signal.h>
#include <inttypes.h>

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

        print("Quokka ");
        print(VERSION);
        println(":");

        current_file = "[CLI]";
        in_cli_mode = 1;
        cli_current_line = malloc(1);
        strcpy(cli_current_line, "");

        compile_init();
        interp_init(); // Test it without these init lines

        int in_compound_line = 0;
        for (;;)
        {
            if (in_compound_line)
            {
                print("  > ");
                char * line = cpstrip(getinput());

                cli_current_line = realloc(cli_current_line, strlen(cli_current_line) + strlen(line) + 1 + 1);
                strcat(cli_current_line, line);
                strcat(cli_current_line, "\n");

                if (!strcmp(line, "end"))
                {
                    char * bytecode = quokka_compile_raw(cli_current_line, 0);
                    quokka_interpret(bytecode);

                    if (stack_size)
                    {
                        // Get top of stack
                        Object ** arglist = makeArglist(stack[stack_size - 1]);

                        // Print it
                        q_function_println(1, arglist);

                        free(arglist);
                    }

                    free(bytecode);

                    in_compound_line = 0;
                }
            }
            else
            {
                print("--> ");
                char * line = cpstrip(getinput());
                cli_current_line = realloc(cli_current_line, strlen(cli_current_line) + strlen(line) + 1 + 1);
                strcpy(cli_current_line, line);
                strcat(cli_current_line, "\n");

                if (startswith(line, "if ") ||
                    startswith(line, "while "))
                {
                    in_compound_line = 1;
                    continue;
                }

                char * bytecode = quokka_compile_line(line, 0, -1, 0);
                quokka_interpret(bytecode);

                if (stack_size)
                {
                    // Get top of stack
                    Object ** arglist = makeArglist(stack[stack_size - 1]);

                    // Print it
                    q_function_println(1, arglist);

                    free(arglist);
                }

                free(bytecode);
            }
        }

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

    if (execute_code)
    {
        if (verbose) println("\n--OUTPUT--");

        resetTrash();

        interp_init();
        quokka_interpret(main_bytecode);

        // freeMemory();
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
