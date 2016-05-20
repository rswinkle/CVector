

#define CVECTOR_short_IMPLEMENTATION
#include "cvector_short.h"

#define CVECTOR_f_struct_IMPLEMENTATION
#include "cvector_f_struct.h"

#include "cvector_all.h"

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
	cvector_i veci;
	cvector_d vecd;
	cvector_str vecstr;
	cvector_void vecvoid;

	cvector_short vshort;
	cvector_f_struct vf_struct;

	cvec_i(&veci, 0, 10);
	cvec_d(&vecd, 0, 10);
	cvec_str(&vecstr, 0, 10);
	cvec_void(&vecvoid, 0, 10, sizeof(long double), NULL, NULL);


	cvec_short(&vshort, 0, 10);
	cvec_f_struct(&vf_struct, 0, 10, free_f_struct, init_f_struct);



	//THIS IS REALLY JUST A C++ compilation test
	//functionality testing is done in vector_tests.h
	//and vector_tests2.h

	cvec_free_i(&veci);
	cvec_free_d(&vecd);
	cvec_free_str(&vecstr);
	cvec_free_void(&vecvoid);

	cvec_free_short(&vshort);
	cvec_free_f_struct(&vf_struct);


	return 0;
}

