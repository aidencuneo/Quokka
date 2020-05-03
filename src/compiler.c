#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../include/t.h"
#include "../include/scopemanage.h"
#include "../include/funcmanage.h" // Unfinished

#define C_HEADERS "\
#include \"../include/t.h\"\n\
\n\
int _FORVARNAME;\n"

scopeManagement declared;
scopeManagement defined;
scopeManagement scpfuncs;
scopeManagement scpcnstrct;

// Function declarations
void error(char * text, int line);
void arrlstrip(char * line[]);
int stringHasChar(char * s, char c);
int stringIsInt(char * s);
int stringInList(char * arr[], char * key);

char * compileline(char * line[], int num, int lineLen, int isInline);
char * compile_tokens(char ** tokens, int isInline);
char * compile_raw(char * rawtext, int maxtokensize, int isInline);
char * quokka_compile(char * filename);

char ** quokka_line_tok(char * line);

// Bools
int verbose = 0;
int compilation_error = 0;

// Ints
int scope = 0;

// Strings
char * file_declarations;
char * object_def;
char * class_constructor;
char * class_declarations;
char * cur_class_def;

// String arrays
char * types[512];
char * c_builtins[] = {
    /*
        types
    */
    "int",
    "char",
    "double",
    "float",
    "short",
    "long",
    "signed",
    "unsigned",

    /*
        duplicates
    */
    "size_t",
    "NULL",

    /*
        stdio.h
    */
    "FILE",
    "fpos_t",
    "_IOFBF",
    "_IOLBF",
    "_IONBF",
    "BUFSIZ",
    "EOF",
    "FOPEN_MAX",
    "FILENAME_MAX",
    "L_tmpnam",
    "SEEK_CUR",
    "SEEK_END",
    "SEEK_SET",
    "TMP_MAX",
    "stderr",
    "stdin",
    "stdout",

    "fclose",
    "clearerr",
    "feof",
    "ferror",
    "fflush",
    "fgetpos",
    "fopen",
    "fread",
    "freopen",
    "fseek",
    "fsetpos",
    "ftell",
    "fwrite",
    "remove",
    "rename",
    "rewind",
    "setbuf",
    "setvbuf",
    "tmpfile",
    "tmpnam",
    "fprintf",
    "printf",
    "sprintf",
    "vfprintf",
    "vprintf",
    "fscanf",
    "scanf",
    "sscanf",
    "fgetc",
    "fgets",
    "fputc",
    "fputs",
    "getc",
    "getchar",
    "gets",
    "putc",
    "putchar",
    "puts",
    "ungetc",
    "perror",

    /*
        stdlib.h
    */
    "wchar_t",
    "div_t",
    "ldiv_t",

    "EXIT_FAILURE",
    "EXIT_SUCCESS",
    "RAND_MAX",
    "MB_CUR_MAX",

    "atof",
    "atoi",
    "atol",
    "strtod",
    "strtol",
    "strtoul",
    "calloc",
    "free",
    "malloc",
    "realloc",
    "abort",
    "atexit",
    "exit",
    "getenv",
    "system",
    "bsearch",
    "qsort",
    "abs",
    "div",
    "labs",
    "ldiv",
    "rand",
    "srand",
    "mblen",
    "mbstowcs",
    "mbtowc",
    "wcstombs",
    "wctomb",

    /*
        string.h
    */
    "memchr",
    "memcmp",
    "memcpy",
    "memmove",
    "memset",
    "strcat",
    "strncat",
    "strchr",
    "strcmp",
    "strncmp",
    "strcoll",
    "strcpy",
    "strncpy",
    "strcspn",
    "strerror",
    "strlen",
    "strpbrk",
    "strrchr",
    "strspn",
    "strstr",
    "strtok",
    "strxfrm",

    /*
        ctype.h
    */
    "isalnum",
    "isalpha",
    "iscntrl",
    "isdigit",
    "isgraph",
    "islower",
    "isprint",
    "ispunct",
    "isspace",
    "isupper",
    "isxdigit",

    "tolower",
    "toupper",
};

void error(char * text, int line)
{
    int MAXLINE = 128;

    char * fullfile = readfile(current_file);

    char * lineprevious = (char *)malloc(MAXLINE);
    char * linepreview = (char *)malloc(MAXLINE);
    char * linenext = (char *)malloc(MAXLINE);

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
    if (strlen(linenext))
        printf("  %d | %s\n", line + 1, linenext);
    print("\n");

    printf("Error: %s\n\n", text);

    free(fullfile);
    free(lineprevious);
    free(linepreview);
    free(linenext);

    compilation_error = 1;
}

void arrlstrip(char * line[])
{
    int position, c, n;
    position = 0;
    n = arrsize(line);
    for (c = position - 1; c < n - 1; c++)
        line[c] = line[c + 1];
    line[n - 1] = "\0";
}

void arrdel(char * line[], int pos)
{
    int c, n;
    n = arrsize(line);
    for (c = pos - 1; c < n - 1; c++)
        line[c] = line[c + 1];
    //line[n - 1] = "\0";
}

int stringHasChar(char * s, char c)
{
    for (int i = 0; i < strlen(s); ++i)
    {
        if (s[i] == c)
            return 1;
    }
    return 0;
}

int stringIsInt(char * s)
{
    if (!strlen(s))
        return 0;

    for (int i = 0; i < strlen(s); ++i)
    {
        if (s[i] != '0' &&
            s[i] != '1' &&
            s[i] != '2' &&
            s[i] != '3' &&
            s[i] != '4' &&
            s[i] != '5' &&
            s[i] != '6' &&
            s[i] != '7' &&
            s[i] != '8' &&
            s[i] != '9')
            return 0;
    }

    return 1;
}

int cpisidentifier(char * s)
{
    if (!strlen(s))
        return 0;

    for (int i = 0; i < strlen(s); ++i)
    {
        if (!isalnum(s[i]) && s[i] != '_')
            return 0;
    }

    return 1;
}

int stringInList(char * arr[], char * key)
{
    for (int n = 0; n < arrsize(arr); ++n)
    {
        if (strcmp(arr[n], key) == 0)
            return 1;
    }
    return 0;
}

char * compile_raw_line(char * linestr, int num, int isInline)
{
    char ** line = quokka_line_tok(linestr);
    if (verbose) println("--RAW LINE--");
    if (verbose)
        for (int p = 0; p < arrsize(line); p++)
            println(line[p]);
    if (verbose) println("--RAW LINE END--");

    char * temp = compileline(line, num, -1, isInline);

    free(line);

    return temp;
}

