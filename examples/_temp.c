#include "../include/t.h"

int _FORVARNAME;
string _Q_strcat(string word,integer count);
string _Q_strcat(string word,integer count){
string out;
for(int _=0;_<count.value;_+=1){
out=out.__add__(out,word);
}
return out;
}
int main(int argc,char**argv){
println(_Q_strcat(__string_Constructor__("aiden") , __integer_Constructor__(3)));
}
