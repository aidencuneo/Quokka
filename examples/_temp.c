#include "../include/q.h"

int _FORVARNAME;
string strgetch(string s,integer i);
integer file_last_modified(string path);
string strexp(string st,integer exp);
string construct_func(string name,string args);
string strgetch(string s,integer i){
return String(s.value[i.value]);
}
integer file_last_modified(string path){
if (getrealpath(path.value) == 0) return Integer(0);struct stat attr;stat(path.value, &attr);struct tm * gmt = gmtime(&(attr.st_ctime));struct tm t = {0};t.tm_year = gmt->tm_year - 1900;t.tm_mon = gmt->tm_mon - 1;t.tm_mday = gmt->tm_mday;t.tm_hour = gmt->tm_hour;t.tm_min = gmt->tm_min;t.tm_sec = gmt->tm_sec;time_t timeSinceEpoch = mktime(&t);return Integer(timeSinceEpoch);
}
string strexp(string st,integer exp){
string out=__string_Constructor__("");
for(int _=0;_<exp.value;_+=1){
out=out.__add__(out,st);
}
return out;
}
string construct_func(string name,string args){
args=args.slice(args,__integer_Constructor__(1),__integer_Constructor__(__integer_Constructor__(__integer_Constructor__(1))));
println(name);
println(args);
}
int main(int argc,char**argv){
string function=construct_func(__string_Constructor__("bob") , __string_Constructor__("(50, 60, 70)"));
}
