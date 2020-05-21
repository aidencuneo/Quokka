#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

// CLI options (bools):
int verbose = 0;
int export_bytecode = 0;
int execute_code = 1;

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
                    char * bytecode = quokka_compile_raw(cli_current_line, -1, 0);
                    quokka_interpret(bytecode);

                    if (stack_size)
                    {
                        // Get top of stack
                        int * arglist = makeIntPtr(stack[stack_size - 1]);

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
                    int * arglist = makeIntPtr(stack[stack_size - 1]);

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
    char * fullname = getrealpath(args[0]);
    if (!fullname)
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

        emptyTrash();
        trash = malloc(sizeof(void *));
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

        free(barefile);
        free(outputfile);
    }

    free(args);

    if (execute_code)
    {
        if (verbose) println("\n--OUTPUT--");

        interp_init();
        quokka_interpret(bytecode);

        freeVars();
        freeMemory();
        free(stack);
        freeRetStack();
        emptyTrash();

        if (verbose) println("--SUCCESS--");
    }

    free(fullname);
    free(dirname);
    free(bytecode);

    return 0;
}
