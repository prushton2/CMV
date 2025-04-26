#include "../clib/cmv.h"
#include <stdio.h>

int main() {
    printf("AAAA\n");
    int *arr = (int *)malloc(sizeof(int) * 10);
    if (arr) {
        arr[0] = 123;
        printf("Value: %d\n", arr[0]);
        free(arr);
    }
    return 0;
}