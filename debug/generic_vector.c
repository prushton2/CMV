#include <stdio.h>
// #include <stdlib.h>
#include "../clib/cmv.h"
#include "generic_vector.h"
#include "status.h"

struct generic_vector
{
	int size;
	int capacity;
	ITEM* data;
	ITEM (*init_copy)(ITEM);
	void (*destroy)(ITEM*);
};
typedef struct generic_vector Generic_vector;

GENERIC_VECTOR generic_vector_init_default(
	ITEM(*init_copy)(ITEM),
	void (*destroy)(ITEM*))
{
	Generic_vector* pVector;
	int i;
	pVector = malloc(sizeof(Generic_vector));
	if (pVector != NULL)
	{
		pVector->size = 0;
		pVector->capacity = 1;
		pVector->init_copy = init_copy;
		pVector->destroy = destroy;
		pVector->data = (ITEM*)malloc(sizeof(ITEM) * pVector->capacity);
		if (pVector->data == NULL)
		{
			free(pVector);
			return NULL;
		}
		for (i = 0; i < pVector->capacity; i++)
		{
			pVector->data[i] = NULL;
		}
	}
	return pVector;
}

Boolean generic_vector_is_empty(GENERIC_VECTOR hVector)
{
	Generic_vector* pVector = (Generic_vector*)hVector;

	return (Boolean)(pVector->size <= 0);
}


Status generic_vector_push_back(GENERIC_VECTOR hVector, ITEM hItem)
{
	Generic_vector* pVector = (Generic_vector*)hVector;
	ITEM* temp;
	int i;

	if (pVector->size >= pVector->capacity) //if there is not enough room then try to make room
	{
		temp = (ITEM*)malloc(sizeof(ITEM) * pVector->capacity * 2);
		if (temp == NULL)
		{
			return Failure;
		}
		for (i = 0; i < pVector->size; i++)
		{
			temp[i] = pVector->data[i]; //shallow copy
		}
		for (; i < pVector->capacity * 2; i++)
		{
			temp[i] = NULL;
		}
		free(pVector->data);
		pVector->data = temp;
		pVector->capacity *= 2;
	}

	//pVector->data[pVector->size] = hItem; //this is bad
	pVector->data[pVector->size] = pVector->init_copy(hItem);
	if (pVector->data[pVector->size] == NULL)
	{
		return Failure;
	}
	pVector->size++;
	return Success;
}


int generic_vector_get_size(GENERIC_VECTOR hVector)
{
	Generic_vector* pVector = (Generic_vector*)hVector;

	return pVector->size;
}

int generic_vector_get_capacity(GENERIC_VECTOR hVector)
{
	Generic_vector* pVector = (Generic_vector*)hVector;

	return pVector->capacity;
}


ITEM* generic_vector_at(GENERIC_VECTOR hVector, int index)
{
	Generic_vector* pVector = (Generic_vector*)hVector;
	if (index < 0 || index >= pVector->size)
	{
		return NULL;
	}
	return pVector->data + index;
}

Status generic_vector_pop_back(GENERIC_VECTOR hVector)
{
	Generic_vector* pVector = (Generic_vector*)hVector;

	if (generic_vector_is_empty(hVector))
	{
		return Failure;
	}
	pVector->destroy(&pVector->data[pVector->size - 1]);
	pVector->size--;
	return Success;
}

void generic_vector_destroy(GENERIC_VECTOR* phVector)
{
	Generic_vector* pVector = (Generic_vector*)*phVector;
	int i;

	for (i = 0; i < pVector->size; i++)
	{
		pVector->destroy(&pVector->data[i]);
	}
	
	free(pVector->data);
	free(pVector);
	*phVector = NULL;
}
