#ifndef MY_STRING_H
#define MY_STRING_H

#include <stdio.h>
#include "./status.h"

struct my_string;
typedef struct my_string* MY_STRING;

//Postcondition: Allocate space for a string object that represents the empty
// string. The string will have capacity 7 and size 0 by default. A copy of
// the address of the opaque object will be returned on success and NULL on
// failure.
MY_STRING my_string_init_default(void);
//Precondition: phMy_string holds the address of a valid handle to a MY_STRING
// OBject.
//Postcondition: The memory used for the MY_STRING object has be reclaimed by
// the system and the handle referred to by the pointer phMy_string has been
// set to NULL.
void my_string_destroy(MY_STRING* phMy_string);

int my_string_compare(MY_STRING hLeft_string, MY_STRING hRight_string);

MY_STRING my_string_init_c_string(const char* c_string);

//Precondition: hMy_string is the handle of a valid My_string object.
//Postcondtion: Returns a copy of the integer value of the object's capacity.
int my_string_get_capacity(MY_STRING hMy_string);
//Precondition: hMy_string is the handle of a valid My_string object.
//Postcondtion: Returns a copy of the integer value of the object's size.
int my_string_get_size(MY_STRING hMy_string);

Status my_string_extraction(MY_STRING hMy_string, FILE *fp);

Status my_string_insertion(MY_STRING hMy_string, FILE *fp);



Status my_string_push_back(MY_STRING hMy_string, char item);

Status my_string_pop_back(MY_STRING hMy_string);

char* my_string_at(MY_STRING hMy_string, int index);

char* my_string_c_str(MY_STRING hMy_string);

Status my_string_concat(MY_STRING hResult, MY_STRING hAppend);

Boolean my_string_empty(MY_STRING hMy_string);

char* my_string_get_data(MY_STRING hMy_string);

Status my_string_assignment(MY_STRING hLeft, MY_STRING hRight);
MY_STRING my_string_init_copy(MY_STRING hMy_string);
void my_string_swap(MY_STRING hLeft, MY_STRING hRight);

#endif
