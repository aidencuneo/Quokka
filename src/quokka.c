#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "../include/q.h"

// Function declarations
void error(char * text, int line);

char ** quokka_line_tok(char * line);

// Bools
int verbose = 0;
int compilation_error = 0;

void error(char * text, int line)
{
    int MAXLINE = 128;

    char * fullfile = readfile(current_file);

    char * lineprevious = malloc(MAXLINE);
    char * linepreview = malloc(MAXLINE);
    char * linenext = malloc(MAXLINE);

    strcpy(lineprevious, "");
    strcpy(linepreview, "");
    strcpy(linenext, "");

    int c = 0;
    for (int i = 0; i < strlen(fullfile) - 1; i++)
    {
        if (fullfile[i] == '\n')
            c++;
        else if (c == line - 1 && strlen(lineprevious) < MAXLINE && fullfile[i] != 10 && fullfile[i] != 13)
            strcat(lineprevious, String(fullfile[i]).value);
        else if (c == line && strlen(linepreview) < MAXLINE && fullfile[i] != 10 && fullfile[i] != 13)
            strcat(linepreview, String(fullfile[i]).value);
        else if (c == line + 1 && strlen(linenext) < MAXLINE && fullfile[i] != 10 && fullfile[i] != 13)
            strcat(linenext, String(fullfile[i]).value);
        if (c > line + 1)
            break;
    }

    line++;

    if (!compilation_error)
        println("\nProgram compilation terminated:\n");

    printf("At %s : Line %d\n\n", current_file, line);

    if (line - 1 > 0)
        printf("  %d | %s\n", line - 1, lineprevious);
    printf("> %d > %s\n", line, linepreview);
    if (line + 1 < charCount(fullfile, '\n'))
        printf("  %d | %s\n", line + 1, linenext);
    print("\n");

    printf("Error: %s\n\n", text);

    free(fullfile);
    free(lineprevious);
    free(linepreview);
    free(linenext);

    compilation_error = 1;
}

char * quokka_compile_line(char ** line, int num, int lineLen, int isInline)
{
    char * bytecode = malloc(2048);

    int len;
    if (lineLen == -1)
        len = arrsize(line);
    else len = lineLen;

    for (int p = 0; p < len; p++)
    {
        print(":P: ");
        println(line[p]);
    }

    bytecode = realloc(bytecode, strlen(bytecode) + 1);
    return bytecode;
}

char * quokka_compile_tokens(char ** tokens, int isInline)
{
    // Keep watch of this in case the files start to get bigger than 8192 characters
    char * compiled = malloc(8192);
    strcpy(compiled, "");

    int size = arrsize(tokens);
    for (int i = 0; i < size; i++)
    {
        if (tokens[i] == NULL)
            continue;

        char ** line = quokka_line_tok(tokens[i]);
        for (int p = 0; line[p] != NULL; p++)
        {
            line[p] = cpstrip(line[p]);
        }

        if (arrsize(line) < 1)
        {
            free(line);
            continue;
        }

        if (verbose) printf("\nSIZE %d\n", arrsize(line));

        strcat(compiled, quokka_compile_line(line, i, -1, isInline));

        if (verbose) printf("-%s-\n", tokens[i]);
        for (int p = 0; line[p] != NULL; p++)
        {
            if (verbose) printf("<%s>\n", line[p]);
        }
    }

    free(tokens);

    return compiled;
}

char * quokka_compile_raw(char * rawtext, int maxtokensize, int isInline)
{
    if (maxtokensize == -1)
        maxtokensize = 2048;
    char ** tokens = malloc(maxtokensize + 1);
    ntokenise(tokens, rawtext, "\n");
    for (int p = 0; p < arrsize(tokens); p++) println(tokens[p]);

    char * res = quokka_compile_tokens(tokens, isInline);

    return res;
}
char * quokka_compile_fname(char * filename)
{
    char * buffer = readfile(filename);

    if (!buffer)
        return 0;

    char * res = quokka_compile_raw(buffer, -1, 0);

    return res;
}

