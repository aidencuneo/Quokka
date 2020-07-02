#include <stdio.h>

int main()
{
    unsigned int arr[2];
    unsigned int * arrptr = arr;
    arrptr[0] = 19;
    arrptr[1] = 21;
    arrptr[2] = 23;
    // printf("%d %d\n", arr[0], arr[1]);

    return 0;
}
