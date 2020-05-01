#include "../include/t.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int _FORVARNAME;
typedef struct __reverse_int_Struct__ reverse_int;
struct __reverse_int_Struct__ {
integer val ;bool (*__bool__)(reverse_int self);
reverse_int (*__add__)(reverse_int self,integer other);
reverse_int (*__sub__)(reverse_int self,integer other);
};
reverse_int __reverse_int_Constructor__(integer value);
bool ____bool___reverse_int__(reverse_int self){
if(((self.val).__bool__(self.val)).value.value){
return __bool_Constructor__(1);
}
return __bool_Constructor__(0);
}
reverse_int ____add___reverse_int__(reverse_int self,integer other){
return __reverse_int_Constructor__(self.val.__sub__(self.val,other));
;
}
reverse_int ____sub___reverse_int__(reverse_int self,integer other){
return __reverse_int_Constructor__(self.val.__add__(self.val,other));
;
}
reverse_int __reverse_int_Constructor__(integer value){
reverse_int self;
self.val=value;
self.__bool__=____bool___reverse_int__;
self.__add__=____add___reverse_int__;
self.__sub__=____sub___reverse_int__;
return self;}
int main(int argc,char**argv){
reverse_int num=__reverse_int_Constructor__(__integer_Constructor__(20000));
;
while(((num).__bool__(num)).value.value){
println(num.val);
num=num.__add__(num,__integer_Constructor__(1));
}
}
