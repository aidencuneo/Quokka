#include "../headers/t.h"
#include "../headers/compiler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int verbose = 0;

int scope = 0;
char * pointers[512];
char * values[512];
char * arrptrs[512];

void error(char * text, int line)
{
    printf("\nProgram compilation terminated:\nLine %d -> %s\n\n", line + 1, text);
    exit(EXIT_FAILURE);
}

void arrlstrip(char * line[])
{
    int position, c, n;
    position = 0;
    n = arrsize(line);
    for (c = position - 1; c < n - 1; c++)
        line[c] = line[c + 1];
}

int stringIsInt(char * s)
{
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

int stringHasChar(char * s, char c)
{
    for (int i = 0; i < strlen(s); ++i)
    {
        if (s[i] == c)
            return 1;
    }
    return 0;
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

char * compileline(char * line[], int num, int lineLen, int isInline)
{
    // num is current line number

    char r[512] = {0};
    char * r_ptr = r;
    int len;
    if (lineLen == -1) len = arrsize(line);
    else len = lineLen;

    if (startswith(line[0], "//"))
        return r_ptr;

    if (line[1] == NULL)
        line[1] = "";

    if (strcmp(line[0], "push") == 0)
    {
        if (isInline) error("push action must be at start of line", num);
        if (len < 2) error("push action missing argument", num);
        arrlstrip(line);
        len--;
        strcat(r, "push(pt,");
        strcat(r, compileline(line, num, len, 1));
        strcat(r, ");");
    }
    else if (strcmp(line[0], "pop") == 0)
    {
        if (!isInline) error("pop action must not be at start of line", num);
        if (len > 1) error("pop action received too many arguments", num);
        strcat(r, "pop(pt)");
    }
    else if (strcmp(line[0], "print") == 0)
    {
        if (len < 2) strcat(r, "print(pop(pt));");
        else
        {
            arrlstrip(line);
            len--;
            strcat(r, "print(");
            strcat(r, compileline(line, num, len, 1));
            strcat(r, ");");
        }
    }
    else if (strcmp(line[0], "println") == 0)
    {
        if (len < 2) strcat(r, "println(pop(pt));");
        else
        {
            arrlstrip(line);
            len--;
            strcat(r, "println(");
            strcat(r, compileline(line, num, len, 1));
            strcat(r, ");");
        }
    }
    else if (strcmp(line[0], "if") == 0)
    {
        if (isInline) error("if statement must be at start of line", num);
        if (len < 2) error("if statement missing condition", num);
        else if (len > 2) error("if statement received too many conditions", num);
        strcat(r, "if(");
        strcat(r, line[1]);
        strcat(r, "){");
        scope++;
    }
    else if (strcmp(line[0], "elif") == 0)
    {
        if (isInline) error("elif statement must be at start of line", num);
        if (len < 2) error("elif statement missing condition", num);
        else if (len > 2) error("elif statement received too many conditions", num);
        strcat(r, "else if(");
        strcat(r, line[1]);
        strcat(r, "){");
        scope++;
    }
    else if (strcmp(line[0], "else") == 0)
    {
        if (isInline) error("else statement must be at start of line", num);
        else if (len > 1) error("else statement received too many conditions", num);
        strcat(r, "else{");
        scope++;
    }
    else if (strcmp(line[0], "while") == 0)
    {
        if (isInline) error("while loop must be at start of line", num);
        if (len < 2) error("while loop missing condition", num);
        else if (len > 2) error("while loop received too many conditions", num);
        strcat(r, "while(");
        strcat(r, line[1]);
        strcat(r, "){");
        scope++;
    }
    else if (strcmp(line[0], "for") == 0)
    {
        if (isInline) error("for loop must be at start of line", num);
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
        strcat(r, "){");
        values[arrsize(values)] = line[2];
        scope++;
    }
    else if (strcmp(line[0], "foreach") == 0)
    {
        if (isInline) error("foreach loop must be at start of line", num);
        if (len < 2) error("foreach loop missing variable type", num);
        if (len < 3) error("foreach loop missing variable name", num);
        if (len < 4) error("foreach loop missing iterable", num);
        if (len > 4) error("foreach loop received too many arguments", num);
        strcat(r, "for(_FORVARNAME;");
        strcat(r, "_FORVARNAME<arrsize(");
        strcat(r, line[3]);
        strcat(r, ");_FORVARNAME++){");
        strcat(r, line[1]);
        strcat(r, " ");
        strcat(r, line[2]);
        strcat(r, "=");
        strcat(r, line[3]);
        strcat(r, "[_FORVARNAME];");
        values[arrsize(values)] = line[2];
        scope++;
    }
    else if (strcmp(line[0], "char*[]") == 0)
    {
        if (isInline) error("char*[] declaration must be at start of line", num);
        if (len < 2) error("char*[] declaration missing variable name", num);
        if (len < 3) error("char*[] declaration missing variable size", num);
        strcat(r, "char* ");
        strcat(r, line[1]);
        strcat(r, "[");
        strcat(r, line[2]);
        strcat(r, "];");
        values[arrsize(values)] = line[1];
    }
    else if (strcmp(line[0], "char[]") == 0)
    {
        if (isInline) error("char[] declaration must be at start of line", num);
        if (len < 2) error("char[] declaration missing variable name", num);
        if (len < 3) error("char[] declaration missing variable size", num);
        strcat(r, "char ");
        strcat(r, line[1]);
        strcat(r, "[");
        strcat(r, line[2]);
        strcat(r, "]={0};");
        values[arrsize(values)] = line[1];
    }
    else if (strcmp(line[0], "string") == 0)
    {
        if (isInline) error("string declaration must be at start of line", num);
        else if (len < 2) error("string declaration missing variable name", num);
        else if (len < 3)
        {
            strcat(r, "string ");
            strcat(r, line[1]);
            strcat(r, ";");
            pointers[arrsize(pointers)] = line[1];
        }
        else
        {
            strcat(r, "string ");
            strcat(r, line[1]);
            strcat(r, "=");
            pointers[arrsize(pointers)] = line[1];
            arrlstrip(line);
            arrlstrip(line);
            len -= 2;
            char * stringvalue = compileline(line, num, len, 1);
            if (endswith(stringvalue, ","))
                strcpy(stringvalue, stringslice(String(stringvalue), 0, 1).value);
            if (!startswith(stringvalue, "String("))
            {
                char newstring[1024] = {0};
                strcpy(newstring, "String(");
                strcat(newstring, stringvalue);
                strcat(newstring, ")");
                strcat(r, newstring);
            } else strcat(r, stringvalue);
            strcat(r, ";");
        }
    }
    else if (strcmp(line[0], "string[]") == 0)
    {
        if (isInline) error("string[] declaration must be at start of line", num);
        else if (len < 2) error("string[] declaration missing variable name", num);
        else if (len < 3)
        {
            strcat(r, "string ");
            strcat(r, line[1]);
            strcat(r, "[1];");
            arrptrs[arrsize(arrptrs)] = line[1];
        }
        else
        {
            strcat(r, "string ");
            strcat(r, line[1]);
            strcat(r, "[]=");
            arrptrs[arrsize(arrptrs)] = line[1];
            arrlstrip(line);
            arrlstrip(line);
            len -= 2;
            char * stringvalue = compileline(line, num, len, 1);
            strcat(r, stringvalue);
            strcat(r, ";");
        }
    }
    else if (strcmp(line[0], "int") == 0)
    {
        if (isInline) error("int declaration must be at start of line", num);
        else if (len < 2) error("int declaration missing arguments", num);
        else if (len < 3)
        {
            strcat(r, "int ");
            strcat(r, line[1]);
            strcat(r, ";");
            values[arrsize(values)] = line[1];
        }
        else
        {
            strcat(r, "int ");
            strcat(r, line[1]);
            strcat(r, ";");
            strcat(r, line[1]);
            strcat(r, "=");
            values[arrsize(values)] = line[1];
            arrlstrip(line);
            arrlstrip(line);
            len -= 2;
            strcat(r, compileline(line, num, len, 1));
            strcat(r, ";");
        }
    }
    else if (strcmp(line[0], "pointer") == 0)
    {
        if (isInline) error("pointer declaration must be at start of line", num);
        if (len < 2) error("pointer declaration missing arguments", num);
        if (len < 3)
        {
            strcat(r, "pointer ");
            strcat(r, line[1]);
            strcat(r, ";");
        }
        else
        {
            strcat(r, "pointer ");
            strcat(r, line[1]);
            strcat(r, ";");
            strcat(r, line[1]);
            strcat(r, ".value=");
            pointers[arrsize(pointers)] = line[1];
            arrlstrip(line);
            arrlstrip(line);
            len -= 2;
            strcat(r, compileline(line, num, len, 1));
            strcat(r, ";");
        }
    }
    else if (strcmp(line[0], "set") == 0)
    {
        if (isInline) error("set action must be at start of line", num);
        if (len < 2) error("set action missing variable name", num);
        if (len < 3) error("set action missing new variable value", num);
        if (!stringInList(pointers, line[1]) && !stringInList(values, line[1]))
            error("set action attempting to set undeclared variable", num);
        strcat(r, line[1]);
        strcat(r, "=");
        arrlstrip(line);
        arrlstrip(line);
        len -= 2;
        strcat(r, compileline(line, num, len, 1));
        strcat(r, ";");
    }
    else if (strcmp(line[0], "var") == 0)
    {
        if (isInline) error("var declaration must be at start of line", num);
        if (len < 2) error("var declaration missing variable type", num);
        if (len < 3) error("var declaration missing variable name", num);
        if (len < 4)
        {
            strcat(r, line[1]);
            strcat(r, " ");
            strcat(r, line[2]);
            strcat(r, ";");
        }
        else
        {
            strcat(r, line[1]);
            strcat(r, " ");
            strcat(r, line[2]);
            strcat(r, "=");
            strcat(r, line[3]);
            strcat(r, ";");
        }
    }
    else if (startswith(line[1], "(") && endswith(line[1], ")"))
    {
        char * args = line[1];
        args++;
        args[strlen(args) - 1] = 0;
        if (len > 3) error("function call can not take arguments after initial arguments", num);
        strcat(r, line[0]);
        strcat(r, "(");
        strcat(r, args);
        strcat(r, ")");
        if (!isInline)
            strcat(r, ";");
    }
    else if (startswith(line[1], "[") && endswith(line[1], "]"))
    {
        if (len > 3) error("index can not take arguments", num);
        strcat(r, line[0]);
        strcat(r, "[");
        strcat(r, stringslice(String(line[1]), 1, 1).value);
        strcat(r, "]");
        if (stringInList(arrptrs, line[0]))
        {
            strcat(r, ".value");
        }
        
        if (!isInline)
            strcat(r, ";");
    }
    else if (endswith(line[0], ":"))
    {
        char * name = stringslice(String(line[0]), 0, 1).value;
        if (isInline) error("proc `:` action must be at start of line", num);
        if (scope > 0) error("proc `:` action can not be in a scope greater than 1", num);
        if (len < 1) error("proc `:` action missing arguments", num);
        else if (len < 2)
        {
            strcat(r, "int ");
            strcat(r, name);
            strcat(r, "(");
            if (strcmp(name, "main") == 0)
            {
                strcat(r, "int argc,char**argv");
                values[arrsize(values)] = "argc";
                pointers[arrsize(pointers)] = "argv";
            }
            strcat(r, "){");
        }
        else if (len > 3) error("proc `:` action received too many arguments", num);
        else
        {
            if (strcmp(name, "main") == 0 && len > 2)
                error("main proc can not be given arguments as they are given by default", num);
            strcat(r, "int ");
            strcat(r, name);
            strcat(r, "(");
            if (strcmp(name, "main") == 0)
            {
                strcat(r, "int argc,char**argv");
                values[arrsize(values)] = "argc";
                pointers[arrsize(pointers)] = "argv";
            }
            else strcat(r, line[2]);
            strcat(r, "){");
        }
        if (strcmp(name, "main") == 0)
            strcat(r, "pt=newStack(4096);");
        scope++;
    }
    else if (strcmp(line[0], "def") == 0)
    {
        if (isInline) error("def action must be at start of line", num);
        if (scope > 0) error("def action can not be in a scope greater than 1", num);
        if (len < 2) error("def action missing type", num);
        if (len < 3) error("def action missing name", num);
        else if (len < 4)
        {
            strcat(r, line[1]);
            strcat(r, " ");
            strcat(r, line[2]);
            strcat(r, "(){");
        }
        else if (len > 4) error("def action received too many arguments", num);
        else
        {
            strcat(r, line[1]);
            strcat(r, " ");
            strcat(r, line[2]);
            strcat(r, "(");
            strcat(r, line[3]);
            strcat(r, "){");
        }
        scope++;
    }
    else if (strcmp(line[0], "end") == 0)
    {
        if (isInline) error("end action must be at start of line", num);
        if (scope < 1) error("end action has nothing to end, program is already at minimum scope", num);
        if (len > 1) error("end action received too many arguments", num);
        strcat(r, "}");
        scope--;
    }
    else if (strcmp(line[0], "continue") == 0)
    {
        if (isInline) error("continue action must be at start of line", num);
        if (scope < 1) error("continue action has nothing to end, program is already at minimum scope", num);
        if (len > 1) error("continue action received too many arguments", num);
        strcat(r, "continue;");
    }
    else if (strcmp(line[0], "ret") == 0)
    {
        if (isInline) error("ret action must be at start of line", num);
        if (scope < 1) error("ret action can not return a value at minimum scope", num);
        if (len < 2) error("ret action missing argument", num);
        else if (len > 2) error("ret action received too many arguments", num);
        else
        {
            strcat(r, "return ");
            arrlstrip(line);
            len--;
            strcat(r, compileline(line, num, len, 1));
            strcat(r, ";");
        }
    }
    else if (strcmp(line[1], "++") == 0)
    {
        if (isInline) error("++ action must be at start of line, after variable name", num);
        if (scope < 1) error("++ action can not be at minimum scope", num);
        if (len > 3) error("++ action received too many arguments", num);
        else if (len > 2)
        {
            strcat(r, line[0]);
            strcat(r, "+=");
            strcat(r, line[2]);
            strcat(r, ";");
        }
        else
        {
            strcat(r, "++");
            strcat(r, line[0]);
            strcat(r, ";");
        }
    }
    else if (strcmp(line[1], "--") == 0)
    {
        if (isInline) error("-- action must be at start of line, after variable name", num);
        if (scope < 1) error("-- action can not be at minimum scope", num);
        if (len > 3) error("-- action received too many arguments", num);
        else if (len > 2)
        {
            strcat(r, line[0]);
            strcat(r, "-=");
            strcat(r, line[2]);
            strcat(r, ";");
        }
        else
        {
            strcat(r, "--");
            strcat(r, line[0]);
            strcat(r, ";");
        }
    }
    else if (startswith(line[0], "\"") && endswith(line[0], "\""))
    {
        for (int p = 0; p < len; p++)
        {
            if (!(startswith(line[p], "\"") && endswith(line[p], "\"")))
                break;
            strcat(r, "String(");
            strcat(r, line[p]);
            strcat(r, ")");
            if (!isInline)
                strcat(r, ";");
        }
    }
    else if (startswith(line[0], "'") && endswith(line[0], "'"))
    {
        int tempnum = 0;
        for (int p = 0; p < len; p++)
        {
            if (!(startswith(line[p], "'") && endswith(line[p], "'")))
                break;
            strcat(r, "String(\"");
            strcat(r, stringslice(stringreplace(String(line[p]), String("\""), String("\\\"")), 1, 1).value);
            strcat(r, "\")");
            if (!isInline)
                strcat(r, ";");
            else strcat(r, ",");
        }
    }
    else if (startswith(line[0], "{") && endswith(line[0], "}"))
    {
        char * tempstr[512];
        tokenise(tempstr, stringslice(String(line[0]), 1, 1).value, ",");
        println("START");
        int templen = 0;
        for (int p = 0; p < arrsize(tempstr); p++)
        {
            tempstr[p] = cpstrip(tempstr[p]);
            templen++;
            println(tempstr[p]);
        }
        println("END");
        strcat(r, "{");
        strcat(r, compileline(tempstr, num, templen, 1));
        strcat(r, "}");
        if (!isInline)
            strcat(r, ";");
    }
    else if (stringIsInt(line[0]))
    {
        strcat(r, line[0]);
        if (!isInline)
            strcat(r, ";");
    }
    else if (stringInList(pointers, line[0]))
    {
        strcat(r, line[0]);
        strcat(r, ".value");
        if (!isInline)
            strcat(r, ";");
    }
    else if (stringInList(values, line[0]))
    {
        strcat(r, line[0]);
        if (!isInline)
            strcat(r, ";");
    }
    else if (stringHasChar(line[0], ','))
    {
        strcat(r, line[0]);
        if (!isInline)
            strcat(r, ";");
    }
    else
    {
        strcat(r, line[0]);
    }

    return r_ptr;
}

int main(int argc, char ** argv)
{
    if (argc > 3)
    {
        if (strcmp(argv[3], "-v") == 0)
            verbose = 1;
    }

    if (verbose) println("--START--");

    if (argc < 3) return 1;

    char * buffer = readfile(argv[1]);

    if (!buffer)
        return 0;

    char * tokens[1024];
    ntokenise(tokens, buffer, "\n");

    char compiled[6000] = {0}; // Keep watch of this in case the files start to get bigger than 6000 characters
    for (int i = 0; i < arrsize(tokens); i++)
    {
        if (tokens[i] == NULL)
            continue;

        char * line[512];
        tokenise(line, tokens[i], "|");
        for (int p = 0; line[p] != NULL; p++)
        {
            line[p] = cpstrip(line[p]);
        }

        if (arrsize(line) < 1) continue;
        if (verbose) printf("SIZE %d\n", arrsize(line));

        char * result = compileline(line, i, -1, 0);
        strcat(compiled, result);

        if (verbose) printf("-%s-\n", tokens[i]);
        for (int p = 0; line[p] != NULL; p++)
        {
            if (verbose) printf("<%s>\n", line[p]);
        }
    }

    FILE * fp = fopen(argv[2], "w");
    fprintf(fp, "%s", C_HEADERS);
    fprintf(fp, "%s", C_FILE_START);
    fprintf(fp, "%s", compiled);
    fclose(fp);

    if (verbose) printf("--END--\n");

    return 0;
}
