#include "../include/t.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int _FORVARNAME;
struct stack*pt;
int nice1();
int nice2(int num);
typedef struct __Potato_Struct__ Potato;
struct __Potato_Struct__ {
int x;
int y;
}
__Potato_Constructor__ (int x,int y){
Potato self;
self.x=x;
self.y=y;
return self;
}
int nice1(){
return 69;
}
int nice2(int num){
return num + 69;
}
int main(int argc,char**argv){
pt=newStack(4096);
Potato potato_1=__Potato_Constructor__(0,0);
Potato potato_2=__Potato_Constructor__(0,0);
print(String("Potato 1: "));
print(potato_1.x);
print(String(","));
println(potato_1.y);
print(String("Potato 2: "));
print(potato_2.x);
print(String(","));
println(potato_2.y);
}
