

#define VECTOR_short_IMPLEMENTATION
#include "vector_short.h"

#define VECTOR_f_struct_IMPLEMENTATION
#include "vector_f_struct.h"

#include <iostream>

char* mystrdup(const char* str)
{
	
	size_t len = strlen(str);
	char* temp = (char*)calloc(len+1, sizeof(char));
	if (!temp) {
		assert(temp != NULL);
		return NULL;
	}
	
	return (char*)memcpy(temp, str, len);  
}

void free_f_struct(void* tmp)
{
	f_struct* f = (f_struct*)tmp;
	if (f->word) {
		free(f->word);
		f->word = NULL;
	}
}


void init_f_struct(void* dest, void* src)
{
	f_struct* d = (f_struct*)dest;
	f_struct* s = (f_struct*)src;
	d->i = s->i;
	d->d = s->d;
	d->word = mystrdup(s->word);
}

using namespace std;

int main(int argc, char** argv)
{
	vector_short vshort;
	vector_f_struct vf_struct;

	vec_short(&vshort, 0, 10);
	vec_f_struct(&vf_struct, 0, 10, free_f_struct, init_f_struct);


	//THIS IS REALLY JUST A C++ compilation test
	//functionality testing is done in vector_tests.h
	//and vector_tests2.h


	free_vec_short(&vshort);
	free_vec_f_struct(&vf_struct);


	return 0;
}

