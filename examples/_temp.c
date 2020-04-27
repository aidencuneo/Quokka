#include "../include/t.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int _FORVARNAME;
struct stack*pt;
typedef struct __potato_Struct__ potato;
struct __potato_Struct__ {
int x;
int y;
}
__potato_Constructor__ (int x,int y){potato self;self.x=x;
self.y=y;
return self;
}
int main(int argc,char**argv){
pt=newStack(4096);
potato potato_1=__potato_Constructor__(0, 0);
potato potato_2=__potato_Constructor__(0, 0);
print(String("Potato 1: "));
print(potato_1.x);
print(String(","));
println(potato_1.y);
print(String("Potato 2: "));
print(potato_2.x);
print(String(","));
println(potato_2.y);
}
