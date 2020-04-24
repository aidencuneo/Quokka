#include "../headers/t.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int _FORVARNAME;
struct stack*pt;
int main(int argc,char**argv){pt=newStack(4096);string first=String("Hello");string second=String(",");string third=String(" World");string fourth=String("!");string cat=stringcat(first, second, third, fourth, "");println(cat.value);}