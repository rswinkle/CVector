

#define CVECTOR_IMPLEMENTATION
#include "cvector.h"


#include <iostream>


using namespace std;

int main(int argc, char** argv)
{
	vector_i veci;
	vector_d vecd;
	vector_str vecstr;
	vector_void vecvoid;


	vec_i(&veci, 0, 10);
	vec_d(&vecd, 0, 10);
	vec_str(&vecstr, 0, 10);
	vec_void(&vecvoid, 0, 10, sizeof(long double), NULL, NULL);



	//THIS IS REALLY JUST A C++ compilation test
	//functionality testing is done in vector_tests.h
	//and vector_tests2.h

	free_vec_i(&veci);
	free_vec_d(&vecd);
	free_vec_str(&vecstr);
	free_vec_void(&vecvoid);



	return 0;
}

