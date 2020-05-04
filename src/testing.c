#include "../include/q.h"

object func(object var)
{
    if (!strcmp(var.name, "string"))
        return String("S");
    else if (!strcmp(var.name, "integer"))
        return String("I");
    else if (!strcmp(var.name, "bool"))
        return String("B");
    else
        return String("?");
}

int main()
{
    object num = Integer(92130828);
    object str = String("This is a string, but also an object.");
    object boo = Bool(0);

    print(num.name);
    print(" : ");
    println(num);

    print(str.name);
    print("  : ");
    println(str);

    print(boo.name);
    print("    : ");
    println(boo);
}
