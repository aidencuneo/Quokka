#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int iprint(int value){printf("%d",value);return value;}
char cprint(char value){printf("%c",value);return value;}
char*sprint(char*value){printf("%s",value);return value;}
#define print(value) _Generic((value),int:iprint,char:cprint,char*:sprint,default:sprint)(value)
struct stack{int maxsize;int top;int*items;};struct stack*newStack(int capacity){struct stack*pt=(struct stack*)malloc(sizeof(struct stack));pt->maxsize=capacity;pt->top=-1;pt->items=(int*)malloc(sizeof(int)*capacity);return pt;}int size(struct stack*pt){return pt->top+1;}
int isEmpty(struct stack*pt){return pt->top==-1;}
int isFull(struct stack*pt){return pt->top==pt->maxsize-1;}
void push(struct stack*pt,int x){if(isFull(pt))exit(EXIT_FAILURE);pt->items[++pt->top]=x;}
int peek(struct stack*pt){if (!isEmpty(pt))return pt->items[pt->top];else return 0;}
int pop(struct stack*pt){if(isEmpty(pt))return 0;return pt->items[pt->top--];}
int main(int argc,char**argv){struct stack*pt=newStack(4096);
print(15);print((char)10);print("aiden\n");return 0;}