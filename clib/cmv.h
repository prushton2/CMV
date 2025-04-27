#ifndef CMV_H
#define CMV_H

#include <stddef.h>

struct encoder {
    char * encoder_value_str;	
};

#define malloc(size) debug_malloc(size)
#define realloc(ptr, size) debug_realloc(ptr, size);
#define free(ptr) debug_free(ptr)

void* debug_malloc(size_t size);
void* debug_realloc(void* ptr, size_t size);
void  debug_free(void* ptr);
void initMem();

#endif