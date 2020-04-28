#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

// OS-related definitions
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


// Structs
typedef struct
{
    void * value;
} pointer;

typedef struct __string_Struct__ string;
struct __string_Struct__
{
    char * value;
    int length;

    string (*upper)(string self);
    string (*lower)(string self);
};


// Function declarations

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

// string
string __lstrip_string__(string st);
string __upper_string__(string st);
string __lower_string__(string st);
string __rstrip_string__(string st);
string __strip_string__(string st);
string __add_string__(string arg1, ...);
string __upper_string__(string st);
string __lower_string__(string st);
string __slice_string__(string st, int start, int stop);
string __reorder_string__(string st, int step);
string __replace_string__(string st, string strep, string repwith);


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
    string * : saprint,\
    default  : cpprint)(value)

#define println(value) _Generic((value),\
    int      : iprintln,\
    long     : lprintln,\
    char     : cprintln,\
    char *   : cpprintln,\
    string   : sprintln,\
    string * : saprintln,\
    default  : cpprintln)(value)


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

string StringFromString(string value)
{
    return value;
}

string StringFromCharPointer(char * value)
{
    string self;

    self.value = value;
    self.length = strlen(value);

    self.upper = __upper_string__;
    self.lower = __lower_string__;

    return self;
}

string StringFromCharArray(char value[])
{
    string out;
    out.value = value;
    out.length = strlen(value);
    return out;
}

string StringFromInt(int value)
{
    char * newval = malloc(11);
    sprintf(newval, "%d", value);
    return StringFromCharPointer(newval);
}

string StringFromLong(long value)
{
    char * newval = malloc(21);
    sprintf(newval, "%ld", value);
    return StringFromCharPointer(newval);
}

string StringFromChar(char value)
{
    string out;
    char newval[2] = "\0";
    newval[0] = value;
    out.value = newval;
    out.length = strlen(out.value);
    return out;
}

#define String(value) _Generic((value),\
    int     : StringFromInt,\
    long    : StringFromLong,\
    char    : StringFromChar,\
    char *  : StringFromCharPointer,\
    string  : StringFromString,\
    default : StringFromCharPointer)(value)

#define __string_Constructor__(value) String(value)

string __lstrip_string__(string s)
{
    if (!s.value)
        return s;

    while (s.value[0] == ' ')
        s.value++;

    return s;
}

string __rstrip_string__(string st)
{
    char * s = (char *)malloc(st.length + 1);
    strcpy(s, st.value);

    if (!s)
        return st;

    size_t size;
    char * end;

    size = st.length;

    if (!size)
        return st;

    end = s + size - 1;
    while (end >= s && isspace(* end))
        end--;
    *(end + 1) = '\0';

    return String(s);
}

string __strip_string__(string st)
{
    char * s = (char *)malloc(st.length + 1);
    strcpy(s, st.value);

    if (!s)
        return st;

    size_t size;
    char * end;

    size = st.length;

    if (!size)
        return st;

    end = s + size - 1;
    while (end >= s && isspace(* end))
        end--;
    *(end + 1) = '\0';

    return __lstrip_string__(String(s));
}

string __add_string__(string arg1, ...)
{
    va_list ap;

    char * x = (char *)malloc(2048 * sizeof(char));
    strcpy(x, arg1.value);

    char * y;

    va_start(ap, arg1);

    string last = va_arg(ap, string);

    while (last.value != "\0")
    {
        strcat(x, last.value);
        last = va_arg(ap, string);
    }

    va_end(ap);
    string out = String(x);
    free(x);
    return out;
}

string __upper_string__(string st)
{
    char * x = (char *)malloc(st.length * sizeof(char));

    for (int i = 0; i < st.length; i++)
    {
        char letter[2] = {0};
        letter[0] = toupper(st.value[i]);
        strcat(x, letter);
    }

    return String(x);
}

string __lower_string__(string st)
{
    char * x = (char *)malloc(st.length * sizeof(char));

    for (int i = 0; i < st.length; i++)
    {
        char letter[2] = {0};
        letter[0] = tolower(st.value[i]);
        strcat(x, letter);
    }

    return String(x);
}

string __slice_string__(string st, int start, int stop)
{
    char * x = (char *)malloc(st.length + 1 * sizeof(char));
    strcpy(x, st.value);

    if (!st.value || !st.length)
        return st;

    char * end;

    end = x + st.length - 1;
    end -= stop;
    *(end + 1) = '\0';

    x += start;

    return String(x);
}

