#include "../include/t.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int _FORVARNAME;
typedef struct __Potato_Struct__ Potato;
struct __Potato_Struct__ {
int x;
int y;
void (*summary)(Potato self);
};
typedef struct __Point_Struct__ Point;
struct __Point_Struct__ {
int x;
int y;
void (*summary)(Point self);
};
int n();
int m();
int o();
int oo();
int oooo();
void __summary_Potato__(Potato self){
print(self.x);
print(String(","));
println(self.y);
}
void __summary_Point__(Point self){
print(self.x);
print(String(","));
println(self.y);
}
Potato __Potato_Constructor__ (int x,int y){
Potato self;
self.x=x;
self.y=y;
self.summary=__summary_Potato__;
return self;}
Point __Point_Constructor__ (int x,int y){
Point self;
self.x=x;
self.y=y;
self.summary=__summary_Point__;
return self;}
int n(){
}
int m(){
}
int o(){
}
int oo(){
}
int oooo(){
}
int main(int argc,char**argv){
Potato potato_1=__Potato_Constructor__(950,450);
Potato potato_2=__Potato_Constructor__(960,450);
potato_1.summary(potato_1);
potato_2.summary(potato_2);
Point point_1=__Point_Constructor__(53,68);
Point point_2=__Point_Constructor__(67,43);
point_1.summary(point_1);
point_2.summary(point_2);
}
