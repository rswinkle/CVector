

#define CVECTOR_IMPLEMENTATION
#include "cvector.h"


#include <iostream>


using namespace std;

int main(int argc, char** argv)
{
	cvector_i veci;
	cvector_d vecd;
	cvector_str vecstr;
	cvector_void vecvoid;


	cvec_i(&veci, 0, 10);
	cvec_d(&vecd, 0, 10);
	cvec_str(&vecstr, 0, 10);
	cvec_void(&vecvoid, 0, 10, sizeof(long double), NULL, NULL);



	//THIS IS REALLY JUST A C++ compilation test
	//functionality testing is done in cvector_tests.h

	cvec_free_i(&veci);
	cvec_free_d(&vecd);
	cvec_free_str(&vecstr);
	cvec_free_void(&vecvoid);



	return 0;
}

