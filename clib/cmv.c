#define _POSIX_C_SOURCE 200112L // Or 200809L for newer systems
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define SIZE 1024

void writeMem(char type, void* address, size_t size) {
    int fd = open("../cmv", O_RDWR | O_CREAT, 0666);
    char* shm = (char*)mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    char buffer[16];
    snprintf(buffer, sizeof(buffer), "%p", address);
    
    memcpy(shm+4, &type, 1);
    memcpy(shm+5, &buffer, 16);
    memcpy(shm+20, &size, 4);
 
    close(fd);
}

void* debug_malloc(size_t size) {
    #undef malloc
    void* ptr = malloc(size);
    #define malloc(size) debug_malloc(size)
    if (ptr == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    writeMem('a', ptr, size);

    return ptr;
}

void* debug_realloc(void* ptr, size_t size) {
    #undef realloc
    ptr = realloc(ptr, size);
    #define realloc(ptr, size) debug_realloc(ptr, size)
    if (ptr == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    writeMem('a', ptr, size);

    return ptr;
}


void debug_free(void* ptr) {
    if (ptr == NULL) {
        fprintf(stderr, "Attempted to free a NULL pointer\n");
        return;
    }
    
    writeMem('f', ptr, 0);

    #undef free
    free(ptr);
    #define free(ptr) debug_free(ptr)

}