#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

#include "qdef.h"
#include "qobject.h"
#include "qinteger.h"
#include "qstring.h"
#include "qbool.h"

// File stuff
char * current_file;

//
/// OS-related definitions
//

#ifdef _WIN32
    #include <direct.h>
    #define chdir(value) _chdir(value)
    #define realpath(first, second) _fullpath(first, second, _MAX_PATH)

    char *strndup(const char *s1, size_t n)
    {
    	char *copy = (char *)malloc(n + 1);
    	memcpy(copy, s1, n);
    	copy[n] = 0;
    	return copy;
    }
#else
    #include <unistd.h>
#endif

//
/// Function declarations
//

// arrsize
int carrsize(char * arr[]);
int iarrsize(int * arr);
int sarrsize(string * arr);

// println
int iprintln(int value);
char * cpprintln(char * value);
object oprintln(object value);

// print
int iprint(int value);
char * cpprint(char * value);
object oprint(object value);


// Definitions
#define arrsize(value) _Generic((value),\
    char **  : carrsize,\
    int *    : iarrsize,\
    string * : sarrsize,\
    default  : carrsize)(value)

#define print(value) _Generic((value),\
    int      : iprint,\
    char *   : cpprint,\
    object   : oprint,\
    default  : cpprint)(value)

#define println(value) _Generic((value),\
    int      : iprintln,\
    char *   : cpprintln,\
    object   : oprintln,\
    default  : oprintln)(value)

//
/// All the rest
//



char * cptrindex(char ** value, int index)
{
    return value[index];
}

int carrsize(char * arr[])
{
    int i = 0;
    long j = (long) arr[0];
    while (j != '\0')
    {
        i++;
        j = (long) arr[i];
    }
    return i;
}

int iarrsize(int * arr)
{
    return sizeof(arr) / sizeof(int);
}

int sarrsize(string * arr)
{
    int i = 0;
    string j = arr[0];
    while (strncmp(j.value, "\0", 2) != 0)
    {
        i++;
        j = arr[i];
    }
    return i;
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

char * readfile(char * fname)
{
    char * buffer = 0;
    long length;

    FILE * f = fopen(fname, "rb");

    if (f)
    {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        buffer = malloc(length);
        if (buffer)
            fread(buffer, 1, length, f);
        fclose(f);
    }

    return buffer;
}

char * nstrtok(char * string, char const * delimiter)
{
    static char * source = NULL;
    char * p, * ret = 0;

    if (string != NULL) source = string;
    if (source == NULL) return NULL;

    if ((p = strpbrk(source, delimiter)) != NULL)
    {
        * p = 0;
        ret = source;
        source = ++p;
    }

    return ret;
}

char * cpstrip(char * s)
{
    if (!s)
        return s;

    size_t size;
    char * end;

    size = strlen(s);

    if (!size)
        return s;

    end = s + size - 1;
    while (end >= s && isspace(* end))
        end--;
    *(end + 1) = '\0';

    while (* s && isspace( * s))
        s++;

    return s;
}

int charCount(char * chst, char ch)
{
    string st = * ((string *)String(chst).obj);

    int count = 0;

    for (int i = 0; i < st.length; i++)
    {
        if (st.value[i] == ch)
            count++;
    }

    return count;
}

void tokenise(char * arr[], char * buffer, char * tokAt)
{
    int i = 0;
    arr[0] = cpstrip(strtok(buffer, tokAt));
    while (arr[i] != NULL)
    {
        i++;
        arr[i] = cpstrip(strtok(NULL, tokAt));
    }
}

void ntokenise(char * arr[], char * buffer, char * tokAt)
{
    int i = 0;
    arr[0] = cpstrip(nstrtok(buffer, tokAt));
    while (arr[i] != NULL)
    {
        i++;
        arr[i] = cpstrip(nstrtok(NULL, tokAt));
    }
}

char * getrealpath(char * path)
{
    char * rp = realpath(path, NULL);

    if (!rp)
    {
        free(rp);
        return 0;
    }

    char * res = ((string *)__replace_string__(String(rp), String("\\"), String("/")).obj)->value;
    free(rp);

    return res;
}

#define ptrindex(value, index) _Generic((value, index), char ** : cptrindex, default : cptrindex)(value, index)

object readchar()
{
    return String(getchar());
}

object input()
{
    char * buffer = (char *)malloc(1);
    strcpy(buffer, "");
    char last = 0;

    while (last != '\n' && last != '\r')
    {
        last = getchar();
        buffer = (char *)realloc(buffer, strlen(buffer) + 1);
        buffer[strlen(buffer)] = last;
    }

    buffer[strlen(buffer) - 1] = '\0';

    return String(buffer);
}

object readint()
{
    int out;
    scanf("%d", &out);
    return Integer(out);
}

float readfloat()
{
    float out;
    scanf("%f", &out);
    return out;
}

// print

int iprint(int value)
{
    printf("%d", value);
    return value;
}

char * cpprint(char * value)
{
    printf("%s", value);
    return value;
}

object oprint(object value)
{
    if (!strcmp(value.name, "integer"))
        printf("%ld", ((integer *)value.obj)->value);
    else if (!strcmp(value.name, "string"))
        printf("%s", ((string *)value.obj)->value);
    else if (!strcmp(value.name, "bool"))
    {
        if (((bool *)value.obj)->value)
            printf("true");
        else printf("false");
    }
    return value;
}

// println

int iprintln(int value)
{
    printf("%d\n", value);
    return value;
}

char * cpprintln(char * value)
{
    printf("%s\n", value);
    return value;
}

object oprintln(object value)
{
    if (!strcmp(value.name, "integer"))
        printf("%ld\n", ((integer *)value.obj)->value);
    else if (!strcmp(value.name, "string"))
        printf("%s\n", ((string *)value.obj)->value);
    else if (!strcmp(value.name, "bool"))
    {
        if (((bool *)value.obj)->value)
            printf("true\n");
        else printf("false\n");
    }
    return value;
}

//
/// More stuff
//

void raise(string text, char * lineprevious, char * linepreview, char * linenext, char * filename, int line)
{
    printf("At %s : Line %d\n\n", filename, line);

    if (line - 1 > 0)
        printf("  %d | %s\n", line - 1, lineprevious);
    printf("> %d > %s\n", line, linepreview);
    if (strlen(linenext))
        printf("  %d | %s\n", line + 1, linenext);
    print("\n");

    printf("Error: %s\n\n", text.value);

    exit(EXIT_FAILURE);
}
