

#define VECTOR_short_IMPLEMENTATION
#include "vector_short.h"

#define VECTOR_f_struct_IMPLEMENTATION
#include "vector_f_struct.h"

#include "vector_all.h"

#include <iostream>


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
	vector_i veci;
	vector_d vecd;
	vector_str vecstr;
	vector_void vecvoid;

	vector_short vshort;
	vector_f_struct vf_struct;

	vec_i(&veci, 0, 10);
	vec_d(&vecd, 0, 10);
	vec_str(&vecstr, 0, 10);
	vec_void(&vecvoid, 0, 10, sizeof(long double), NULL, NULL);


	vec_short(&vshort, 0, 10);
	vec_f_struct(&vf_struct, 0, 10, free_f_struct, init_f_struct);



	//THIS IS REALLY JUST A C++ compilation test
	//functionality testing is done in vector_tests.h
	//and vector_tests2.h

	free_vec_i(&veci);
	free_vec_d(&vecd);
	free_vec_str(&vecstr);
	free_vec_void(&vecvoid);

	free_vec_short(&vshort);
	free_vec_f_struct(&vf_struct);


	return 0;
}