char * precompile_stage_1(char ** origline, char * separator, int num, int isInline)
{
    char * linestr1 = (char *)malloc(1);
    strcpy(linestr1, "");

    int len1 = arrsize(origline);

    for (int p = 0; p < len1 && origline[p] != NULL; p++)
    {
        if (verbose) print(":: ");
        if (verbose) println(origline[p]);

        if (!strlen(origline[p]))
            break;

        if (p > 0 && startswith(origline[p], "."))
        {
            linestr1[strlen(linestr1) - 1] = '\0';
            linestr1 = (char *)realloc(linestr1, strlen(linestr1) + strlen(origline[p]) + 1);
            strncat(linestr1, origline[p], strlen(origline[p]));

            linestr1 = (char *)realloc(linestr1, strlen(linestr1) + strlen(separator) + 1);
            strncat(linestr1, separator, strlen(separator));
        }
        else if (
            (startswith(origline[p], "(") && endswith(origline[p], ")")) && strlen(origline[p]) > 2 && ((!(
                (p > 0 && stringInList(types, origline[p - 1])) ||
                (p > 1 && strcmp(origline[p - 2], "new") == 0) ||
                (strcmp(origline[0], "def") == 0) ||
                (p > 0 && cpisidentifier(origline[p - 1]))
            )) )
        )
        {
            if (verbose) println("!!BRACKETS");
            if (verbose) println(origline[p]);
            char * temp = __slice_string__(String(origline[p]), 1, 1).value;
            if (verbose) println(temp);
            char * tempcomp = compile_raw_line(temp, num, 1);
            if (verbose) println(tempcomp);
            if (verbose) println("??BRACKETS");

            linestr1 = (char *)realloc(linestr1, strlen(linestr1) + 1 + strlen(tempcomp) + 1 + 1);
            strncat(linestr1, "(", 2);
            strncat(linestr1, tempcomp, strlen(tempcomp));
            strncat(linestr1, ")", 2);

            linestr1 = (char *)realloc(linestr1, strlen(linestr1) + strlen(separator) + 1);
            strncat(linestr1, separator, strlen(separator));
        }
        else if (startswith(origline[p], "'") && endswith(origline[p], "'"))
        {
            char * temp = (char *)malloc(strlen(origline[p]) + 24 + 1 + 1);
            strcpy(temp, "__string_Constructor__(\"");
            string sliced = __slice_string__(String(origline[p]), 1, 1);
            string escaped = __replace_string__(sliced, String("\""), String("\\\""));
            strcat(temp, escaped.value);
            strcat(temp, "\")");

            linestr1 = (char *)realloc(linestr1, strlen(linestr1) + strlen(temp) + 1);
            strncat(linestr1, temp, strlen(temp));

            free(temp);

            linestr1 = (char *)realloc(linestr1, strlen(linestr1) + strlen(separator) + 1);
            strncat(linestr1, separator, strlen(separator));
        }
        else if (startswith(origline[p], "\"") && endswith(origline[p], "\""))
        {
            char * temp = (char *)malloc(strlen(origline[p]) + 23 + 1 + 1);
            strcpy(temp, "__string_Constructor__(");
            strcat(temp, origline[p]);
            strcat(temp, ")");

            linestr1 = (char *)realloc(linestr1, strlen(linestr1) + strlen(temp) + 1);
            strncat(linestr1, temp, strlen(temp));

            free(temp);

            linestr1 = (char *)realloc(linestr1, strlen(linestr1) + strlen(separator) + 1);
            strncat(linestr1, separator, strlen(separator));
        }
        else if (stringIsInt(origline[p]))
        {
            linestr1 = (char *)realloc(linestr1, strlen(linestr1) + 24 + strlen(origline[p]) + 2 + 1);
            strncat(linestr1, "__integer_Constructor__(", 25);
            strncat(linestr1, origline[p], strlen(origline[p]));
            strncat(linestr1, ")", 2);

            linestr1 = (char *)realloc(linestr1, strlen(linestr1) + strlen(separator) + 1);
            strncat(linestr1, separator, strlen(separator));
        }
        else if (strcmp(origline[p], "true") == 0)
        {
            linestr1 = (char *)realloc(linestr1, strlen(linestr1) + 23 + 1);
            strncat(linestr1, "__bool_Constructor__(1)", 24);

            linestr1 = (char *)realloc(linestr1, strlen(linestr1) + strlen(separator) + 1);
            strncat(linestr1, separator, strlen(separator));
        }
        else if (strcmp(origline[p], "false") == 0)
        {
            linestr1 = (char *)realloc(linestr1, strlen(linestr1) + 23 + 1);
            strncat(linestr1, "__bool_Constructor__(0)", 24);

            linestr1 = (char *)realloc(linestr1, strlen(linestr1) + strlen(separator) + 1);
            strncat(linestr1, separator, strlen(separator));
        }
        else if (strcmp(origline[p], "and") == 0)
        {
            linestr1 = (char *)realloc(linestr1, strlen(linestr1) + 5);
            strncat(linestr1, " && ", 5);

            linestr1 = (char *)realloc(linestr1, strlen(linestr1) + strlen(separator) + 1);
            strncat(linestr1, separator, strlen(separator));
        }
        else if (strcmp(origline[p], "or") == 0)
        {
            linestr1 = (char *)realloc(linestr1, strlen(linestr1) + 5);
            strncat(linestr1, " || ", 5);

            linestr1 = (char *)realloc(linestr1, strlen(linestr1) + strlen(separator) + 1);
            strncat(linestr1, separator, strlen(separator));
        }
        else if (strcmp(origline[p], "not") == 0)
        {
            linestr1 = (char *)realloc(linestr1, strlen(linestr1) + 3);
            strncat(linestr1, " !", 3);

            linestr1 = (char *)realloc(linestr1, strlen(linestr1) + strlen(separator) + 1);
            strncat(linestr1, separator, strlen(separator));
        }
        else if (strcmp(origline[p], "...") == 0)
        {
            linestr1 = (char *)realloc(linestr1, strlen(linestr1) + 29);
            strncat(linestr1, " __integer_Constructor__(0) ", 29);

            linestr1 = (char *)realloc(linestr1, strlen(linestr1) + strlen(separator) + 1);
            strncat(linestr1, separator, strlen(separator));
        }
        else if (startswith(origline[p], "_q_") || stringInList(c_builtins, origline[p]))
        {
            linestr1 = (char *)realloc(linestr1, strlen(linestr1) + 3 + strlen(origline[p]) + 1);

            if (startswith(origline[p], "_q_"))
                strncat(linestr1, "_Q_", 4);
            else if (startswith(origline[p], "_q_") && !startswith(origline[p], "_q__Q_"))
                strncat(linestr1, "_q_", 4);
            else
                strncat(linestr1, "_Q_", 4);

            strncat(linestr1, origline[p], strlen(origline[p]));

            linestr1 = (char *)realloc(linestr1, strlen(linestr1) + strlen(separator) + 1);
            strncat(linestr1, separator, strlen(separator));
        }
        else
        {
            linestr1 = (char *)realloc(linestr1, strlen(linestr1) + strlen(origline[p]) + 1);
            strncat(linestr1, origline[p], strlen(origline[p]));

            linestr1 = (char *)realloc(linestr1, strlen(linestr1) + strlen(separator) + 1);
            strncat(linestr1, separator, strlen(separator));
        }
    }

    return linestr1;
}

