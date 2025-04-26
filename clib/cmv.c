#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

int writeFile(char type, void* address, size_t size) {
    FILE* file = fopen("cmv", "w");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file 'cmv'\n");
        return -1;
    } else {
        fclose(file); // Clear the file by opening in write mode and immediately closing
    }

    file = fopen("cmv", "a");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file 'cmv'\n");
        return -1;
    }
    fprintf(file, "%c-%p-%u", type, address, size);
    fclose(file);
}

void* debug_malloc(size_t size) {
    #undef malloc
    void* ptr = malloc(size);
    #define malloc(size) debug_malloc(size)
    if (ptr == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    writeFile('a', ptr, size);

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

    writeFile('f', ptr, 0);

}