string __reorder_string__(string st, int step)
{
    char * x = (char *)malloc(st.length * sizeof(char));

    if (step < 1) step = 1;
    if (step > st.length) step = st.length;
    for (int i = 0; i < st.length; i += step)
    {
        char * letter = malloc(2 * sizeof(char));
        letter[0] = st.value[i];
        strcat(x, letter);
    }

    return String(x);
}

string __replace_string__(string st, string strep, string repwith) {
    char * orig = st.value;
    char * rep = strep.value;
    char * with = repwith.value;

    char * result; // the return string
    char * ins;    // the next insert point
    char * tmp;    // varies
    int len_rep;  // length of rep (the string to remove)
    int len_with; // length of with (the string to replace rep with)
    int len_front; // distance between rep and end of last rep
    int count;    // number of replacements

    // sanity checks and initialization
    if (!orig || !rep)
        return String("");
    len_rep = strlen(rep);
    if (len_rep == 0)
        return String(""); // empty rep causes infinite loop during count
    if (!with)
        with = "";
    len_with = strlen(with);

    // count the number of replacements needed
    ins = orig;
    for (count = 0; tmp = strstr(ins, rep); ++count) {
        ins = tmp + len_rep;
    }

    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return String("");

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
    string out;
    out = String(result);
    return out;
}

void sarrappend(string * lst, string arg1, ...)
{
    printf("<%s>\n", lst[0].value);

    va_list ap;

    int len = arrsize(lst);

    lst = (string *)realloc(lst, (len + 2) * sizeof(string));
    lst[len + 1] = String("\0");
    lst[len] = arg1;
    len++;

    va_start(ap, arg1);

    string last = va_arg(ap, string);

    while (strcmp(last.value, "\0") != 0)
    {
        printf("[%s]\n", last.value);
        lst = (string *)realloc(lst, (len + 2) * sizeof(string));
        lst[len + 1] = String("\0");
        lst[len] = last;
        len++;
        last = va_arg(ap, string);
    }

    va_end(ap);

    printf("<%s>\n", lst[0].value);
}

string * sarradd(string * lst, string arg1, ...)
{
    va_list ap;

    int len = arrsize(lst);
    string * newlst = (string *)malloc(len + 1024 * sizeof(string));

    for (int i = 0; i < len; i++)
        newlst[i] = lst[i];

    newlst[len + 1] = String("\0");
    newlst[len] = arg1;

    va_start(ap, arg1);

    string last = va_arg(ap, string);

    while (last.value != "\0")
    {
        newlst[len + 1] = String("\0");
        newlst[len] = last;
        len++;
        last = va_arg(ap, string);
    }

    va_end(ap);

    return newlst;
}

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
    while (j.value != "\0")
    {
        i++;
        j = arr[i];
    }
    return i;
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

void tokenise(char * arr[], char * buffer, char * tokAt)
{
    int i = 0;
    arr[0] = strtok(buffer, tokAt);
    while (arr[i] != NULL)
    {
        i++;
        arr[i] = cpstrip(strtok(NULL, tokAt));
    }
}

void ntokenise(char * arr[], char * buffer, char * tokAt)
{
    int i = 0;
    arr[0] = nstrtok(buffer, tokAt);
    while (arr[i] != NULL)
    {
        i++;
        arr[i] = cpstrip(nstrtok(NULL, tokAt));
    }
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

char * getrealpath(char * path)
{
    char * rp = realpath(path, NULL);
    if (!rp) return NULL;
    return __replace_string__(String(rp), String('\\'), String('/')).value;
}

#define ptrindex(value, index) _Generic((value, index), char ** : cptrindex, default : cptrindex)(value, index)

char readchar(){return getchar();}
int readint(){int out;scanf("%d",&out);return out;}
float readfloat(){float out;scanf("%f",&out);return out;}

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

string * saprint(string value[])
{
    printf("{");
    for (int i = 0; i < arrsize(value); i++)
    {
        if (i > 0)
            printf(", ");
        printf("%s", value[i].value);
    }
    printf("}");
    return value;
}

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

string * saprintln(string value[])
{
    printf("{");
    for (int i = 0; i < arrsize(value); i++)
    {
        if (i > 0)
            printf(", ");
        printf("%s", value[i].value);
    }
    printf("}\n");
    return value;
}