char * precompile_stage_2(char ** line1, char * separator, int num, int isInline)
{
    char * linestr2 = (char *)malloc(1);
    strcpy(linestr2, "");

    int len2 = arrsize(line1);

    for (int p = 0; p < len2 && line1[p] != NULL; p++)
    {
        if (verbose) print("2:: ");
        if (verbose) println(line1[p]);

        if (!strlen(line1[p]))
            break;

        if (strcmp(line1[p], "+") == 0)
        {
            if (!(p > 0))
                error("unary `+` not supported", num);
            if (!(p < len2 - 1))
                error("`+` operator missing second argument", num);
            linestr2[strlen(linestr2) - 1] = '\0';

            linestr2 = (char *)realloc(linestr2, strlen(linestr2) + 9);
            strncat(linestr2, ".__add__", 9);

            linestr2 = (char *)realloc(linestr2, strlen(linestr2) + strlen(separator) + 1);
            strncat(linestr2, separator, strlen(separator));

            linestr2 = (char *)realloc(linestr2, strlen(linestr2) + 1 + strlen(line1[p + 1]) + 2 + 1);
            strncat(linestr2, "(", 2);
            strncat(linestr2, line1[p + 1], strlen(line1[p + 1]));
            strncat(linestr2, ")", 2);

            arrdel(line1, p + 1);
            len2--;

            linestr2 = (char *)realloc(linestr2, strlen(linestr2) + strlen(separator) + 1);
            strncat(linestr2, separator, strlen(separator));
        }
        else if (strcmp(line1[p], "-") == 0)
        {
            if (!(p > 0))
                error("unary `-` not supported", num);
            if (!(p < len2 - 1))
                error("`-` operator missing second argument", num);
            linestr2[strlen(linestr2) - 1] = '\0';

            linestr2 = (char *)realloc(linestr2, strlen(linestr2) + 9);
            strncat(linestr2, ".__sub__", 9);

            linestr2 = (char *)realloc(linestr2, strlen(linestr2) + strlen(separator) + 1);
            strncat(linestr2, separator, strlen(separator));

            linestr2 = (char *)realloc(linestr2, strlen(linestr2) + 1 + strlen(line1[p + 1]) + 2 + 1);
            strncat(linestr2, "(", 2);
            strncat(linestr2, line1[p + 1], strlen(line1[p + 1]));
            strncat(linestr2, ")", 2);

            arrdel(line1, p + 1);
            len2--;

            linestr2 = (char *)realloc(linestr2, strlen(linestr2) + strlen(separator) + 1);
            strncat(linestr2, separator, strlen(separator));
        }
        else if (strcmp(line1[p], "*") == 0)
        {
            if (!(p > 0))
                error("`*` operator missing first argument", num);
            if (!(p < len2 - 1))
                error("`*` operator missing second argument", num);
            linestr2[strlen(linestr2) - 1] = '\0';

            linestr2 = (char *)realloc(linestr2, strlen(linestr2) + 9);
            strncat(linestr2, ".__mul__", 9);

            linestr2 = (char *)realloc(linestr2, strlen(linestr2) + strlen(separator) + 1);
            strncat(linestr2, separator, strlen(separator));

            linestr2 = (char *)realloc(linestr2, strlen(linestr2) + 1 + strlen(line1[p + 1]) + 2 + 1);
            strncat(linestr2, "(", 2);
            strncat(linestr2, line1[p + 1], strlen(line1[p + 1]));
            strncat(linestr2, ")", 2);

            arrdel(line1, p + 1);
            len2--;

            linestr2 = (char *)realloc(linestr2, strlen(linestr2) + strlen(separator) + 1);
            strncat(linestr2, separator, strlen(separator));
        }
        else if (strcmp(line1[p], "/") == 0)
        {
            if (!(p > 0))
                error("`/` operator missing first argument", num);
            if (!(p < len2 - 1))
                error("`/` operator missing second argument", num);
            linestr2[strlen(linestr2) - 1] = '\0';

            linestr2 = (char *)realloc(linestr2, strlen(linestr2) + 9);
            strncat(linestr2, ".__div__", 9);

            linestr2 = (char *)realloc(linestr2, strlen(linestr2) + strlen(separator) + 1);
            strncat(linestr2, separator, strlen(separator));

            linestr2 = (char *)realloc(linestr2, strlen(linestr2) + 1 + strlen(line1[p + 1]) + 2 + 1);
            strncat(linestr2, "(", 2);
            strncat(linestr2, line1[p + 1], strlen(line1[p + 1]));
            strncat(linestr2, ")", 2);

            arrdel(line1, p + 1);
            len2--;

            linestr2 = (char *)realloc(linestr2, strlen(linestr2) + strlen(separator) + 1);
            strncat(linestr2, separator, strlen(separator));
        }
        else if (strcmp(line1[p], "==") == 0)
        {
            if (!(p > 0))
                error("`==` equality operator missing first argument", num);
            if (!(p < len2 - 1))
                error("`==` equality operator missing second argument", num);
            linestr2[strlen(linestr2) - 1] = '\0';

            linestr2 = (char *)realloc(linestr2, strlen(linestr2) + 9);
            strncat(linestr2, ".__eq__", 9);

            linestr2 = (char *)realloc(linestr2, strlen(linestr2) + strlen(separator) + 1);
            strncat(linestr2, separator, strlen(separator));

            linestr2 = (char *)realloc(linestr2, strlen(linestr2) + 1 + strlen(line1[p + 1]) + 2 + 1);
            strncat(linestr2, "(", 2);
            strncat(linestr2, line1[p + 1], strlen(line1[p + 1]));
            strncat(linestr2, ")", 2);

            arrdel(line1, p + 1);
            len2--;

            linestr2 = (char *)realloc(linestr2, strlen(linestr2) + strlen(separator) + 1);
            strncat(linestr2, separator, strlen(separator));
        }
        else if (stringInList(types, line1[p]) && p + 1 < len2 && startswith(line1[p + 1], "(") && endswith(line1[p + 1], ")") && !(
            p - 1 >= 0 && strcmp(line1[p - 1], "new") == 0
        ))
        {
            linestr2 = (char *)realloc(linestr2, strlen(linestr2) + strlen(line1[p + 1]) + 3 + strlen(line1[p]) + 2 + 1);
            strncat(linestr2, line1[p + 1], strlen(line1[p + 1]));
            strncat(linestr2, ".__", 4);
            strncat(linestr2, line1[p], strlen(line1[p]));
            strncat(linestr2, "__", 3);

            linestr2 = (char *)realloc(linestr2, strlen(linestr2) + strlen(separator) + 1);
            strncat(linestr2, separator, strlen(separator));

            linestr2 = (char *)realloc(linestr2, strlen(linestr2) + 2 + 1);
            strncat(linestr2, "()", 3);

            linestr2 = (char *)realloc(linestr2, strlen(linestr2) + strlen(separator) + 1);
            strncat(linestr2, separator, strlen(separator));
        }
        else
        {
            linestr2 = (char *)realloc(linestr2, strlen(linestr2) + strlen(line1[p]) + 1);
            strncat(linestr2, line1[p], strlen(line1[p]));

            linestr2 = (char *)realloc(linestr2, strlen(linestr2) + strlen(separator) + 1);
            strncat(linestr2, separator, strlen(separator));
        }
    }

    return linestr2;
}

