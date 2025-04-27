#include "./my_string.h"

// #include <stdlib.h>
#include "../clib/cmv.h"
#include <stdio.h>
#include <string.h>

struct my_string {
	int size;
	int capacity;
	char* data;
};

typedef struct my_string* MY_STRING;

MY_STRING my_string_init_default() {
	
	MY_STRING pMyString = NULL;
	pMyString =(MY_STRING) malloc(sizeof(struct my_string));
	pMyString->capacity = 32;
	pMyString->size = 0;
	pMyString->data = (char*)malloc(sizeof(char) * pMyString->capacity);

	return pMyString;
}



MY_STRING my_string_init_c_string(const char* c_string) {
	
	int i;

	for(i = 0; c_string[i] != '\0'; i++) {}

	MY_STRING pMyString = NULL;
	pMyString =(MY_STRING) malloc(sizeof(struct my_string));
	pMyString->capacity = i+1;
	pMyString->size = i+1;
	pMyString->data = (char*)malloc(sizeof(char) * pMyString->capacity);
	memcpy(pMyString->data, c_string, i); //*c_string;
	memset(pMyString->data+i, '\0', 1);

	return pMyString;
}


void my_string_destroy(MY_STRING* phMy_string) {

	struct my_string* pMyString = (struct my_string*) *phMy_string;

	free(pMyString->data);
	free(pMyString);
	*phMy_string = NULL;

}

int my_string_compare(MY_STRING hLeft, MY_STRING hRight) {

	int max;


	if(hLeft->size > hRight->size) {
		return 2;
	} else if (hLeft->size < hRight->size) {
		return -2;
	
	}
	max = hRight->size;

	for(int i = 0; i<max; i++) {
	
		if (hLeft->data[i] > hRight->data[i]) {
			return 1;
		}
		else if (hLeft->data[i] < hRight->data[i]) {
			return -1;
		}	
	}

	return 0;
}


int my_string_get_capacity(MY_STRING hMy_string) {
	
	return ((struct my_string*)hMy_string)->capacity;
}

int my_string_get_size(MY_STRING hMy_string) {

	return ((struct my_string*)hMy_string)->size-1;
}


Status my_string_extraction(MY_STRING hMy_string, FILE* fp) {
	char string[128];
	int len = 0;
	char cChar = ' ';

	while (cChar == ' ') {
		cChar = fgetc(fp);
	}

	if (cChar == EOF) {
		return Failure;
	}

	while(cChar != ' ' && cChar != '\n') {
		
		if(cChar == EOF) {
			break;
		}
		
		string[len] = cChar;
		string[len+sizeof(char)] = '\0';
		len++;
		//www.youtube.com/watch?v=i8V1MSuDmUg
		cChar = fgetc(fp);	
	}
	
	fseek(fp, -1, SEEK_CUR);
	
	hMy_string->capacity = len;
	hMy_string->size = len;
	
	free(hMy_string->data);
	hMy_string->data = (char*) malloc(len * sizeof(char));
	memcpy(hMy_string->data, string, len);

	return Success;
}


Status my_string_insertion(MY_STRING hMy_string, FILE* fp) {
	
	fputs(hMy_string->data, fp);
	
	return Success;

}

Status my_string_push_back(MY_STRING hMy_string, char item) {
	if(hMy_string == NULL) {
		return Failure;
	}

	if(hMy_string->data == NULL) {
		return Failure;
	}

	hMy_string->capacity++;
	hMy_string->data = realloc(hMy_string->data, hMy_string->capacity + 1);
	memset(hMy_string->data+hMy_string->size-1, item, 1);
	hMy_string->size++;
	memset(hMy_string->data+hMy_string->size-1, '\0', 1);
	return Success;
}

Status my_string_pop_back(MY_STRING hMy_string) {
	if (hMy_string->size <= 1) {
		return Failure;
	}
	hMy_string->capacity--;
	memset(hMy_string->data+hMy_string->size-2, '\0', 1);
	hMy_string->size--;
	return Success;
}

char* my_string_at(MY_STRING hMy_string, int index) {
	
	if(hMy_string == NULL) {
		return NULL;
	}

	if(index > hMy_string->size || index < 0) {
		return NULL;
	}

	return &hMy_string->data[index];
}

char* my_string_get_data(MY_STRING hMy_string) {
	return hMy_string->data;

}

Status my_string_concat(MY_STRING result, MY_STRING append) {
	result->capacity = result->size + append->size;

	char* newData = (char*)malloc(sizeof(char) * result->capacity);
	memcpy(newData, result->data, result->size);
	memcpy(newData + result->size-1, append->data, append->size);

	result->size += append->size-1;
	
	free(result->data);

	result->data = newData;

	return Success;
}

Boolean my_string_empty(MY_STRING hMy_String) {
	if (hMy_String->size == 1) {
		return TRUE;
	}
	return FALSE;
}

Status my_string_assignment(MY_STRING hLeft, MY_STRING hRight) {
	if (hLeft == NULL || hRight == NULL)
		return Failure;
	if (hLeft->data == NULL || hRight->data == NULL)
		return Failure;

	hRight->capacity += 1;

	hLeft->data = (char*)realloc(hLeft->data, sizeof(char) * hRight->capacity);

	int i = hRight->size;
	memcpy(hLeft->data, my_string_get_data(hRight), i); //*c_string;
	memset(hLeft->data+i, '\0', 1);
	
	hLeft->size = hRight->size;
	hLeft->capacity = hRight->capacity;

	return Success;
}


MY_STRING my_string_init_copy(MY_STRING hMy_string) {
	if(hMy_string == NULL) 
		return Failure;
	if(hMy_string->data == NULL)
		return Failure;

	MY_STRING pNew = (MY_STRING)malloc(sizeof(struct my_string));

	pNew->size = hMy_string->size;
	pNew->capacity = hMy_string->capacity;

	pNew->data = (char*)malloc(sizeof(char) * pNew->size+1);
	memcpy(pNew->data, hMy_string->data, pNew->size);
	memset(pNew->data+pNew->size, '\0', 1);

	return pNew;
}

void my_string_swap(MY_STRING hLeft, MY_STRING hRight) {
	if (hLeft == NULL || hRight == NULL)
		return;
	if (hLeft->data == NULL || hRight->data == NULL)
		return;

	MY_STRING temp = my_string_init_copy(hLeft);
	my_string_assignment(hLeft, hRight);
	my_string_assignment(hRight, temp);
	my_string_destroy(&temp);
}
