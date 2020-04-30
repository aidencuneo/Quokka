#include "../include/t.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int _FORVARNAME;
typedef struct __Person_Struct__ Person;
struct __Person_Struct__ {
string name ;string id ;void (*show)(Person self);
};
void __show_Person__(Person self){
println(self.name.__add__(self.name,(String(", ID: ")).__add__(String(", ID: "),self.id)));
}
Person __Person_Constructor__(string name){
Person self;
self.name=name;
self.id=__string_Constructor__(name.length.__mul__(name.length,(Integer(5)).__add__(Integer(5),Integer(1))));
;
self.show=__show_Person__;
return self;}
int main(int argc,char**argv){
print((String(String("Please enter a name: "))));
Person person=__Person_Constructor__(input());
;
person.show(person);println((person.name.__add__(person.name,String(" is this person's name"))).__add__(person.name.__add__(person.name,String(" is this person's name")),String(String("."))));
println(String("Bunch of gibberish. ").__add__(String("Bunch of gibberish. "),((person.name)).__add__(person.name,String(String(".")))));
}
