#include "../headers/t.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int _FORVARNAME;
struct stack*pt;
int main(int argc,char**argv){pt=newStack(4096);string array[]={String("word number one"),String("\0")};println(array);println(sarradd(array,String("word number two")));}