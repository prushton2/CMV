#ifndef CMV_H
#define CMV_H

#include <stddef.h>

#define malloc(size) debug_malloc(size)
#define free(ptr) debug_free(ptr)

// Include necessary headers
void* debug_malloc(size_t size);
void debug_free(void* ptr);

#endif