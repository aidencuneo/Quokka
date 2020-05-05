#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "qdef.h"
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
long lprintln(long value);
char cprintln(char value);
char * cpprintln(char * value);
string sprintln(string value);
string * saprintln(string value[]);
integer intprintln(integer value);
bool bprintln(bool value);

// print
int iprint(int value);
long lprint(long value);
char cprint(char value);
char * cpprint(char * value);
string sprint(string value);
string * saprint(string value[]);
integer intprint(integer value);
bool bprint(bool value);


// Definitions
#define arrsize(value) _Generic((value),\
    char **  : carrsize,\
    int *    : iarrsize,\
    string * : sarrsize,\
    default  : carrsize)(value)

#define print(value) _Generic((value),\
    int      : iprint,\
    long     : lprint,\
    char     : cprint,\
    char *   : cpprint,\
    string   : sprint,\
    integer  : intprint,\
    bool     : bprint,\
    default  : cpprint)(value)

#define println(value) _Generic((value),\
    int      : iprintln,\
    long     : lprintln,\
    char     : cprintln,\
    char *   : cpprintln,\
    string   : sprintln,\
    integer  : intprintln,\
    bool     : bprintln,\
    default  : cpprintln)(value)

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
    string st = String(chst);

    int count = 0;

    for (int i = 0; i < st.length.value; i++)
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

    char * res = __replace_string__(String(rp), String("\\"), String("/")).value;
    free(rp);

    return res;
}

#define ptrindex(value, index) _Generic((value, index), char ** : cptrindex, default : cptrindex)(value, index)

string readchar()
{
    return String(getchar());
}

string input()
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

integer readint()
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

long lprint(long value)
{
    printf("%ld", value);
    return value;
}

char cprint(char value)
{
    printf("%c", value);
    return value;
}

char * cpprint(char * value)
{
    printf("%s", value);
    return value;
}

string sprint(string value)
{
    printf("%s", value.value);
    return value;
}

integer intprint(integer value)
{
    printf("%ld", value.value);
    return value;
}

bool bprint(bool value)
{
    if (value.value.value)
        printf("true");
    else printf("false");
    return value;
}

// println

int iprintln(int value)
{
    printf("%d\n", value);
    return value;
}

long lprintln(long value)
{
    printf("%ld\n", value);
    return value;
}

char cprintln(char value)
{
    printf("%c\n", value);
    return value;
}

char * cpprintln(char * value)
{
    printf("%s\n", value);
    return value;
}

string sprintln(string value)
{
    printf("%s\n", value.value);
    return value;
}

integer intprintln(integer value)
{
    printf("%ld\n", value.value);
    return value;
}

bool bprintln(bool value)
{
    if (value.value.value)
        printf("true");
    else printf("false");
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
