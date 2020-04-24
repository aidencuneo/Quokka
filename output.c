#include "t.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int _FORVARNAME;
struct stack*pt;
int main(int argc,char**argv){pt=newStack(4096);string hello=String("   Hello, World!     ");hello=stringstrip(hello);print(hello.value);println(String("EOF"));}