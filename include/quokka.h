#include "qdef.h"
#include "qstring.h"

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

// print
int iprint(int value);
long lprint(long value);
char cprint(char value);
char * cpprint(char * value);
string sprint(string value);
string * saprint(string value[]);

// Definitions
#define arrsize(value) _Generic((value),\
    char **  : carrsize,\
    int *    : iarrsize,\
    default  : carrsize)(value)

#define print(value) _Generic((value),\
    int      : iprint,\
    long     : lprint,\
    char     : cprint,\
    char *   : cpprint,\
    string   : sprint,\
    default  : cpprint)(value)

#define println(value) _Generic((value),\
    int      : iprintln,\
    long     : lprintln,\
    char     : cprintln,\
    char *   : cpprintln,\
    string   : sprintln,\
    default  : cpprintln)(value)

// Used to typecast Object pointers into Quokka functions
typedef Object (*standard_func_def)(Object * argv);

//
/// All the rest
//

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

char * makeLiteralString(char * str)
{
    if ((startswith(str, "'") && endswith(str, "'")) ||
        (startswith(str, "\"") && endswith(str, "\"")))
        str = __slice_string__(String(str), 1, 1).value;

    char * newstr = malloc(strlen(str) + 1);
    strcpy(newstr, "");

    int len = strlen(str);
    int escaped = 0;

    for (int i = 0; i < len; i++)
    {
        if (str[i] == '\\')
            escaped = 1;
        else if (escaped)
        {
            if      (str[i] == 'a')  strcat(newstr, "\a");
            else if (str[i] == 'b')  strcat(newstr, "\b");
            else if (str[i] == 'e')  strcat(newstr, "\e");
            else if (str[i] == 'f')  strcat(newstr, "\f");
            else if (str[i] == 'n')  strcat(newstr, "\n");
            else if (str[i] == 'r')  strcat(newstr, "\r");
            else if (str[i] == 't')  strcat(newstr, "\t");
            else if (str[i] == 'v')  strcat(newstr, "\v");
            else if (str[i] == '\\') strcat(newstr, "\\");
            else if (str[i] == '\'') strcat(newstr, "'");
            else if (str[i] == '"')  strcat(newstr, "\"");
            else if (str[i] == '?')  strcat(newstr, "?");
            escaped = 0;
        }
        else
        {
            newstr[strlen(newstr)] = str[i];
            escaped = 0;
        }
    }

    return newstr;
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