char * compileline(char * origline[], int num, int lineLen, int isInline)
{
    // num is current line number

    most_recent_line = num;

    // MAKE QUOKKA FOLLOW BIDMAS
    // Brackets - Done
    // Indices  - Not done, will probably leave until last
    // Divide   - Not done
    // Multiply - Not done
    // Add      - Not done
    // Subtract - Not done

    // D & M will group
    // A & S will group

    char * separator = "\n";

    //
    /// PRECOMPILE STAGE 1
    //

    if (verbose) println("--PRECOMPILE STAGE 1--");

    char * linestr1 = precompile_stage_1(origline, separator, num, isInline);

    char ** line1 = (char **)malloc(1024 * sizeof(char *)); // Keep watch of this (1024)
    line1[0] = "";

    tokenise(line1, linestr1, separator);

    if (verbose)
        for (int p = 0; p < arrsize(line1); p++)
            println(line1[p]);

    //
    /// PRECOMPILE STAGE 2
    //

    if (verbose) println("--PRECOMPILE STAGE 2--");

    char * linestr2 = precompile_stage_2(line1, separator, num, isInline);

    char ** line = (char **)malloc(1024 * sizeof(char *)); // Keep watch of this (1024)
    line[0] = "";

    tokenise(line, linestr2, separator);

    if (verbose)
        for (int p = 0; p < arrsize(line); p++)
            println(line[p]);

    //
    /// Cleanup
    //

    //free(linestr1);
    //free(line1);
    //free(linestr2);

    //
    /// COMPILE STAGE
    //

    if (verbose) println("--COMPILE STAGE--");

    char * r = malloc(2048); // Keep watch of this (2048) THIS CAN CAUSE ISSUES IF IMPORTED FILES ARE TOO LARGE
    strcpy(r, "");

    int len;
    if (lineLen == -1) len = arrsize(line); // Retrieve length from last precompile stage if length is not given
    else len = lineLen;

    if (len < 1)
    {
        free(line);
        return r;
    }

    if (line[1] == NULL && len <= 1)
        line[1] = "";

    if (scope == -1)
    {
        if (strcmp(line[0], "end") == 0)
        {
            if (len > 1) error("end action received too many arguments", num);
            scope++;
        }
        else if (len > 1)
            error("invalid syntax, headers must be included on separate lines", num);
        else
        {
            char * header_name;
            if (stringHasChar(line[0], '.'))
            {
                header_name = (char *)malloc(strlen(line[0]) + 1);
                strcpy(header_name, line[0]);
            }
            else
            {
                header_name = (char *)malloc(strlen(line[0]) + 3);
                strcpy(header_name, line[0]);
                strcat(header_name, ".q");
            }
            if (strcmp(current_file, header_name) == 0)
            {
                char * err = (char *)malloc(strlen(line[0]) + strlen(header_name) + 92); // 1 + 18 + 72 + 1 (\0 ending)
                strcpy(err, "'");
                strcat(err, line[0]);
                strcat(err, "' (at local path '");
                strcat(err, header_name);
                strcat(err, "') is being imported from within itself, this should not happen");
                error(err, num);
            }
            char * oldfile = current_file;
            current_file = header_name;
            scope = 0;
            char * header_data = quokka_compile(header_name);
            current_file = oldfile;
            scope = -1;
            if (strlen(header_data) <= 0)
            {
                char * err = (char *)malloc(strlen(line[0]) + strlen(header_name) + 92); // 1 + 18 + 72 + 1 (\0 ending)
                strcpy(err, "'");
                strcat(err, line[0]);
                strcat(err, "' (at local path '");
                strcat(err, header_name);
                strcat(err, "') is an invalid header file, make sure the file exists and is not empty");
                error(err, num);
            }
            strcat(r, strndup(header_data, strlen(header_data)));

            free(header_name);
        }

        free(line);
        return r;
    }

    if (strcmp(line[0], "raise") == 0)
    {
        if (len < 2) error("raise action missing arguments", num);
        else
        {
            arrlstrip(line);
            len--;
            strcat(r, "raise(");

            char * temp = compileline(line, num, len, 1);
            strcat(r, temp);

            int MAXLINE = 128;

            char * fullfile = readfile(current_file);

            char * lineprevious = (char *)malloc(MAXLINE);
            char * linepreview = (char *)malloc(MAXLINE);
            char * linenext = (char *)malloc(MAXLINE);

            strcpy(lineprevious, "");
            strcpy(linepreview, "");
            strcpy(linenext, "");

            int c = 0;
            for (int i = 0; i < strlen(fullfile) - 1; i++)
            {
                if (fullfile[i] == '\n')
                    c++;
                else if (c == num - 1 && strlen(lineprevious) < MAXLINE && fullfile[i] != 10 && fullfile[i] != 13)
                    strcat(lineprevious, String(fullfile[i]).value);
                else if (c == num && strlen(linepreview) < MAXLINE && fullfile[i] != 10 && fullfile[i] != 13)
                    strcat(linepreview, String(fullfile[i]).value);
                else if (c == num + 1 && strlen(linenext) < MAXLINE && fullfile[i] != 10 && fullfile[i] != 13)
                    strcat(linenext, String(fullfile[i]).value);
                if (c > num + 1)
                    break;
            }

            free(fullfile);

            strcat(r, ", \"");
            strcat(r, __replace_string__(String(lineprevious), String("\""), String("\\\"")).value);
            strcat(r, "\", \"");
            strcat(r, __replace_string__(String(linepreview), String("\""), String("\\\"")).value);
            strcat(r, "\", \"");
            strcat(r, __replace_string__(String(linenext), String("\""), String("\\\"")).value);
            strcat(r, "\", \"");
            strcat(r, current_file);
            strcat(r, "\", ");
            char * n = malloc(11);
            sprintf(n, "%d", num);
            strcat(r, n);
            strcat(r, ");\n");

            free(n);
            free(temp);
            free(lineprevious);
            free(linepreview);
            free(linenext);
        }
    }
    else if (strcmp(line[0], "print") == 0)
    {
        if (len < 2) strcat(r, "print(\"\");\n");
        else
        {
            arrlstrip(line);
            len--;
            strcat(r, "print(");
            strcat(r, compileline(line, num, len, 1));
            strcat(r, ");\n");
        }
    }
    else if (strcmp(line[0], "println") == 0)
    {
        if (len < 2) strcat(r, "println(\"\\n\");\n");
        else
        {
            arrlstrip(line);
            len--;
            strcat(r, "println(");
            char * temp = compileline(line, num, len, 1);
            strcat(r, temp);
            strcat(r, ");\n");

            free(temp);
        }
    }
    else if (strcmp(line[0], "if") == 0)
    {
        if (isInline) error("if statement must be at the start of a line", num);
        else if (len < 2) error("if statement missing condition", num);

        char ** temp = malloc(3 * sizeof(char *));
        temp[0] = malloc(4 + 1);
        strcpy(temp[0], "bool");

        temp[1] = malloc(2);
        strcpy(temp[1], "(");

        temp[2] = NULL;

        for (int p = 1; p < len; p++)
        {
            temp[1] = realloc(temp[1], strlen(temp[1]) + strlen(line[p]) + 1);
            strcat(temp[1], line[p]);
        }

        temp[1] = realloc(temp[1], strlen(temp[1]) + 2);
        strcat(temp[1], ")");

        strcat(r, "if((");
        char * ctemp = compileline(temp, num, 2, 1);
        strcat(r, ctemp);
        strcat(r, ").value.value){\n");
        scope++;

        free(temp[0]);
        free(temp[1]);
        free(temp);
        free(ctemp);
    }
    else if (strcmp(line[0], "elif") == 0)
    {
        if (isInline) error("elif statement must be at the start of a line", num);
        else if (len < 2) error("elif statement missing condition", num);

        char ** temp = malloc(3 * sizeof(char *));
        temp[0] = malloc(4 + 1);
        strcpy(temp[0], "bool");

        temp[1] = malloc(2);
        strcpy(temp[1], "(");

        temp[2] = NULL;

        for (int p = 1; p < len; p++)
        {
            temp[1] = realloc(temp[1], strlen(temp[1]) + strlen(line[p]) + 1);
            strcat(temp[1], line[p]);
        }

        temp[1] = realloc(temp[1], strlen(temp[1]) + 2);
        strcat(temp[1], ")");

        strcat(r, "else if((");
        char * ctemp = compileline(temp, num, 2, 1);
        strcat(r, ctemp);
        strcat(r, ").value.value){\n");
        scope++;

        free(temp);
        free(ctemp);
    }
    else if (strcmp(line[0], "else") == 0)
    {
        if (isInline) error("else statement must be at the start of a line", num);
        else if (len > 1) error("else statement received too many conditions", num);
        strcat(r, "else{\n");
        scope++;
    }
    else if (strcmp(line[0], "while") == 0)
    {
        if (isInline) error("while loop must be at the start of a line", num);
        else if (len < 2) error("while loop missing condition", num);
        
        char ** temp = malloc(3 * sizeof(char *));
        temp[0] = malloc(4 + 1);
        strcpy(temp[0], "bool");

        temp[2] = NULL;

        temp[1] = malloc(2);
        strcpy(temp[1], "(");

        for (int p = 1; p < len; p++)
        {
            temp[1] = realloc(temp[1], strlen(temp[1]) + strlen(line[p]) + 1);
            strcat(temp[1], line[p]);
        }

        temp[1] = realloc(temp[1], strlen(temp[1]) + 2);
        strcat(temp[1], ")");

        strcat(r, "while((");
        char * ctemp = compileline(temp, num, 2, 1);
        strcat(r, ctemp);
        strcat(r, ").value.value){\n");
        scope++;

        free(temp);
        free(ctemp);
    }
    else if (strcmp(line[0], "for") == 0)
    {
        if (isInline) error("for loop must be at the start of a line", num);
        if (len < 2) error("for loop missing variable type", num);
        if (len < 3) error("for loop missing variable name", num);
        if (len < 4) error("for loop missing variable value", num);
        if (len < 5) error("for loop missing condition", num);
        if (len < 6) error("for loop missing end-of-line action (usually i++ or similar)", num);
        if (len > 6) error("for loop received too many arguments", num);
        strcat(r, "for(");
        strcat(r, line[1]);
        strcat(r, " ");
        strcat(r, line[2]);
        strcat(r, "=");
        strcat(r, line[3]);
        strcat(r, ";");
        strcat(r, line[4]);
        strcat(r, ";");
        strcat(r, line[5]);
        strcat(r, "){\n");

        scope++;
        declared = _sc_add(declared, line[2], scope);
        defined = _sc_add(defined, line[2], scope);
    }
    else if (strcmp(line[0], "foreach") == 0)
    {
        if (isInline) error("foreach loop must be at the start of a line", num);
        if (len < 2) error("foreach loop missing variable type", num);
        if (len < 3) error("foreach loop missing variable name", num);
        if (len < 4) error("foreach loop missing iterable", num);
        if (len > 4) error("foreach loop received too many arguments", num);
        strcat(r, "for(_FORVARNAME;");
        strcat(r, "_FORVARNAME<arrsize(");
        strcat(r, line[3]);
        strcat(r, ");_FORVARNAME++){\n");
        strcat(r, line[1]);
        strcat(r, " ");
        strcat(r, line[2]);
        strcat(r, "=");
        strcat(r, line[3]);
        strcat(r, "[_FORVARNAME];\n");

        declared = _sc_add(declared, line[2], scope);
        defined = _sc_add(defined, line[2], scope);
        scope++;
    }
    else if (strcmp(line[0], "rangefor") == 0)
    {
        char * start;
        char * stop;
        char * step;
        if (isInline) error("rangefor loop must be at the start of a line", num);
        else if (len < 2) error("rangefor loop missing variable name", num);
        else if (len < 3) error("rangefor loop missing range stop number", num);
        else if (len == 3)
        {
            start = "0";
            stop = line[2];
            step = "1";
        }
        else if (len == 4)
        {
            start = line[2];
            stop = line[3];
            step = "1";
        }
        else if (len == 5)
        {
            start = line[2];
            stop = line[3];
            step = line[4];
        }
        else if (len > 5) error("rangefor loop received too many arguments", num);
        strcat(r, "for(int ");
        strcat(r, line[1]);
        strcat(r, "=");
        strcat(r, start);
        strcat(r, ";");
        strcat(r, line[1]);
        strcat(r, "<");
        strcat(r, stop);
        strcat(r, ".value;");
        strcat(r, line[1]);
        strcat(r, "+=");
        strcat(r, step);
        strcat(r, "){\n");

        scope++;
        declared = _sc_add(declared, line[2], scope);
        defined = _sc_add(defined, line[2], scope);
    }
    // else if (startswith(line[1], "[") && endswith(line[1], "]"))
    // {
    //     // This must be remade to use OO indexing `__index__(0)` instead of plain indexing `[0]`
    //     if (len > 3) error("index can not take arguments", num);
    //     strcat(r, line[0]);
    //     strcat(r, "[");
    //     strcat(r, __slice_string__(String(line[1]), 1, 1).value);
    //     strcat(r, "]");
    //     if (stringInList(arrptrs, line[0]))
    //     {
    //         strcat(r, ".value");
    //     }
        
    //     if (!isInline)
    //         strcat(r, ";\n");
    // }
    else if (strcmp(line[len - 1], ":") == 0)
    {
        if (isInline) error("proc `:` action must be at the start of a line", num);
        else if (scope > 0 && !strlen(class_constructor)) error("proc `:` action can not be in a scope greater than 1", num);
        else if (len < 2) error("proc `:` action missing arguments", num);
        else if (len > 2) error("proc `:` action received too many arguments", num);
        else if (len == 2 && strcmp(line[0], "include") != 0)
        {
            strcat(r, "int ");
            strcat(r, line[0]);
            strcat(r, "(");
            if (strcmp(line[0], "main") == 0)
            {
                strcat(r, "int argc,char**argv");
                declared = _sc_add(declared, "argc", scope);
                declared = _sc_add(declared, "argv", scope);
                defined = _sc_add(defined, "argc", scope);
                defined = _sc_add(defined, "argv", scope);
            }
            else
            {
                file_declarations = (char *)realloc(file_declarations,
                    strlen(file_declarations) + 4 + strlen(line[0]) + 4 + 1);
                strcat(file_declarations, "int ");
                strcat(file_declarations, line[0]);
                strcat(file_declarations, "();\n");
            }
            strcat(r, "){\n");
        }

        if (strcmp(line[0], "include") == 0)
            scope = -1;
        else
        {
            scpfuncs = _sc_add(scpfuncs, strndup(line[0], strlen(line[0])), 0);
            scope++;
        }
    }
    else if (strcmp(line[0], "def") == 0)
    {
        if (isInline) error("def action must be at the start of a line", num);
        if (scope == 1 && strlen(object_def))
        {
            if (len < 2) error("def action missing type", num);
            else if (len < 3) error("def action missing name", num);
            else if (len == 3)
            {
                strcat(r, line[1]);
                strcat(r, " (*");
                strcat(r, line[2]);
                strcat(r, ")(");
                strcat(r, object_def);
                strcat(r, " self);\n");
            }
            else if (len == 4)
            {
                strcat(r, line[1]);
                strcat(r, " (*");
                strcat(r, line[2]);
                strcat(r, ")(");
                strcat(r, object_def);
                strcat(r, " self");

                char * args = line[3];
                if (startswith(args, "(") && endswith(args, ")"))
                    args = __slice_string__(String(args), 1, 1).value;

                char ** argslist = malloc(charCount(args, ',') + 1 * sizeof(char *));
                tokenise(argslist, args, ",");
                int size = arrsize(argslist);

                for (int p = 0; p < size; p++)
                {
                    char ** varsg = quokka_line_tok(argslist[p]);
                    int vsize = arrsize(varsg);
                    if (vsize == 0)
                        break;
                    if (vsize != 2)
                        error("invalid syntax in function argument declaration", num);
                    if (!stringInList(types, varsg[0]))
                    {
                        char * err = (char *)malloc(14 + strlen(varsg[0]) + 1 + 1);
                        strcpy(err, "invalid type '");
                        strcat(err, varsg[0]);
                        strcat(err, "'");
                        error(err, num);
                    }

                    declared = _sc_add(declared, varsg[1], scope + 1);
                    defined = _sc_add(defined, varsg[1], scope + 1);

                    strcat(r, ",");
                    strcat(r, argslist[p]);

                    free(varsg);
                }

                free(argslist);

                strcat(r, ");\n");
            }
            else if (len > 4) error("def action received too many arguments", num);
        }
        else
        {
            if (scope > 0 && !strlen(class_constructor))
                error("def action can not be in a scope greater than 1 (excluding class definitions)",
                    num);
            else if (len < 2) error("def action missing type", num);
            else if (len < 3) error("def action missing name", num);
            else if (len == 3)
            {
                if (strlen(class_constructor))
                {
                    class_declarations = (char *)realloc(class_declarations,
                        strlen(class_declarations) + strlen(line[1]) + 3 +
                        strlen(line[2]) + 12 + strlen(class_constructor) + 8 + 1);
                    strcat(class_declarations, line[1]);
                    strcat(class_declarations, " __");
                    strcat(class_declarations, line[2]);
                    strcat(class_declarations, "_");
                    strcat(class_declarations, class_constructor);
                    strcat(class_declarations, "__(");
                    strcat(class_declarations, class_constructor);
                    strcat(class_declarations, " self){\n");

                    cur_class_def = (char *)realloc(cur_class_def,
                        strlen(cur_class_def) + strlen(line[2]) + 1);
                    strcat(cur_class_def, line[2]);
                }
                else
                {
                    strcat(r, line[1]);
                    strcat(r, " ");
                    strcat(r, line[2]);
                    strcat(r, "(){\n");

                    file_declarations = (char *)realloc(file_declarations,
                        strlen(file_declarations) + strlen(line[1]) + 1 + strlen(line[2]) + 4 + 1);
                    strcat(file_declarations, line[1]);
                    strcat(file_declarations, " ");
                    strcat(file_declarations, line[2]);
                    strcat(file_declarations, "();\n");

                    scpfuncs = _sc_add(scpfuncs, strndup(line[2], strlen(line[2])), scope);
                }
            }
            else if (len > 4) error("def action received too many arguments", num);
            // if (len == 4)
            else
            {
                if (strlen(class_constructor))
                {
                    class_declarations = (char *)realloc(class_declarations,
                        strlen(class_declarations) + strlen(line[1]) + 3 + strlen(line[2]) +
                        1 + strlen(class_constructor) + 3 + strlen(class_constructor) + 6 +
                        strlen(line[3]) + 3 + 1);
                    strcat(class_declarations, line[1]);
                    strcat(class_declarations, " __");
                    strcat(class_declarations, line[2]);
                    strcat(class_declarations, "_");
                    strcat(class_declarations, class_constructor);
                    strcat(class_declarations, "__(");
                    strcat(class_declarations, class_constructor);
                    strcat(class_declarations, " self");
                    char * args = line[3];
                    if (startswith(args, "(") && endswith(args, ")"))
                        args = __slice_string__(String(args), 1, 1).value;
                    if (strlen(args))
                    {
                        strcat(class_declarations, ",");
                        strcat(class_declarations, args);
                    }
                    strcat(class_declarations, "){\n");

                    cur_class_def = (char *)realloc(cur_class_def,
                        strlen(line[2]) + 1);
                    strcpy(cur_class_def, line[2]);
                }
                else
                {
                    char * sg;
                    sg = malloc(strlen(line[1]) + 1 + strlen(line[2]) + 1 + 1);
                    strcpy(sg, line[1]);
                    strcat(sg, " ");
                    strcat(sg, line[2]);
                    strcat(sg, "(");

                    char * args = line[3];
                    if (!(startswith(args, "(") && endswith(args, ")")))
                        error("invalid syntax in function argument declaration", num);

                    if (strlen(args) > 2)
                    {
                        args = __slice_string__(String(args), 1, 1).value;

                        char ** argslist = malloc(charCount(args, ',') + 1 * sizeof(char *));
                        tokenise(argslist, args, ",");
                        int size = arrsize(argslist);

                        for (int p = 0; p < size; p++)
                        {
                            char ** varsg = quokka_line_tok(argslist[p]);
                            int vsize = arrsize(varsg);

                            if (vsize == 0)
                                break;
                            if (vsize != 2)
                                error("invalid syntax in function argument declaration", num);
                            if (!stringInList(types, varsg[0]))
                            {
                                char * err = (char *)malloc(14 + strlen(varsg[0]) + 1 + 1);
                                strcpy(err, "invalid type '");
                                strcat(err, varsg[0]);
                                strcat(err, "'");
                                error(err, num);
                            }

                            declared = _sc_add(declared, varsg[1], scope + 1);
                            defined = _sc_add(defined, varsg[1], scope + 1);

                            sg = realloc(sg, strlen(sg) + strlen(argslist[p]) + 1 + 1);
                            strcat(sg, argslist[p]);
                            strcat(sg, ",");

                            free(varsg);
                        }

                        while (endswith(sg, ","))
                            strcpy(sg, __slice_string__(String(sg), 0, 1).value);

                        free(argslist);
                    }

                    sg = realloc(sg, strlen(sg) + 2);
                    strcat(sg, ")");
                    file_declarations = (char *)realloc(file_declarations,
                        strlen(file_declarations) + strlen(sg) + 2 + 1);
                    strcat(file_declarations, sg);
                    strcat(file_declarations, ";\n");

                    sg = realloc(sg, strlen(sg) + 2 + 1);
                    strcat(sg, "{\n");
                    strcpy(r, sg);

                    free(sg);

                    scpfuncs = _sc_add(scpfuncs, strndup(line[2], strlen(line[2])), scope);
                }
            }
            scope++;
        }
    }
    else if (strcmp(line[0], "object") == 0)
    {
        char * name;
        if (strlen(class_constructor)) error("object definition must not be within class constructor", num);
        if (isInline) error("object definition must be at the start of a line", num);
        if (len > 2) error("object definition received too many arguments", num);
        if (len < 2)
        {
            int ext = strlen(strchr(current_file, '.'));
            name = (char *)malloc(strlen(current_file) - ext + 1);
            strcpy(name, __slice_string__(String(current_file), 0, ext).value);
        }
        else
        {
            name = (char *)malloc(strlen(line[1]));
            strcpy(name, line[1]);
        }
        // Define type
        file_declarations = (char *)realloc(
            file_declarations, strlen(file_declarations) + 17 + strlen(name) + 10 + strlen(name) + 2 + 1);
        strcat(file_declarations, "typedef struct __");
        strcat(file_declarations, name);
        strcat(file_declarations, "_Struct__ ");
        strcat(file_declarations, name);
        strcat(file_declarations, ";\n");
        // Define struct
        file_declarations = (char *)realloc(
            file_declarations, strlen(file_declarations) + 9 + strlen(name) + 12 + 1);
        strcat(file_declarations, "struct __");
        strcat(file_declarations, name);
        strcat(file_declarations, "_Struct__ {\n");
        object_def = (char *)realloc(object_def, strlen(object_def) + strlen(name) + 1);
        strcpy(object_def, name);
        types[arrsize(types)] = name;
        scope++;

        free(name);
    }
    else if (strcmp(line[0], "class") == 0)
    {
        char * name;
        char * args = 0;
        if (strlen(class_constructor)) error("class constructor can not be within class constructor", num);
        if (scope > 0) error("class constructor scope must not be higher than minimum scope", num);
        if (isInline) error("class constructor definition must be at the start of a line", num);
        if (len > 3) error("class constructor definition received too many arguments", num);
        // if `class classname(int arg1, int arg2)`
        if (len == 3)
        {
            name = (char *)malloc(strlen(line[1]));
            strcpy(name, line[1]);

            args = line[2];
            if (!(startswith(args, "(") && endswith(args, ")")))
                error("invalid syntax in class constructor", num);
        }
        else if (len == 2)
        {
            // if `class(int arg1, int arg2)`
            if (startswith(line[1], "(") && endswith(line[1], ")"))
            {
                int ext = strlen(strchr(current_file, '.'));
                name = (char *)malloc(strlen(current_file) - ext);
                strcpy(name, __slice_string__(String(current_file), 0, ext).value);

                args = line[1];
            }
            // if `class classname`
            else
            {
                name = (char *)malloc(strlen(line[1]));
                strcpy(name, line[1]);
            }
        }
        else if (len == 1)
        {
            int ext = strlen(strchr(current_file, '.'));
            name = (char *)malloc(strlen(current_file) - ext);
            strcpy(name, __slice_string__(String(current_file), 0, ext).value);
        }

        char * sg;
        sg = malloc(strlen(name) + 3 + strlen(name) + 15 + 1);
        strcpy(sg, name);
        strcat(sg, " __");
        strcat(sg, name);
        strcat(sg, "_Constructor__(");

        if (strlen(args) > 2)
        {
            args = __slice_string__(String(args), 1, 1).value;

            char ** argslist = malloc(charCount(args, ',') + 1 * sizeof(char *));
            tokenise(argslist, args, ",");
            int size = arrsize(argslist);

            for (int p = 0; p < size; p++)
            {
                char ** varsg = quokka_line_tok(argslist[p]);
                int vsize = arrsize(varsg);

                if (vsize == 0)
                    break;
                if (vsize != 2)
                    error("invalid syntax in class constructor's argument declaration", num);
                if (!stringInList(types, varsg[0]))
                {
                    char * err = (char *)malloc(14 + strlen(varsg[0]) + 1 + 1);
                    strcpy(err, "invalid type '");
                    strcat(err, varsg[0]);
                    strcat(err, "'");
                    error(err, num);
                }

                declared = _sc_add(declared, varsg[1], scope + 1);
                defined = _sc_add(defined, varsg[1], scope + 1);

                sg = realloc(sg, strlen(sg) + strlen(argslist[p]) + 1 + 1);
                strcat(sg, argslist[p]);
                strcat(sg, ",");

                free(varsg);
            }

            while (endswith(sg, ","))
                strcpy(sg, __slice_string__(String(sg), 0, 1).value);

            free(argslist);
        }

        sg = realloc(sg, strlen(sg) + 2);
        strcat(sg, ")");
        file_declarations = (char *)realloc(file_declarations,
            strlen(file_declarations) + strlen(sg) + 2 + 1);
        strcat(file_declarations, sg);
        strcat(file_declarations, ";\n");

        sg = realloc(sg, strlen(sg) + 2 + strlen(name) + 7 + 1);
        strcat(sg, "{\n");
        strcat(sg, name);
        strcat(sg, " self;\n");
        strcpy(r, sg);

        free(sg);

        class_constructor = (char *)realloc(class_constructor,
            strlen(class_constructor) + strlen(name) + 1);
        strcpy(class_constructor, name);

        types[arrsize(types)] = strndup(name, strlen(name));

        free(name);

        scope++;
    }
    else if (strcmp(line[0], "new") == 0)
    {
        if (verbose) println("--NEW START--");
        if (verbose)
            for (int p = 0; p < len; p++)
                println(line[p]);
        if (verbose) println("--!NEW END--");
        char * args = 0;
        if (scope < 1) error("new object creation must not be at minimum scope", num);
        else if (len < 2) error("new object creation missing object to instantiate", num);
        else if (len > 3) error("new object creation received too many arguments", num);
        else if (len == 3)
            args = line[2];
        if (!(startswith(args, "(") && endswith(args, ")")))
            error("invalid constructor arguments in new object creation, arguments must have braces", num);
        strcat(r, "__");
        strcat(r, line[1]);
        strcat(r, "_Constructor__(");
        if (args && strlen(args) > 2)
        {
            char * tempstr[512];
            tokenise(tempstr, __slice_string__(String(args), 1, 1).value, ",");
            for (int p = 0; p < arrsize(tempstr); p++)
            {
                tempstr[p] = cpstrip(tempstr[p]);
                strcat(r, compile_raw_line(tempstr[p], num, 1));
                strcat(r, ",");
            }
            while (endswith(r, ","))
                strcpy(r, __slice_string__(String(r), 0, 1).value);
        }
        strcat(r, ")");
        if (!isInline)
            strcat(r, ";\n");
    }
    else if (strcmp(line[0], "scope") == 0)
    {
        if (isInline) error("scope action must be at the start of a line", num);
        else if (scope < 1) error("scope action has no effect at global scope", num);
        strcat(r, "{\n");
        scope++;
    }
    else if (strcmp(line[0], "end") == 0)
    {
        if (isInline) error("end action must be at the start of a line", num);
        else if (scope < 1) error("end action has nothing to end, program is already at minimum scope", num);
        else if (len > 1) error("end action received too many arguments", num);
        else if (strlen(class_constructor) && scope == 1)
        {
            strcat(r, "return self;\n}\n");
            class_constructor = (char *)realloc(class_constructor, 1);
            //strcpy(class_constructor, "");
            class_constructor[0] = '\0';
        }
        else if (strlen(object_def) && scope == 1)
        {
            file_declarations = (char *)realloc(file_declarations,
                strlen(file_declarations) + 3 + 1);
            strcat(file_declarations, "};\n");
            object_def = (char *)realloc(object_def, 1);
            //strcpy(object_def, "");
            object_def[0] = '\0';
        }
        else if (strlen(class_constructor) && strlen(cur_class_def) && scope == 2)
        {
            class_declarations = (char *)realloc(class_declarations,
                strlen(class_declarations) + 2 + 1);
            strcat(class_declarations, "}\n");
            strcat(r, "self.");
            strcat(r, cur_class_def);
            strcat(r, "=__");
            strcat(r, cur_class_def);
            strcat(r, "_");
            strcat(r, class_constructor);
            strcat(r, "__;\n");
            cur_class_def = (char *)realloc(cur_class_def, 1);
            //strcpy(cur_class_def, "");
            cur_class_def[0] = '\0';
        }
        else strcat(r, "}\n");
        declared = _sc_delscope(declared, scope);
        defined = _sc_delscope(defined, scope);
        scope--;
    }
    // else if (strcmp(line[0], "continue") == 0)
    // {
    //     if (isInline) error("continue action must be at the start of a line", num);
    //     if (scope < 2) error("continue action outside of loop", num);
    //     if (len > 1) error("continue action received too many arguments", num);
    //     strcat(r, "continue;\n");
    // }
    // else if (strcmp(line[0], "break") == 0)
    // {
    //     if (isInline) error("break action must be at the start of a line", num);
    //     if (scope < 2) error("break action outside of loop", num);
    //     if (len > 1) error("break action received too many arguments", num);
    //     strcat(r, "break;\n");
    // }
    else if (strcmp(line[0], "ret") == 0)
    {
        if (isInline) error("ret action must be at the start of a line", num);
        else if (scope < 1) error("ret action can not return a value at minimum scope", num);
        else if (len < 2) error("ret action missing argument", num);
        else
        {
            strcat(r, "return ");
            arrlstrip(line);
            len--;
            char * temp = compileline(line, num, len, 1);
            strcat(r, strndup(temp, strlen(temp)));
            strcat(r, ";\n");

            free(temp);
        }
    }
    else if (startswith(line[0], "{") && endswith(line[0], "}"))
    {
        char * tempstr[512];
        tokenise(tempstr, __slice_string__(String(line[0]), 1, 1).value, ",");
        if (verbose) println("START");
        for (int p = 0; p < arrsize(tempstr); p++)
        {
            tempstr[p] = cpstrip(tempstr[p]);
            if (verbose) println(tempstr[p]);
        }
        if (verbose) println("END");
        strcat(r, "{");
        strcat(r, compileline(tempstr, num, arrsize(tempstr), 1));
        strcat(r, "}");
        if (!isInline)
            strcat(r, ";\n");
    }
    else if (stringInList(line, "="))
    {
        if (isInline) error("variables can only be defined at the start of a line", num);
        else if (scope < 1)
            error("variables can not be defined at minimum scope, did you mean to declare a global variable instead?",
                num);
        else if (len < 2) error("invalid syntax in variable definition", num);
        else if (len < 3) error("invalid syntax in variable definition", num);
        strcat(r, line[0]); // Type/Name
        // If second line token is not an equals sign `string word =`, `int num =`
        if (strcmp(line[1], "=") != 0)
        {
            if (!stringInList(types, line[0]))
            {
                char * err = (char *)malloc(14 + strlen(line[0]) + 1 + 1);
                strcpy(err, "invalid type '");
                strcat(err, line[0]);
                strcat(err, "'");
                error(err, num);
            }
            char * varname = line[1];
            if (_sc_exists(declared, varname, scope))
            {
                char * err = malloc(20 + strlen(varname) + 82 + 1);
                strcpy(err, "variable with name '");
                strcat(err, varname);
                strcat(err,
                    "' has already been declared, maybe you forget to remove this line's variable type?");
                error(err, num);
            }
            strcat(r, " ");
            strcat(r, varname); // Var name
            strcat(r, "=");
            len -= 3;
            arrlstrip(line);
            arrlstrip(line);
            arrlstrip(line);

            char * temp = compileline(line, num, len, 1);
            strcat(r, temp); // Var value

            strcat(r, ";\n");
            declared = _sc_add(declared, varname, scope);
            defined = _sc_add(defined, varname, scope);

            free(temp);
        }
        // If second line token is an equals sign `word =`, `num =`
        else
        {
            char * varname = line[0];

            if (!_sc_exists(declared, varname, scope))
            {
                char * err = malloc(20 + strlen(varname) + 66 + 1);
                strcpy(err, "variable with name '");
                strcat(err, varname);
                strcat(err,
                    "' has not been declared, did you forget this line's variable type?");
                error(err, num);
            }

            strcat(r, "=");
            len -= 2;
            arrlstrip(line);
            arrlstrip(line);

            char * temp = compileline(line, num, len, 1);
            strcat(r, temp); // Var value

            strcat(r, ";\n");
            if (!_sc_exists(defined, varname, scope))
            {
                // Get the scope of this variable's declaration
                int declaredscope = _sc_getscope(declared, varname);
                defined = _sc_add(defined, strndup(varname, strlen(varname)), declaredscope);
            }

            free(temp);
        }
    }
    else if (stringInList(types, line[0]))
    {
        if (isInline)
            error("variable declaration can not be placed here (type name found inside inline statement)",
                num);
        else if (len < 2) error("variable declaration missing type", num);
        else if (len > 2) error("invalid syntax in variable declaration (too many arguments)", num);
        strcat(r, line[0]);
        strcat(r, " ");
        strcat(r, line[1]);
        strcat(r, ";\n");
        declared = _sc_add(declared, line[1], scope);
    }
    else if (len > 1 && startswith(line[len - 1], "(") && endswith(line[len - 1], ")"))
    {
        if (!stringHasChar(line[0], '.') && !_sc_exists(scpfuncs, line[0], scope) && !(
            (startswith(line[0], "__") && endswith(line[0], "__"))
        ))
        {
            char * funcname = line[0];
            if (startswith(funcname, "_q_") || startswith(funcname, "_Q_"))
                funcname += 3;
            char * err = malloc(20 + strlen(funcname) + 1 + 1);
            strcpy(err, "undefined function '");
            strcat(err, funcname);
            strcat(err, "'");
            error(err, num);
        }

        char * sg = (char *)malloc(1);
        strcpy(sg, "");

        char * last;

        for (int p = 0; p < len; p++)
        {
            if (stringHasChar(line[p], '.') && p < len - 1)
            {
                int sblen = strlen(strrchr(line[p], '.'));
                char * name = __slice_string__(String(line[p]), 0, sblen).value;

                while (startswith(name, "(") && endswith(name, ")"))
                    strcpy(name, __slice_string__(String(name), 1, 1).value);

                sg = realloc(sg, strlen(sg) + strlen(line[p]) + 1 + strlen(name) + 1 + 1);

                strcat(sg, line[p]);
                strcat(sg, "(");
                strcat(sg, name);
                strcat(sg, ",");
            }
            else
            {
                char * args = line[p];

                if (p == len - 1 && strcmp(line[p], "()"))
                    args = compile_raw_line(args, num, 1);

                if (startswith(args, "(") && endswith(args, ")") && endswith(sg, ","))
                    strcpy(args, __slice_string__(String(args), 1, 1).value);

                sg = realloc(sg, strlen(sg) + strlen(args) + 1);
                strcat(sg, args);
            }
        }

        while (endswith(sg, ","))
            strcpy(sg, __slice_string__(String(sg), 0, 1).value);

        int lbc = charCount(sg, '(');
        int rbc = charCount(sg, ')');

        for (int p = 0; p < (lbc - rbc); p++)
        {
            sg = realloc(sg, strlen(sg) + 2);
            strcat(sg, ")");
        }

        strcat(r, sg);
        free(sg);

        if (!isInline)
            strcat(r, ";");
    }
    else
    {
        for (int p = 0; p < len; p++)
        {
            if (startswith(line[p], "`") && endswith(line[p], "`"))
            {
                strcat(r, __slice_string__(String(line[p]), 1, 1).value);
                continue;
            }

            if (cpisidentifier(line[p]) &&
                _sc_exists(declared, line[p], scope) &&
                !_sc_exists(defined, line[p], scope) &&
                !_sc_exists(scpfuncs, line[p], scope) &&
                !_sc_exists(scpcnstrct, line[p], scope))
            {
                char * ident = line[p];
                if (startswith(ident, "_q_") || startswith(ident, "_Q_"))
                    ident += 3;
                char * err = malloc(22 + strlen(ident) + 36 + 1);
                strcpy(err, "undefined identifier '");
                strcat(err, ident);
                strcat(err, "', did you forget to set it's value?");
                error(err, num);
            }
            else if (cpisidentifier(line[p]) &&
                !_sc_exists(declared, line[p], scope) &&
                !_sc_exists(defined, line[p], scope) &&
                !_sc_exists(scpfuncs, line[p], scope) &&
                !_sc_exists(scpcnstrct, line[p], scope))
            {
                char * ident = line[p];
                if (startswith(ident, "_q_") || startswith(ident, "_Q_"))
                    ident += 3;
                char * err = malloc(23 + strlen(ident) + 1 + 1);
                strcpy(err, "undeclared identifier '");
                strcat(err, ident);
                strcat(err, "'");
                error(err, num);
            }

            strcat(r, line[p]);
            strcat(r, " ");
        }

        while (endswith(r, ","))
            strcpy(r, __slice_string__(String(r), 0, 1).value);

        if (!isInline)
        {
            strcat(r, ";\n");
        }

        strcpy(r, cpstrip(r));
    }

    return r;
}

