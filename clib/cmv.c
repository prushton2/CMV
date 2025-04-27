#include "cmv.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

struct encoder* shmPtr;

int init_mem_pointer() {
    int shmid;
    // unsigned int encoder_value[8];
    // char temp0[5], temp1[5], temp2[5], temp3[5], temp4[5], temp5[5], temp6[5], temp7[5];
	
	// struct encoder* shmPtr;
	// struct encoder dummy[8];

    int key = 12;

	/** shmget returns a identifier in shmid **/
	if((shmid = shmget(key, 2048 , 0666|IPC_CREAT)) < 0)            //1024
	{
		printf("Error while getting shared memeory id");
		exit(1);	
	}

	/** shmat attaches to shared memeory **/
	if((shmPtr = (struct encoder *)shmat(shmid, (void*)0, 0)) == (char *) -1)
	{
		printf("Error attaching shared memeory id");
		exit(1);
	}
}

int writeMem(char type, void* address, size_t size) {
    // FILE* file = fopen("cmv", "w");
    // if (file == NULL) {
    //     // fprintf(stderr, "Failed to open file 'cmv'\n");
    //     return 1;
    // } 
    // else {
    //     fclose(file); // Clear the file by opening in write mode and immediately closing
    // }

    // file = fopen("cmv", "a");
    // // *((int *)shared_ptr) = 1;

    // fprintf(file, "%c-%p-%lu", type, address, size);
    // fclose(file);


 

    return 0;
}

void* debug_malloc(size_t size) {
    #undef malloc
    void* ptr = malloc(size);
    #define malloc(size) debug_malloc(size)

    writeFile('a', ptr, size);

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

    writeFile('a', ptr, size);

    return ptr;
}


void debug_free(void* ptr) {
    if (ptr == NULL) {
        fprintf(stderr, "Attempted to free a NULL pointer\n");
        return;
    }
    
    writeFile('f', ptr, 0);

    #undef free
    free(ptr);
    #define free(ptr) debug_free(ptr)

}