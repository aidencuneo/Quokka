#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

typedef struct
{
    void * value;
} pointer;

typedef struct
{
    char * value;
    int length;
} string;

string StringFromString(string value)
{
    return value;
}

string StringFromCharPointer(char * value)
{
    string out;
    out.value = value;
    out.length = strlen(value);
    return out;
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
    string out;
    char newval[11];
    sprintf(newval, "%d", value);
    return StringFromCharPointer(newval);
}

string StringFromLong(long value)
{
    string out;
    char newval[20];
    sprintf(newval, "%ld", value);
    out.value = newval;
    out.length = strlen(out.value);
    return out;
}

string StringFromChar(char value)
{
    string out;
    char newval[2] = "0";
    newval[0] = value;
    out.value = newval;
    out.length = strlen(out.value);
    return out;
}

#define String(value) _Generic((value),\
    int : StringFromInt,\
    long : StringFromLong,\
    char : StringFromChar,\
    char * : StringFromCharPointer,\
    string : StringFromString,\
    default : StringFromString)(value)

string stringlstrip(string s)
{
    if (!s.value)
        return s;

    while (s.value[0] == ' ')
        s.value++;

    return s;
}

string stringrstrip(string st)
{
    char s[st.length];
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

string stringstrip(string st)
{
    char s[st.length];
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

    return String(stringlstrip(String(s)));
}

string stringcat(string arg1, ...)
{
    va_list ap;

    char * x = (char *)malloc(2048 * sizeof(char));
    strcat(x, arg1.value);

    char * y;

    va_start(ap, arg1);

    string last = va_arg(ap, string);
    strcat(x, last.value);

    while (last.length)
    {
        last = va_arg(ap, string);
        printf("[%s]\n", last.value);
        strcat(x, last.value);
    }

    va_end(ap);
    return String(x);
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
        end--;*(end + 1) = '\0';

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

#define ptrindex(value, index) _Generic((value, index), char ** : cptrindex, default : cptrindex)(value, index)
#define arrsize(value) _Generic((value), char ** : carrsize, int * : iarrsize, default : carrsize)(value)

char readchar(){return getchar();}
int readint(){int out;scanf("%d",&out);return out;}
float readfloat(){float out;scanf("%f",&out);return out;}

int iprint(int value){printf("%d",value);return value;}
long lprint(long value){printf("%ld",value);return value;}
char cprint(char value){printf("%c",value);return value;}
char*cpprint(char*value){printf("%s",value);return value;}
string sprint(string value){printf("%s",value.value);return value;}

#define print(value) _Generic((value),\
    int : iprint,\
    long : lprint,\
    char : cprint,\
    char * : cpprint,\
    string : sprint,\
    default : sprint)(value)

int iprintln(int value){printf("%d\n",value);return value;}
long lprintln(long value){printf("%ld\n",value);return value;}
char cprintln(char value){printf("%c\n",value);return value;}
char*cpprintln(char*value){printf("%s\n",value);return value;}
string sprintln(string value){printf("%s\n",value.value);return value;}

#define println(value) _Generic((value),\
    int : iprintln,\
    long : lprintln,\
    char : cprintln,\
    char * : cpprintln,\
    string : sprintln,\
    default : sprintln)(value)

struct stack{int maxsize;int top;int*items;};struct stack*newStack(int capacity){struct stack*pt=(struct stack*)malloc(sizeof(struct stack));pt->maxsize=capacity;pt->top=-1;pt->items=(int*)malloc(sizeof(int)*capacity);return pt;}int size(struct stack*pt){return pt->top+1;}
int isEmpty(struct stack*pt){return pt->top==-1;}
int isFull(struct stack*pt){return pt->top==pt->maxsize-1;}
void push(struct stack*pt,int x){if(isFull(pt))exit(EXIT_FAILURE);pt->items[++pt->top]=x;}
int peek(struct stack*pt){if (!isEmpty(pt))return pt->items[pt->top];else return 0;}
int pop(struct stack*pt){if(isEmpty(pt))return 0;return pt->items[pt->top--];}