char * compile_tokens(char ** tokens, int isInline)
{
    // Keep watch of this in case the files start to get bigger than 6000 characters
    char * compiled = (char *)malloc(6000);
    strcpy(compiled, "");

    for (int i = 0; i < arrsize(tokens); i++)
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

        char * result = compileline(line, i, -1, isInline);

        if (strlen(object_def))
        {
            file_declarations = (char *)realloc(file_declarations,
                strlen(file_declarations) + strlen(result) + 1);
            strcat(file_declarations, result);
        }
        else if (strlen(class_constructor) && strlen(cur_class_def))
        {
            class_declarations = (char *)realloc(class_declarations,
                strlen(class_declarations) + strlen(result) + 1);
            strcat(class_declarations, result);
        }
        else strcat(compiled, result);

        if (verbose) printf("-%s-\n", tokens[i]);
        for (int p = 0; line[p] != NULL; p++)
        {
            if (verbose) printf("<%s>\n", line[p]);
        }
    }

    free(tokens);

    return compiled;
}

char * compile_raw(char * rawtext, int maxtokensize, int isInline)
{
    if (maxtokensize == -1)
        maxtokensize = 2048;
    char ** tokens = malloc(maxtokensize + 1);
    ntokenise(tokens, rawtext, "\n");

    char * res = compile_tokens(tokens, isInline);

    return res;
}

