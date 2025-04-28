#include "../clib/cmv.h"
#include <stdio.h>

int main() {
    int *arr  = (int*)malloc(sizeof(int) * 10);
    int *arr2 = (int*)malloc(sizeof(int) * 10);

    if (arr) {
        arr[0] = 123;
        free(arr);
    }
    if(arr2) {
        free(arr2);
    }

    return 0;
}