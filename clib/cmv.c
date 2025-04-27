#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

char* shm = NULL;

void initMem() {
    int fd = open("cmv", O_RDWR | O_CREAT, 0666);
    ftruncate(fd, 32);
    shm = (char*)mmap(NULL, 32, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
}

void writeMem(char type, void* address, size_t size) {
    if(shm == NULL) {
        fprintf(stderr, "Place `initMem()` at the beginning of your main function\n");
        exit(1);
    }

    char f[32] = "--------------------------------";

    char buffer[16];
    snprintf(buffer, sizeof(buffer), "%p", address);

    memcpy(shm, &f, 32);
    memcpy(shm, &type, 1);
    memcpy(shm+1, &buffer, 16);
    memcpy(shm+16, &size, 4);
    // 0x5555555592a0
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
    // *((int *)shared_ptr) = 1;

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

    printf("%c", shm[0]);

    #undef free
    free(ptr);
    #define free(ptr) debug_free(ptr)

}