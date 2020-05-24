#include <stdio.h>

int ptr()
{
    return 0;
}

int main()
{
    printf("%ld\n", sizeof(long long));
    printf("%ld\n", sizeof(long));
    printf("%ld\n", sizeof(int));

    printf("%ld\n", sizeof(ptr));
    printf("%ld\n", sizeof(&ptr));

    return 0;
}
