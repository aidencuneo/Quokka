#include "../include/t.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int _FORVARNAME;
typedef struct __StringList_Struct__ StringList;
struct __StringList_Struct__ {
string value[1];
int length;
};
StringList __StringList_Constructor__(){
StringList self;
return self;}
int main(int argc,char**argv){
println(String("Started."));
StringList mylist=__StringList_Constructor__();
println(String("Success."));
}
