#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>



void* debug_malloc(size_t size) {
    void* ptr = malloc(size); // This will recursively call the macro, so use `#undef` temporarily.
    #undef malloc
    ptr = malloc(size);
    #define malloc(size) debug_malloc(size)
    if (ptr == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    printf("Allocated memory at address: %p\n", ptr);
    return ptr;
}


void debug_free(void* ptr) {
    if (ptr == NULL) {
        fprintf(stderr, "Attempted to free a NULL pointer\n");
        return;
    }
    printf("Freeing memory at address: %p\n", ptr);
    #undef free
    free(ptr); // Use the original free function
    #define free(ptr) debug_free(ptr)
}