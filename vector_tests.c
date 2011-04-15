#include "vector.h"
#include <CUnit/Automated.h>


/* vector_i tests */
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





/* vector_d tests */
void pushd_test()
{
	vector_d* vec = vec_d(0);

	CU_ASSERT_EQUAL(VEC_D_START_SZ, vec->capacity);
	CU_ASSERT_EQUAL(0, vec->size);


	int i=0;
	for(i=0; i<100; i++)
		push_backd(vec, i+0.5);

	CU_ASSERT_EQUAL(100, vec->size);

	for(i=0; i<vec->size; i++)
		CU_ASSERT_EQUAL(i+0.5, vec->a[i]);

	free_vecd(vec);
}


void erased_test()
{
	vector_d* vec = vec_d(100);

	CU_ASSERT_EQUAL(VEC_D_START_SZ+100, vec->capacity);
	CU_ASSERT_EQUAL(100, vec->size);


	int i=0;
	for(i=0; i<100; i++)
		push_backd(vec, i+0.5);

	CU_ASSERT_EQUAL(100, vec->size);

	erased(vec, 25, 74);

	CU_ASSERT_EQUAL(50, vec->size);

	int j=0;
	for(i=0; i<vec->size; i++,j++) {
		CU_ASSERT_EQUAL(j+0.5, vec->a[i]);
		if(i==24) j +=50;
	}

	free_vecd(vec);
}



void insertd_test()
{
	int array[] = { 0.5, 1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 7.5, 8.5, 9.5 };

	vector_d* vec = init_vec_d(array, 10);

	CU_ASSERT_EQUAL(vec->size, 10);


	int i;
	for(i=0; i<vec->size; i++)
		CU_ASSERT_EQUAL(vec->a[i], i+0.5);

	for(i=0; i<10; i++)
		insertd(vec, 0, -i-0.5);

	CU_ASSERT_EQUAL(vec->size, 20);

	for(i=0; i<vec->size; i++)
		CU_ASSERT_EQUAL(vec->a[i], i-9.5 );

	free_vecd(vec);
}



void popd_test()
{
	vector_d* vec = init_vec_d(NULL, 10);

	CU_ASSERT_EQUAL(vec->capacity, 10+VEC_D_START_SZ);

	int i;
	for(i=0; i<1000; i++)
		push_backd(vec, i);

	CU_ASSERT_EQUAL(vec->size, 1000);

	double temp;
	for(i=999; i>=0; i++) {
		temp = pop_backd(vec);
		CU_ASSERT_EQUAL(temp, i);
	}

	CU_ASSERT_EQUAL(vec->size, 0);

	free_vecd(vec);
}


void reserved_test()
{
	vector_d* vec = vec_d(0);

	reserved(vec, 20);
	CU_ASSERT( vec->capacity>=20 );

	free_vecd(vec);
}


void set_vald_test()
{
	vector_d* vec = vec_d(20);

	set_val_szd(vec, 42.5);
	int i;
	for(i=0; i<vec->size; i++)
		CU_ASSERT_EQUAL(vec->a[i], 42.5);

	set_val_capd(vec, 25.5);

	for(i=0; i<vec->capacity; i++)
		CU_ASSERT_EQUAL(vec->a[i], 25.5);

	free_vecd(vec);
}






/* vector_s tests */
void pushs_test()
{
	vector_s* vec = vec_s(0);

	CU_ASSERT_EQUAL(VEC_S_START_SZ, vec->capacity);
	CU_ASSERT_EQUAL(0, vec->size);

	int i;
	char buffer[50];

	for(i=0; i<50; i++) {
		sprintf(buffer, "hello %d", i);
		push_backs(vec, buffer);
	}

	CU_ASSERT_EQUAL(50, vec->size);

	for(i=0; i<vec->size; i++) {
		sprintf(buffer, "hello %d", i);
		CU_ASSERT_STRING_EQUAL(vec->a[i], buffer);
	}


	free_vecs(vec);
}


void erasei_test()
{
	vector_s* vec = vec_s(10);

	CU_ASSERT_EQUAL(VEC_S_START_SZ, vec->capacity);
	CU_ASSERT_EQUAL(10, vec->size);

	int i;
	char buffer[50];

	for(i=0; i<100; i++) {
		sprintf(buffer, "hello %d", i);
		push_backs(vec, buffer);
	}

	CU_ASSERT_EQUAL(100, vec->size);

	erases(vec, 25, 74);

	CU_ASSERT_EQUAL(50, vec->size);

	int j=0;
	for(i=0; i<vec->size; i++,j++) {
		sprintf(buffer, "hello %d", j);
		CU_ASSERT_STRING_EQUAL(vec->a[i], buffer);

		if(i==24) j +=50;
	}

	free_vecs(vec);

}



void inserti_test()
{
	char* array[] = { "hello 0", "hello 1", "hello 2", "hello 3", "hello 4",
			"hello 5", "hello 6", "hello 7", "hello 8", "hello 9" };

	vector_s* vec = init_vec_s(array, 10);

	CU_ASSERT_EQUAL(vec->size, 10);


	int i;
	for(i=0; i<vec->size; i++) {
		sprintf(buffer, "hello %d", i);
		CU_ASSERT_STRING_EQUAL(vec->a[i], buffer);


	for(i=0; i<10; i++) {
		sprintf(buffer, "hello %d", -i);
		inserts(vec, 0, buffer);
	}


	CU_ASSERT_EQUAL(vec->size, 20);

	for(i=0; i<vec->size; i++) {
		sprintf(buffer, "hello %d", i-((i<10)? 9 : 10));
		CU_ASSERT_STRING_EQUAL(vec->a[i], buffer);
	}

	free_vecs(vec);
}



void pops_test()
{
	vector_s* vec = init_vec_s(NULL, 10);

	CU_ASSERT_EQUAL(vec->capacity, 10+VEC_S_START_SZ);

	int i;
	char buffer[50];

	for(i=0; i<1000; i++) {
		sprintf(buffer, "hello %d", i);
		push_backs(vec, buffer);
	}

	CU_ASSERT_EQUAL(vec->size, 1000);

	for(i=999; i>=0; i++) {
		pop_backs(vec);
	}

	//should add some testing here
	//especially if I change pop_backs to have a return parameter

	CU_ASSERT_EQUAL(vec->size, 0);

	free_vecs(vec);
}


void reserves_test()
{
	vector_s* vec = vec_s(0);

	reserves(vec, 20);
	CU_ASSERT( vec->capacity>=20 );

	free_vecs(vec);
}


void set_vals_test()
{
	vector_s* vec = vec_s(20);

	set_val_szs(vec, "42");
	int i;
	for(i=0; i<vec->size; i++)
		CU_ASSERT_STRING_EQUAL(vec->a[i], "42");

	set_val_caps(vec, "25");

	for(i=0; i<vec->capacity; i++)
		CU_ASSERT_STRING_EQUAL(vec->a[i], "25");

	free_vecs(vec);
}
