char * quokka_compile(char * filename)
{
    char * buffer = readfile(filename);

    if (!buffer)
        return 0;

    char * res = compile_raw(buffer, -1, 0);

    return res;
}

char ** quokka_line_tok(char * line)
{
    line = cpstrip(line);

    char * tokenstr = (char *)malloc(1);
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
            tokenstr = (char *)realloc(tokenstr, strlen(tokenstr) + strlen(separator) + 1);
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
            tokenstr = (char *)realloc(tokenstr, strlen(tokenstr) + 2);
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
    if (argc > 3)
    {
        if (strcmp(argv[3], "-v") == 0)
            verbose = 1;
    }

    if (verbose) println("--START--");

    // Set up a line-by-line interpreter like the Python one possibly?
    if (argc < 2)
    {
        println("Input file path not given, no data to compile.");
        return 1;
    }
    else if (argc < 3)
    {
        println("Output file path not given.");
        return 1;
    }

    // Construct scope managers
    declared = _scopeManager();
    defined = _scopeManager();
    scpfuncs = _scopeManager();
    scpcnstrct = _scopeManager();

    scpfuncs = _sc_add(scpfuncs, "print", 0);
    scpfuncs = _sc_add(scpfuncs, "println", 0);
    scpfuncs = _sc_add(scpfuncs, "raise", 0);

    scpcnstrct = _sc_add(scpcnstrct, "__integer_Constructor__", 0);
    scpcnstrct = _sc_add(scpcnstrct, "__string_Constructor__", 0);
    scpcnstrct = _sc_add(scpcnstrct, "__bool_Constructor__", 0);

    // Set built-in types
    types[arrsize(types)] = "integer";
    types[arrsize(types)] = "string";
    types[arrsize(types)] = "bool";

    // Initialise file_declarations
    file_declarations = (char *)malloc(1);
    strcpy(file_declarations, "");

    // Initialise object_def
    object_def = (char *)malloc(1);
    strcpy(object_def, "");

    // Initialise class_constructor
    class_constructor = (char *)malloc(1);
    strcpy(class_constructor, "");

    // Initialise class_declarations
    class_declarations = (char *)malloc(1);
    strcpy(class_declarations, "");

    // Initialise cur_class_def
    cur_class_def = (char *)malloc(1);
    strcpy(cur_class_def, "");

    // Full path directing to first file to compile
    char * fullname = getrealpath(argv[1]);
    if (!strlen(fullname))
    {
        println("Input file path not found or not accessible.");
        return 1;
    }

    // Full path directing to output file
    if (getrealpath(argv[2]) == 0)
    {
        FILE * fp = fopen(argv[2], "w");
        fclose(fp);
    }
    char * outputpath = getrealpath(argv[2]);

    // Dirname of first file to compile
    char * dirname = strndup(fullname, strlen(fullname) - strlen(strrchr(fullname, '/')));

    // File name of first file to compile (no dirname)
    char * fname = strrchr(fullname, '/') + 1;

    current_file = fname;

    // Move CWD to dirname
    chdir(dirname);

    char * compiled = quokka_compile(fname);

    if (compilation_error)
    {
        println("Compilation failed.\n");
        return 1;
    }

    if (verbose) println("--COMPILED--");

    if (scope != 0)
        error("EOF inside a scope, did you forget to add 'end' somewhere?", charCount(readfile(fname), '\n'));

    FILE * fp = fopen(outputpath, "w");
    fprintf(fp, "%s", C_HEADERS);

    if (!strlen(compiled))
    {
        fprintf(fp, "%s", "int main(){}\n");
        fclose(fp);
        return 0;
    }

    fprintf(fp, "%s", file_declarations);
    fprintf(fp, "%s", class_declarations);
    fprintf(fp, "%s", compiled);
    fclose(fp);

    // Free extra data
    free(outputpath);
    free(dirname);

    free(file_declarations);
    free(object_def);
    free(class_constructor);
    free(class_declarations);
    free(cur_class_def);
    free(compiled);

    // Free scope managers
    _sc_clear(declared);
    _sc_clear(defined);
    _sc_clear(scpfuncs);
    _sc_clear(scpcnstrct);

    if (verbose) println("--END--");

    return 0;
}

/* TODO */

/*

1.  Add +=, -=, *=, and /= operators.

2.  Store variable types within scopeManagers. That way statements can decide
    beforehand whether or not they will immediately fail. It'll help a little
    to reduce the amount of errors that have to get sent to GCC before you get
    told about them, and hopefully increase the amount that Quokka can handle.

3.  Add definitions from t.h into the c_builtins list.
    Obviously don't chuck in definitions that are going to be used in Quokka.

4.  Finish funcmanage.h to keep a record of every function that exists in a
    current Quokka execution session, and keep a record of how many arguments
    each function can or can't take.
    Eventually, improve this to include optional arguments, where any
    arguments that aren't given values through a function call are
    automatically given a null value ( most likely Integer(0) ).

*/
