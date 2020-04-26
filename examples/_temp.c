#include "../include/t.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int _FORVARNAME;
struct stack*pt;
int main(int argc,char**argv){pt=newStack(4096);string * array=(string *)malloc(2 * sizeof(string));array[0]=String("\0");sarrappend(array,String("aiden"),String("another word"),String("\0"));println(array);println(arrsize(array));println(array);println(arrsize(array));}