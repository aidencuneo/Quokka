// VERSION STUFF
#define VERSION "0.2.2"

// Defines
#define LN10 2.3025850929940456840179914546844

// Global file stuff
char * current_file;
char * cli_current_line;
int in_cli_mode = 0;

//
/// OS-related definitions
//

#ifdef _WIN32
    #include <direct.h>
    #define chdir(value) _chdir(value)
    #define realpath(first, second) _fullpath(NULL, first, _MAX_PATH)

    char * strndup(const char * s1, size_t n)
    {
    	char * copy = malloc(n + 1);
    	memcpy(copy, s1, n);
    	copy[n] = 0;
    	return copy;
    }
#else
    #include <unistd.h>
#endif

//
/// Includes deliberately after OS-related definitions
//

#include "qdef.h"

//
/// Memory
//

Object * mem;
int memsize;

//
/// Function declarations
//

char * strSlice(char * st, int start, int stop);
char * strReplace(char * orig, char * rep, char * with);
void mstrcat(char ** charptr, char * newstr);
void mstrcattrip(char ** charptr, char * newstr, char * endstr);

// Compiler stuff
void error(char * text, int line);

// Interpreter stuff
void freeVars();
void freeMemory();
void freeStack();
void freeRetStack();
void emptyTrash();
void cleanupAll();

// arrsize
int carrsize(char * arr[]);
int iarrsize(int * arr);

// println
int iprintln(int value);
long lprintln(long value);
char cprintln(char value);
char * cpprintln(char * value);

// print
int iprint(int value);
long lprint(long value);
char cprint(char value);
char * cpprint(char * value);

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
    default  : cpprint)(value)

#define println(value) _Generic((value),\
    int      : iprintln,\
    long     : lprintln,\
    char     : cprintln,\
    char *   : cpprintln,\
    default  : cpprintln)(value)

// Used to typecast Object pointers into Quokka functions
typedef Object (*standard_func_def)(int argc, Object * argv);

//
/// All the rest
//

int carrsize(char ** arr)
{
    int i;
    for (i = 0; arr[i] != NULL; i++);

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

    char * res = strReplace(rp, "\\", "/");
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
    char * buffer;
    long length;

    FILE * f = fopen(fname, "rb");

    if (!f) return 0;

    fseek(f, 0, SEEK_END);
    length = ftell(f);
    fseek(f, 0, SEEK_SET);

    buffer = malloc(length);
    strcpy(buffer, "");

    if (buffer)
        fread(buffer, 1, length, f);

    fclose(f);

    buffer = realloc(buffer, length + 1);
    buffer[length] = '\0';

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

int stringHasChar(char * s, char c)
{
    for (int i = 0; i < strlen(s); ++i)
    {
        if (s[i] == c)
            return 1;
    }
    return 0;
}

int charCount(char * st, char ch)
{
    int len = strlen(st);

    int count = 0;

    for (int i = 0; i < len; i++)
    {
        if (st[i] == ch)
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
        str = strSlice(str, 1, 1);

    char * newstr = malloc(strlen(str) + 1);
    memset(newstr, 0, strlen(str) + 1);
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

    free(str);

    return newstr;
}

char * makeRawString(char * str)
{
    int dquotes;

    char * newstr = malloc(2);

    if (stringHasChar(str, '\''))
    {
        strcpy(newstr, "\"");
        dquotes = 1;
    }
    else
    {
        strcpy(newstr, "'");
        dquotes = 0;
    }

    int len = strlen(str);
    int escaped = 0;

    for (int i = 0; i < len; i++)
    {
        if      (str[i] == '\a') mstrcat(&newstr, "\\a");
        else if (str[i] == '\b') mstrcat(&newstr, "\\b");
        else if (str[i] == '\e') mstrcat(&newstr, "\\e");
        else if (str[i] == '\f') mstrcat(&newstr, "\\f");
        else if (str[i] == '\n') mstrcat(&newstr, "\\n");
        else if (str[i] == '\r') mstrcat(&newstr, "\\r");
        else if (str[i] == '\t') mstrcat(&newstr, "\\t");
        else if (str[i] == '\v') mstrcat(&newstr, "\\v");
        else if (str[i] == '\\') mstrcat(&newstr, "\\\\");
        else if (str[i] == '\'' && !dquotes) mstrcat(&newstr, "\\'");
        else if (str[i] == '"' && dquotes)   mstrcat(&newstr, "\\\"");
        else
        {
            newstr = realloc(newstr, strlen(newstr) + 1 + 1);
            strncat(newstr, &str[i], 1);
        }
    }

    if (dquotes)
        mstrcat(&newstr, "\"");
    else mstrcat(&newstr, "'");

    return newstr;
}

double log10LN(double x)
{
    double old_sum = 0.0;
    double xmlxpl = (x - 1) / (x + 1);
    double xmlxpl_2 = xmlxpl * xmlxpl;
    double denom = 1.0;
    double frac = xmlxpl;
    double term = frac;
    double sum = term;

    while (sum != old_sum)
    {
        old_sum = sum;
        denom += 2.0;
        frac *= xmlxpl_2;
        sum += frac / denom;
    }

    return 2.0 * sum;
}

double log10(double x)
{
    return log10LN(x) / LN10;
}

char * intToStr(int value)
{
    int length = snprintf(NULL, 0, "%d", value);

    if (!length)
    {
        char * empty = malloc(1);
        empty[0] = '\0';

        return empty;
    }

    char * newval = malloc(length + 1);
    strcpy(newval, "");

    sprintf(newval, "%d", value);

    return newval;
}

char * LLToStr(long long value)
{
    char * newval = malloc(20);
    sprintf(newval, "%lld", value);

    char * out = strndup(newval, strlen(newval));
    free(newval);

    return out;
}

int ipowMath(int base, int exp)
{
    int result = 1;
    for (;;)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (!exp)
            break;
        base *= base;
    }

    return result;
}

long long lpowMath(long long base, int exp)
{
    long long result = 1;
    for (;;)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (!exp)
            break;
        base *= base;
    }

    return result;
}

