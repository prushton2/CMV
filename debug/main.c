#include <stdio.h>
#include <stdlib.h>
#include "./my_string.h"
#include "./generic_vector.h"
#include "./status.h"

int main(int argc, char* argv[]) {
	
	//Load words into giant ass array
	GENERIC_VECTOR vec[33] = {
		generic_vector_init_default(&my_string_init_copy, &my_string_destroy),
		generic_vector_init_default(&my_string_init_copy, &my_string_destroy),
		generic_vector_init_default(&my_string_init_copy, &my_string_destroy),
		generic_vector_init_default(&my_string_init_copy, &my_string_destroy),
		generic_vector_init_default(&my_string_init_copy, &my_string_destroy),
		generic_vector_init_default(&my_string_init_copy, &my_string_destroy),
		generic_vector_init_default(&my_string_init_copy, &my_string_destroy),
		generic_vector_init_default(&my_string_init_copy, &my_string_destroy),
		generic_vector_init_default(&my_string_init_copy, &my_string_destroy),
		generic_vector_init_default(&my_string_init_copy, &my_string_destroy),
		generic_vector_init_default(&my_string_init_copy, &my_string_destroy),
		generic_vector_init_default(&my_string_init_copy, &my_string_destroy),
		generic_vector_init_default(&my_string_init_copy, &my_string_destroy),
		generic_vector_init_default(&my_string_init_copy, &my_string_destroy),
		generic_vector_init_default(&my_string_init_copy, &my_string_destroy),
		generic_vector_init_default(&my_string_init_copy, &my_string_destroy),
		generic_vector_init_default(&my_string_init_copy, &my_string_destroy),
		generic_vector_init_default(&my_string_init_copy, &my_string_destroy),
		generic_vector_init_default(&my_string_init_copy, &my_string_destroy),
		generic_vector_init_default(&my_string_init_copy, &my_string_destroy),
		generic_vector_init_default(&my_string_init_copy, &my_string_destroy),
		generic_vector_init_default(&my_string_init_copy, &my_string_destroy),
		generic_vector_init_default(&my_string_init_copy, &my_string_destroy),
		generic_vector_init_default(&my_string_init_copy, &my_string_destroy),
		generic_vector_init_default(&my_string_init_copy, &my_string_destroy),
		generic_vector_init_default(&my_string_init_copy, &my_string_destroy),
		generic_vector_init_default(&my_string_init_copy, &my_string_destroy),
		generic_vector_init_default(&my_string_init_copy, &my_string_destroy),
		generic_vector_init_default(&my_string_init_copy, &my_string_destroy),
		generic_vector_init_default(&my_string_init_copy, &my_string_destroy),
		generic_vector_init_default(&my_string_init_copy, &my_string_destroy),
		generic_vector_init_default(&my_string_init_copy, &my_string_destroy),
		generic_vector_init_default(&my_string_init_copy, &my_string_destroy)
	};

	FILE* fp = fopen("dictionary.txt", "r");
	
	while(1) {
		char str[32] = "";
		
		
		fgets(str, 32, fp);
		//printf("%s\n", str);
		int finalchar = fgetc(fp);
		if(finalchar == -1) {
			break;
		}

		//printf("|%c|", finalchar);
		MY_STRING str2 = my_string_init_c_string(str);
		MY_STRING str1 = my_string_init_c_string((char*)&finalchar);
		my_string_concat(str1, str2);
		my_string_pop_back(str1);
		generic_vector_push_back(vec[my_string_get_size(str1)], str1);

		my_string_destroy(&str1);
		my_string_destroy(&str2);
		
	}


	fclose(fp);

	for(int i=0; i<32; i++) {
		printf("%d: %d\n", i, generic_vector_get_size(vec[i]));
	}

	for(int i = 0; i<33; i++) {
		generic_vector_destroy(&vec[i]);
	}
	return 0;
}
