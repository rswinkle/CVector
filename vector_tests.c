#include "vector.h"
#include <CUnit/Automated.h>

void pushi_test()
{
	vector_i* vec = vec_i(0);

	CU_ASSERT_EQUAL(VEC_I_START_SZ, vec->capacity);
	CU_ASSERT_EQUAL(0, vec->size);


	int i=0;
	for(i=0; i<100; i++)
		push_backi(vec, i);

	CU_ASSERT_EQUAL(100, vec->size);

	for(i=0; i<vec->size; i++)
		CU_ASSERT_EQUAL(i, vec->a[i]);

	free_veci(vec);
}


void erasei_test()
{
	vector_i* vec = vec_i(100);

	CU_ASSERT_EQUAL(VEC_I_START_SZ+100, vec->capacity);
	CU_ASSERT_EQUAL(100, vec->size);


	int i=0;
	for(i=0; i<100; i++)
		push_backi(vec, i);

	CU_ASSERT_EQUAL(100, vec->size);

	erasei(vec, 25, 74);

	CU_ASSERT_EQUAL(50, vec->size);

	int j=0;
	for(i=0; i<vec->size; i++,j++) {
		CU_ASSERT_EQUAL(j, vec->a[i]);
		if(i==24) j +=50;
	}

	free_veci(vec);
}



void inserti_test()
{
	int array[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	vector_i* vec = init_vec_i(array, 10);

	CU_ASSERT_EQUAL(vec->size, 10);


	int i;
	for(i=0; i<vec->size; i++)
		CU_ASSERT_EQUAL(vec->a[i], i);

	for(i=0; i<10; i++)
		inserti(vec, 0, -i);

	CU_ASSERT_EQUAL(vec->size, 20);

	for(i=0; i<vec->size; i++)
		CU_ASSERT_EQUAL(vec->a[i], i-((i<10)? 9 : 10) );

	free_veci(vec);
}



void popi_test()
{
	vector_i* vec = init_vec_i(NULL, 10);

	CU_ASSERT_EQUAL(vec->capacity, 10+VEC_I_START_SZ);

	int i;
	for(i=0; i<1000; i++)
		push_backi(vec, i);

	CU_ASSERT_EQUAL(vec->size, 1000);

	int temp;
	for(i=999; i>=0; i++) {
		temp = pop_backi(vec);
		CU_ASSERT_EQUAL(temp, i);
	}

	CU_ASSERT_EQUAL(vec->size, 0);

	free_veci(vec);
}



void reservei_test()
{
	vector_i* vec = vec_i(0);

	reservei(vec, 20);
	CU_ASSERT( vec->capacity>=20 );

	free_veci(vec);
}




void set_vali_test()
{
	vector_i* vec = vec_i(20);

	set_val_szi(vec, 42);
	int i;
	for(i=0; i<vec->size; i++)
		CU_ASSERT_EQUAL(vec->a[i], 42);

	set_val_capi(vec, 25);

	for(i=0; i<vec->capacity; i++)
		CU_ASSERT_EQUAL(vec->a[i], 25);

	free_veci(vec);
}































