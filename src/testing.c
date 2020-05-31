#include <stdio.h>
#include <stdlib.h>

int main()
{
    char * ptr = malloc(16);
    ptr = realloc(ptr, 32);
    free(ptr);

    return 0;
}
