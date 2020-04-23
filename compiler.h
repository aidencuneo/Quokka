#define C_HEADERS "\
#include <stdio.h>\n\
#include <string.h>\n\
#include <stdlib.h>\n"

#define C_DATATYPES "\
typedef struct {\n\
    void * value;\n\
} pointer;\n"

#define C_INPUT_FUNCS "\
char readchar(){return getchar();}\n\
int readint(){int out;scanf(\"%d\",&out);return out;}\n\
float readfloat(){float out;scanf(\"%f\",&out);return out;}\n"

#define C_PRINT_FUNCS "\
int iprint(int value){printf(\"%d\",value);return value;}\n\
long lprint(long value){printf(\"%ld\",value);return value;}\n\
char cprint(char value){printf(\"%c\",value);return value;}\n\
char*sprint(char*value){printf(\"%s\",value);return value;}\n"

#define C_PRINT_MANAGE "\
#define print(value) _Generic((value), int : iprint, long : lprint, char : cprint, char * : sprint, default : sprint)(value)\n"

#define C_PRINTLN_FUNCS "\
int iprintln(int value){printf(\"%d\\n\",value);return value;}\n\
long lprintln(long value){printf(\"%ld\\n\",value);return value;}\n\
char cprintln(char value){printf(\"%c\\n\",value);return value;}\n\
char*sprintln(char*value){printf(\"%s\\n\",value);return value;}\n"

#define C_PRINTLN_MANAGE "\
#define println(value) _Generic((value), int : iprintln, long : lprintln, char : cprintln, char * : sprintln, default : sprintln)(value)\n"

#define C_STACK_STRUCT "\
struct stack{int maxsize;int top;int*items;};struct stack*newStack(int capacity){struct stack*pt=(struct stack*)malloc(sizeof(struct stack));pt->maxsize=capacity;pt->top=-1;pt->items=(int*)malloc(sizeof(int)*capacity);return pt;}int size(struct stack*pt){return pt->top+1;}\n\
int isEmpty(struct stack*pt){return pt->top==-1;}\n\
int isFull(struct stack*pt){return pt->top==pt->maxsize-1;}\n\
void push(struct stack*pt,int x){if(isFull(pt))exit(EXIT_FAILURE);pt->items[++pt->top]=x;}\n\
int peek(struct stack*pt){if (!isEmpty(pt))return pt->items[pt->top];else return 0;}\n\
int pop(struct stack*pt){if(isEmpty(pt))return 0;return pt->items[pt->top--];}\n"

#define C_FILE_START "\
int CONDITIONAL_FLAG=1;\n\
struct stack*pt;\n"

// I need to remove the code below this eventually, it's not being used anywhere.

#define C_0 "c=0;"
#define C_1 "++c;"
#define C_2 "c+=2;"
#define C_3 "c+=3;"
#define C_4 "c+=4;"
#define C_5 "c+=5;"
#define C_6 "c+=6;"
#define C_7 "c+=7;"
#define C_8 "c+=8;"
#define C_9 "c+=9;"
#define C_10 "c+=10;"
#define C_11 "c+=11;"
#define C_12 "c+=12;"
#define C_13 "c+=13;"
#define C_14 "c+=14;"
#define C_15 "c+=15;"
#define C_MINUS "c-=pop(pt);"
#define C_PLUS "c+=pop(pt);"
#define C_WSET "w=c;"
#define C_WGET "c=w;"
#define C_XSET "x=c;"
#define C_XGET "c=x;"
#define C_YSET "y=c;"
#define C_YGET "c=y;"
#define C_ZSET "z=c;"
#define C_ZGET "c=z;"
#define C_SEMICOLON "}"
#define C_IFTRUE "if(c>0){"
#define C_IFNOT "if(c==0){"
#define C_WHILETRUE "while(c>0){"
#define C_WHILENOT "while(c==0){"
#define C_PRINTF "printf(\"%%d\",c);"
#define C_STACKPOP "c=pop(pt);"
#define C_STACKPUSH "push(pt,);"
#define C_GETBYTE "char d[2];fgets(d,2,stdin);c=d[0];"
#define C_PRINT "printf(\"%%c\",c);"
#define C_EXIT "return 0;"