char ** quokka_line_tok(char * line)
{
    line = cpstrip(line);

    char * tokenstr = malloc(1);
    strcpy(tokenstr, "");

    // Bools
    int sq = 0; // Single-Quote = false
    int dq = 0; // Double-Quote = false
    int bt = 0; // Backtick = false

    // Ints
    int rb = 0; // Regular-bracket = 0
    int sb = 0; // Square-bracket = 0
    int cb = 0; // Curly-bracket = 0

    // Used to identify a `//` comment
    int comment = 0; // 1 if `/` found, else 0    

    // Chars
    char q;
    char p = '\0';
    char t = 0;

    // Strings
    char * separator = "\n";

    // Set initial char type
    if (isalnum(line[0]))
        p = 'A';
    else if (isdigit(line[0]))
        p = 'D';
    else if (ispunct(line[0]))
        p = 'S';
    else if (isspace(line[0]))
        p = 'W';

    int len = strlen(line);
    for (int i = 0; i < len; i++)
    {
        q = p;
        char c = line[i];
        if (isalpha(c))
            p = 'A'; // Alphabet
        else if (isdigit(c))
            p = 'D'; // Digit
        else if (ispunct(c))
            p = 'S'; // Symbol
        else if (isspace(c))
            p = 'W'; // Whitespace
        if (((q != p && p != 'W') || p == 'S') && !(
            sq || dq || bt || rb > 0 || sb > 0 || cb > 0
        ) && !(
            (t == '-' || t == '+') && p == 'D' // Join pluses/minuses with digits, -10, +5
        ) && !(
            t == '-' && c == '-' // Join minuses together --, ---, etc
        ) && !(
            t == '+' && c == '+' // Join pluses together ++, +++, etc
        ) && !(
            (t == '-' || t == '+' || t == '*' || t == '/') && c == '=' // Join together operators: -= += *= /=
        ) && !(
            q == 'A' && c == '.' // Join together names like `word.upper` (second part is below)
        ) && !(
            p == 'A' && t == '.' // Second part to the line above.
        ) && !(
            t == '_' && (p == 'A' || c == '.') // Join together names like `string_one` (second part is below)
        ) && !(
            (q == 'A' || t == '.') && c == '_' // Second part to the line above.
        ) && !(
            t == '_' && c == '_' // Join together double underscores `__`
        ) && !(
            q == 'A' && p == 'D' // Join alphabetical and numerical characters.
        ) && !(
            p == 'A' && q == 'D' // Second part to the line above.
        ) && !(
            t == '.' && c == '.' // Join together all `.` tokens.
        ))
        {
            tokenstr = realloc(tokenstr, strlen(tokenstr) + strlen(separator) + 1);
            strncat(tokenstr, separator, strlen(separator));
        }
        if (c == '\'' && !(
            dq || rb > 0 || sb > 0 || cb > 0))
            sq = !sq;
        else if (c == '"' && !(
            sq || bt || rb > 0 || sb > 0 || cb > 0))
            dq = !dq;
        else if (c == '`' && !(
            sq || dq || rb > 0 || sb > 0 || cb > 0))
            bt = !bt;
        else if (c == '(' && !(
            sq || dq || bt || sb > 0 || cb > 0))
            rb += 1;
        else if (c == ')' && !(
            sq || dq || bt || sb > 0 || cb > 0))
            rb -= 1;
        else if (c == '/' && !(
            sq || dq || bt))
        {
            comment++;
            if (comment >= 2)
            {
                tokenstr[strlen(tokenstr) - 2] = '\0';
                break;
            }
        }
        if (comment <= 1)
        {
            if (comment && c != '/')
                comment = 0;
            tokenstr = realloc(tokenstr, strlen(tokenstr) + 2);
            strncat(tokenstr, &c, 1);
        }
        t = c;
    }

    char ** output = (char **)malloc(1024 * sizeof(char *));
    output[0] = "\0";
    tokenise(output, tokenstr, separator);
    for (int p = 0; p < arrsize(output); p++)
        output[p] = cpstrip(output[p]);

    return output;
}

int main(int argc, char ** argv)
{
    if (argc > 2)
    {
        if (strcmp(argv[2], "-v") == 0)
            verbose = 1;
    }

    if (verbose) println("--START--");

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

    println(bytecode);

    if (verbose) println("--STOP--");

    return 0;
}
