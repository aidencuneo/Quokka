#include "../include/t.h"

typedef struct A_Struct A;
struct A_Struct
{
    int a;
    int b;
};

A A_Constructor(int a, int b)
{
    A out;
    out.a = a;
    out.b = b;
    return out;
}

int main()
{
    A item = A_Constructor(5, 7);
    println(item.a);
    println(item.b);
}
