#include "../include/t.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int _FORVARNAME;
struct stack*pt;
int main(int argc,char**argv){pt=newStack(4096);string array[]={String("this"),String("is"),String("a"),String("string"),String("array"),String("\0")};for(_FORVARNAME;_FORVARNAME<arrsize(array);_FORVARNAME++){string s=array[_FORVARNAME];println(s);}}