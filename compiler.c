#include "compiler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int scope = 0;
char * pointers[512];
char * values[512];

void error(char * text, int line)
{
    printf("\nProgram compilation terminated:\nLine %d -> %s\n\n", line + 1, text);
    exit(EXIT_FAILURE);
}

int arrsize(char * arr[])
{
    int i = 0;
    long j = (long)arr[0];
    while (j != '\0')
    {
        i++;
        j = (long)arr[i];
    }
    return i;
}

void arrlstrip(char * line[])
{
    int position, c, n;
    position = 0;
    n = arrsize(line);
    for (c = position - 1; c < n - 1; c++)
        line[c] = line[c + 1];
}

void lstrip(char * s, char stripChar)
{
    for ( ; *s != '\0'; ++s )
    {
        if (* s == stripChar)
            * s = * (s + 1);
        else
            break;
    }
}

int startswith(const char * a, const char * b)
{
    if (strncmp(a, b, strlen(b)) == 0) return 1;
    return 0;
}

int endswith(const char * str, const char * suffix)
{
    if (!str || !suffix)
        return 0;
    size_t lenstr = strlen(str);
    size_t lensuffix = strlen(suffix);
    if (lensuffix >  lenstr)
        return 0;
    return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
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

int stringInList(char * arr[], char * key)
{
    for (int n = 0; n < arrsize(arr); ++n)
    {
        if (strcmp(arr[n], key) == 0)
            return 1;
    }
    return 0;
}

char * nstrtok(char * string, char const * delimiter)
{
    static char *source = NULL;
    char *p, *ret = 0;
    if (string != NULL) source = string;
    if (source == NULL) return NULL;

    if ((p = strpbrk(source, delimiter)) != NULL)
    {
        *p = 0;
        ret = source;
        source = ++p;
    }
    return ret;
}

char * compileline(char * line[], int num, int lineLen, int isInline)
{
    char r[512] = {0};
    char * r_ptr = r;
    int len;
    if (lineLen == -1) len = arrsize(line);
    else len = lineLen;

    if (strcmp(line[0], "?") == 0)
    {
        if (isInline) error("'?' conditional must be at start of line", num);
        if (len < 2) error("'?' conditional missing code to run", num);
        arrlstrip(line);
        len--;
        strcat(r, "if(CONDITIONAL_FLAG){");
        strcat(r, compileline(line, num, len, 1));
        strcat(r, "}");
    }
    else if (strcmp(line[0], ":") == 0)
    {
        if (isInline) error("':' conditional must be at start of line", num);
        if (len < 2) error("':' conditional missing code to run", num);
        arrlstrip(line);
        len--;
        strcat(r, "while(CONDITIONAL_FLAG){");
        strcat(r, compileline(line, num, len - 1, 1));
        strcat(r, "}");
    }
    else if (strcmp(line[0], "push") == 0)
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
        else
        {
            strcat(r, "if(");
            strcat(r, line[1]);
            strcat(r, ")CONDITIONAL_FLAG=1;else CONDITIONAL_FLAG=0;");
        }
    }
    else if (strcmp(line[0], "int") == 0)
    {
        if (isInline) error("int declaration must be at start of line", num);
        if (len < 2) error("int declaration missing arguments", num);
        if (len < 3)
        {
            strcat(r, "int ");
            strcat(r, line[1]);
            strcat(r, ";");
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
    else if (strcmp(line[0], "call") == 0)
    {
        if (len < 2) error("call action missing arguments", num);
        else if (len < 3)
        {
            strcat(r, line[1]);
            strcat(r, "()");
        }
        else
        {
            strcat(r, line[1]);
            strcat(r, "(");
            strcat(r, line[2]);
            strcat(r, ")");
        }
        if (!isInline)
            strcat(r, ";");
    }
    else if (strcmp(line[0], "ch") == 0)
    {
        if (len < 2) strcat(r, "(char)0");
        else if (len > 3) error("ch action received too many arguments", num);
        else
        {
            strcat(r, "(char)");
            arrlstrip(line);
            len--;
            strcat(r, compileline(line, num, len, 1));
        }
        if (!isInline)
                strcat(r, ";");
    }
    else if (strcmp(line[0], "proc") == 0)
    {
        if (isInline) error("proc action must be at start of line", num);
        if (scope > 0) error("proc action can not be in a scope greater than 1", num);
        if (len < 2) error("proc action missing arguments", num);
        else if (len < 3)
        {
            strcat(r, "int ");
            strcat(r, line[1]);
            strcat(r, "(){");
        }
        else if (len > 3) error("proc action received too many arguments", num);
        else
        {
            strcat(r, "int ");
            strcat(r, line[1]);
            strcat(r, "(");
            strcat(r, line[2]);
            strcat(r, "){");
        }
        if (strcmp(line[1], "main") == 0)
            strcat(r, "pt=newStack(4096);");
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
    else if (strcmp(line[0], "ret") == 0)
    {
        if (isInline) error("ret action must be at start of line", num);
        if (scope < 1) error("ret action can not return a value at minimum scope", num);
        if (len < 2) error("ret action missing argument", num);
        else if (len > 2) error("ret action received too many arguments", num);
        else
        {
            strcat(r, "return ");
            strcat(r, line[1]);
            strcat(r, ";");
        }
    }
    else if (startswith(line[0], "\"") && endswith(line[0], "\""))
    {
        strcat(r, line[0]);
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
    else
    {
        char err[128] = {0};
        strcat(err, "undefined token '");
        strcat(err, line[0]);
        strcat(err, "'");
        error(err, num);
    }

    return r_ptr;
}

int main(int argc, char ** argv)
{
    printf("--START--\n");

    if (argc < 3) return 1;

    char * buffer = 0;
    long length;
    FILE * f = fopen(argv[1], "rb");

    if (f)
    {
        fseek(f, 0, SEEK_END);
        length = ftell (f);
        fseek(f, 0, SEEK_SET);
        buffer = malloc (length);
        if (buffer)
            fread (buffer, 1, length, f);
        fclose(f);
    }

    if (!buffer)
        return 0;

    int i = 0;
    char * tokens[1024];
    tokens[0] = nstrtok(buffer, "\n");
    while (tokens[i] != NULL)
    {
        i++;
        tokens[i] = nstrtok(NULL, "\n");
    }

    char compiled[4096] = {0};
    for (int i = 0; i < sizeof(tokens); i++)
    {
        if (tokens[i] == NULL)
            continue;
        else if (strcmp(tokens[i], "EOF") == 0)
            break;

        char * line[512];
        int j = 0;
        line[0] = strtok(tokens[i], ";");
        while (line[j] != NULL)
        {
            j++;
            line[j] = strtok(NULL, ";");
        }

        if (arrsize(line) < 1) continue;
        printf("SIZE %d\n", arrsize(line));

        char * result = compileline(line, i, -1, 0);
        strcat(compiled, result);

        printf("-%s-\n", tokens[i]);
        for (int p = 0; line[p] != NULL; p++)
            printf("<%s>\n", line[p]);
    }

    FILE * fp = fopen(argv[2], "w");
    fprintf(fp, "%s", C_HEADERS);
    fprintf(fp, "%s", C_DATATYPES);
    fprintf(fp, "%s", C_INPUT_FUNCS);
    fprintf(fp, "%s", C_PRINT_FUNCS);
    fprintf(fp, "%s", C_PRINT_MANAGE);
    fprintf(fp, "%s", C_PRINTLN_FUNCS);
    fprintf(fp, "%s", C_PRINTLN_MANAGE);
    fprintf(fp, "%s", C_STACK_STRUCT);
    fprintf(fp, "%s", C_FILE_START);
    fprintf(fp, "%s", compiled);
    fclose(fp);

    printf("--END--\n");

    return 0;
}