char * strSlice(char * st, int start, int stop)
{
    int len = strlen(st);

    char * x = malloc(len - start - stop + 1);
    strcpy(x, "");

    if (!st || !len)
    {
        free(x);
        return st;
    }

    for (int i = start; i < len - stop; i++)
        strncat(x, &st[i], 1);

    return x;
}

char * strReplace(char * orig, char * rep, char * with)
{
    char * result; // the return string
    char * ins;    // the next insert point
    char * tmp;    // varies
    int len_rep;  // length of rep (the string to remove)
    int len_with; // length of with (the string to replace rep with)
    int len_front; // distance between rep and end of last rep
    int count;    // number of replacements

    // sanity checks and initialization
    if (!orig || !rep)
        return 0;
    len_rep = strlen(rep);
    if (len_rep == 0)
        return 0; // empty rep causes infinite loop during count
    if (!with)
        with = "";
    len_with = strlen(with);

    // count the number of replacements needed
    ins = orig;
    for (count = 0; (tmp = strstr(ins, rep)); ++count) {
        ins = tmp + len_rep;
    }

    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
    {
        free(tmp);
        return 0;
    }

    // first time through the loop, all the variable are set correctly
    // from here on,
    //    tmp points to the end of the result string
    //    ins points to the next occurrence of rep in orig
    //    orig points to the remainder of orig after "end of rep"
    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    }

    strcpy(tmp, orig);

    return result;
}

char * getinput()
{
    char * buffer = malloc(1);
    strcpy(buffer, "");
    int buflen = 0;

    char last = 0;

    while (last != '\n' && last != '\r')
    {
        last = getchar();

        buffer = realloc(buffer, strlen(buffer) + 1);
        buffer[buflen] = last;

        buflen++;
    }

    buffer[buflen - 1] = '\0';

    return buffer;
}

void mstrcat(char ** charptr, char * newstr)
{
    int len = strlen(*charptr) + strlen(newstr);
    *charptr = realloc(*charptr, len + 1);
    strcat(*charptr, newstr);
    (*charptr)[len] = '\0';
}

void mstrcattrip(char ** charptr, char * newstr, char * endstr)
{
    int len = strlen(*charptr) + strlen(newstr) + strlen(endstr);
    *charptr = realloc(*charptr, len + 1);
    strcat(*charptr, newstr);
    strcat(*charptr, endstr);
    (*charptr)[len] = '\0';
}

int intArrHas(int * arr, int ptr, int size)
{
    for (int i = 0; i < size; i++)
        if (arr[i] == ptr)
            return 1;

    return 0;
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
