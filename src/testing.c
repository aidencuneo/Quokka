#include "../include/q.h"

int main()
{
    stringArr mylist;

    mylist.data[0] = String("aidennnn");
    mylist.data[1] = String("tariyaaa");
    mylist.size = 2;

    for (int i = 0; i < mylist.size; i++)
        println(mylist.data[i]);

    return 0;
}
