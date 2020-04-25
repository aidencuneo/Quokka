#include "../headers/t.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int _FORVARNAME;
struct stack*pt;
int main(int argc,char**argv){pt=newStack(4096);string str=String("The first <animal> \'looked at the second <animal>.");println(str.value);string rep=String("<animal>");string with=String("cat");string nstr=String(stringreplace(str, rep, with));println(nstr.value);}