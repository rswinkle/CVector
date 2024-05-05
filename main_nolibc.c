
/* Try sbrk and mmap */
#include <unistd.h>
#include <sys/mman.h>

void* mymalloc(unsigned long sz)
{
	return sbrk(sz);

	/*
	long pagesize = sysconf(_SC_PAGESIZE);
	return mmap(NULL, sz, PROT_READ|PROT_WRITE, MAP_ANONYMOUS, -1, 0);
	*/
}

void myfree(void* addr)
{
	/* no-op for sbrk based system */

	/*
	Too lazy to bother for a compilation test
	return munmap(NULL, sz);
	*/
}

void* myrealloc(void* addr, unsigned long sz)
{
	unsigned long i;
	unsigned char* to, *from;
	void* newaddr = mymalloc(sz);
	if (!newaddr) {
		return NULL;
	}
	to = newaddr;
	from = addr;

	for (i=0; i<sz; i++) {
		to[i] = from[i];
	}
	myfree(addr);
	return newaddr;
}

#define CVEC_MALLOC(sz)      mymalloc(sz)
#define CVEC_REALLOC(p, sz)  myrealloc(p, sz)
#define CVEC_FREE(p)         myfree(p)

#define CVEC_SIZE_T long
#define CVECTOR_IMPLEMENTATION
#include "cvector.h"



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


	/*
	THIS IS REALLY JUST A compilation test to see/prove it really doesn't
	depend on the C stdlib
	functionality testing is done in main.c and cvector_tests.c
	*/

	cvec_free_i(&veci);
	cvec_free_d(&vecd);
	cvec_free_str(&vecstr);
	cvec_free_void(&vecvoid);



	return 0;
}


