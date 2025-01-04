
#include "test_types.h"

/*
 uncomment this and in cvector_all.c to test with something other than
 the default size_t

#define CVEC_SIZE_T int
*/

/*
 * recommended way is cvector.h which has everythng: all 4 base types flat
 * (not #included) + the macros for compile time code generation
 */
#ifdef USE_CVECTOR_H

#define CVECTOR_IMPLEMENTATION
#ifdef USE_POSIX_STRDUP
#define CVEC_STRDUP strdup
#endif
#include "cvector.h"

#define RESIZE(a) ((a+1)*2)

CVEC_NEW_DECLS(short)
CVEC_NEW_DECLS2(f_struct)

CVEC_NEW_DEFS(short, RESIZE)
CVEC_NEW_DEFS2(f_struct, RESIZE)

#else
/* otherwise you can use individual headers (cvector_all.h just includes them) */

#include "cvector_all.h"

#ifdef USE_TEMPLATES

/* plus files generated with generate_code.py and the template files */

/* replace with or add your own generated file and edit the template test*/
#define CVECTOR_short_IMPLEMENTATION
#define CVECTOR_f_struct_IMPLEMENTATION
#include "cvector_short.h"
#include "cvector_f_struct.h"

#else

/* or plus cvector_macro.h which has only the macros */

#include "cvector_macro.h"

#define RESIZE(a) ((a+1)*2)

CVEC_NEW_DECLS(short)
CVEC_NEW_DECLS2(f_struct)

CVEC_NEW_DEFS(short, RESIZE)
CVEC_NEW_DEFS2(f_struct, RESIZE)

#endif
/* end templates vs macros */

#endif
/* end cvector.h vs cvector_all.h */

#include <CUnit/Automated.h>
#include <stdio.h>


/* cvector_i tests */
void push_i_test()
{
	int i;
	cvector_i vec;
	cvec_i(&vec, 0, 0);

	CU_ASSERT_EQUAL(CVEC_I_START_SZ, vec.capacity);
	CU_ASSERT_EQUAL(0, vec.size);

	for (i=0; i<100; i++)
		cvec_push_i(&vec, i);

	CU_ASSERT_EQUAL(100, vec.size);

	for (i=0; i<vec.size; i++)
		CU_ASSERT_EQUAL(i, vec.a[i]);

	cvec_free_i(&vec);
}


void erase_i_test()
{
	int i,j;
	cvector_i vec;
	cvec_i(&vec, 100, 0);

	CU_ASSERT_EQUAL(CVEC_I_START_SZ+100, vec.capacity);
	CU_ASSERT_EQUAL(100, vec.size);


	for (i=0; i<100; i++)
		vec.a[i] = i;

	CU_ASSERT_EQUAL(100, vec.size);

	cvec_erase_i(&vec, 25, 74);

	CU_ASSERT_EQUAL(50, vec.size);

	for (i=0,j=0; i<vec.size; i++,j++) {
		CU_ASSERT_EQUAL(j, vec.a[i]);
		if(i == 24) j +=50;
	}

	cvec_free_i(&vec);
}

/* zeroing is valid initialization and cvec_free_i returns to this state */
void zero_init_i_test()
{
	int i;
	cvector_i vec = { 0 };

	CU_ASSERT(vec.capacity == 0)
	CU_ASSERT(vec.size == 0)
	CU_ASSERT(vec.a == NULL)

	for (i=0; i<100; i++)
		cvec_push_i(&vec, i);

	CU_ASSERT_EQUAL(100, vec.size);

	/* based on (x+1)*2 allocator macros */
	CU_ASSERT_EQUAL(126, vec.capacity);

	for (i=0; i<vec.size; i++)
		CU_ASSERT_EQUAL(i, vec.a[i]);

	cvec_free_i(&vec);
	CU_ASSERT(vec.capacity == 0)
	CU_ASSERT(vec.size == 0)
	CU_ASSERT(vec.a == NULL)
}


void insert_i_test()
{
	int array[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	int i;

	cvector_i vec;
	cvec_init_i(&vec, array, 10);

	CU_ASSERT_EQUAL(vec.size, 10);

	for (i=0; i<vec.size; i++)
		CU_ASSERT_EQUAL(vec.a[i], i);

	for (i=0; i<10; i++)
		cvec_insert_i(&vec, 0, -i);

	CU_ASSERT_EQUAL(vec.size, 20);

	for (i=0; i<vec.size; i++)
		CU_ASSERT_EQUAL(vec.a[i], i-((i<10)? 9 : 10) );

	cvec_free_i(&vec);
}

void insert_array_i_test()
{
	int i;
	int array[] = { 0, 1, 0, 1, 0, 1, 0, 1 };
	cvector_i vec;
	cvec_i(&vec, 0, 0);

	for (i=0; i<CVEC_I_START_SZ*2-5; ++i)
		cvec_push_i(&vec, i);

	CU_ASSERT_EQUAL(vec.size, CVEC_I_START_SZ*2-5);
	CU_ASSERT_EQUAL(vec.capacity, (CVEC_I_START_SZ+1)*2);

	cvec_insert_array_i(&vec, 30, array, 8);

	CU_ASSERT_EQUAL(vec.size, CVEC_I_START_SZ*2-5 + 8);

	CU_ASSERT_EQUAL(vec.capacity, (CVEC_I_START_SZ+1)*2 + 8 + CVEC_I_START_SZ);

	for (i=0; i<vec.size; ++i) {
		if (i < 30) {
			CU_ASSERT_EQUAL(vec.a[i], i);
		} else if (i < 38) {
			CU_ASSERT_EQUAL(vec.a[i], (i%2));
		} else {
			CU_ASSERT_EQUAL(vec.a[i], i-8);
		}
	}

	cvec_free_i(&vec);
}

void copy_i_test()
{
	int i;
	cvector_i vec1 = { 0 };
	cvector_i vec2 = { 0 };

	for (i=0; i<123; i++)
		cvec_push_i(&vec1, i);

	CU_ASSERT_EQUAL(123, vec1.size);

	cvec_copy_i(&vec2, &vec1);

	CU_ASSERT_EQUAL(123, vec2.size);

	/* This is true for now, could change.  See TODO note above implementation */
	CU_ASSERT_EQUAL(vec1.capacity, vec2.capacity);

	for (i=0; i<vec1.size; i++)
		CU_ASSERT_EQUAL(vec1.a[i], vec2.a[i]);

	cvec_free_i(&vec1);
	cvec_free_i(&vec2);
}


void pop_i_test()
{
	int i, temp;
	cvector_i vec;
	cvec_i(&vec, 10, 100);

	CU_ASSERT_EQUAL(vec.capacity, 100);
	CU_ASSERT_EQUAL(vec.size, 10);

	for (i=0; i<1000; i++) {
		cvec_push_i(&vec, i);
	}
	CU_ASSERT_EQUAL(vec.size, 1010);


	cvec_set_cap_i(&vec, vec.size);
	CU_ASSERT_EQUAL(vec.size, vec.capacity);

	for (i=999; i>=0; i--) {
		temp = cvec_pop_i(&vec);
		CU_ASSERT_EQUAL(temp, i);
	}

	CU_ASSERT_EQUAL(vec.size, 10);

	cvec_free_i(&vec);
}

void replace_i_test()
{
	int array[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	int ret;

	cvector_i vec;
	cvec_init_i(&vec, array, 10);

	CU_ASSERT_EQUAL(vec.a[4], 4);

	ret = cvec_replace_i(&vec, 4, 42);

	CU_ASSERT_EQUAL(ret, 4);
	CU_ASSERT_EQUAL(vec.a[4], 42);

	cvec_free_i(&vec);
}

void reserve_i_test()
{
	cvector_i* vec = cvec_i_heap(0, 0);

	CU_ASSERT_EQUAL(vec->size, 0);
	CU_ASSERT_EQUAL(vec->capacity, CVEC_I_START_SZ);

	cvec_reserve_i(vec, 20);
	CU_ASSERT(vec->capacity >= 20);

	cvec_free_i_heap(vec);
}


void set_capacity_i_test()
{
	int i;
	cvector_i* vec = cvec_i_heap(0, 0);

	CU_ASSERT_EQUAL(vec->size, 0);
	CU_ASSERT_EQUAL(vec->capacity, CVEC_I_START_SZ);

	for (i=0; i<1000; i++)
		cvec_push_i(vec, i);

	CU_ASSERT(vec->capacity >= 1000);
	CU_ASSERT(vec->size == 1000);

	cvec_set_cap_i(vec, 500);

	CU_ASSERT(vec->capacity == 500);
	CU_ASSERT(vec->size == 500);

	for (i=0; i<vec->size; i++)
		CU_ASSERT_EQUAL(vec->a[i], i);

	cvec_free_i_heap(vec);
}




void set_val_i_test()
{
	int i;
	cvector_i vec;
	cvec_i(&vec, 20, 20);

	CU_ASSERT_EQUAL(vec.size, 20);
	CU_ASSERT_EQUAL(vec.capacity, 20);

	cvec_set_val_cap_i(&vec, 25);

	for (i=0; i<vec.capacity; i++)
		CU_ASSERT_EQUAL(vec.a[i], 25);

	cvec_set_val_sz_i(&vec, 42);
	for (i=0; i<vec.capacity; i++) {
		/*macro is multiple lines/operations, hence the need for braces*/
		if (i < vec.size) {
			CU_ASSERT_EQUAL(vec.a[i], 42);
		} else {
			CU_ASSERT_EQUAL(vec.a[i], 25);
		}
	}

	cvec_free_i(&vec);
}





/* cvector_d tests */
void push_d_test()
{
	int i;
	cvector_d vec;
	cvec_d(&vec, 0, 1);

	CU_ASSERT_EQUAL(1, vec.capacity);
	CU_ASSERT_EQUAL(0, vec.size);


	for (i=0; i<100; i++)
		cvec_push_d(&vec, i+0.5);

	CU_ASSERT_EQUAL(100, vec.size);

	for (i=0; i<vec.size; i++)
		CU_ASSERT_EQUAL(i+0.5, vec.a[i]);

	cvec_free_d(&vec);
}


void erase_d_test()
{
	int i,j;
	cvector_d vec;
	cvec_d(&vec, 100, 101);

	CU_ASSERT_EQUAL(101, vec.capacity);
	CU_ASSERT_EQUAL(100, vec.size);

	for (i=0; i<100; i++)
		vec.a[i] = i+0.5;

	CU_ASSERT_EQUAL(100, vec.size);

	cvec_erase_d(&vec, 25, 74);

	CU_ASSERT_EQUAL(50, vec.size);

	for (i=0,j=0; i<vec.size; i++,j++) {
		CU_ASSERT_EQUAL(j+0.5, vec.a[i]);
		if(i==24) j +=50;
	}

	cvec_free_d(&vec);
}

/* zeroing is valid initialization and cvec_free_d returns to this state */
void zero_init_d_test()
{
	int i;
	cvector_d vec = { 0 };
	
	CU_ASSERT(vec.capacity == 0)
	CU_ASSERT(vec.size == 0)
	CU_ASSERT(vec.a == NULL)

	for (i=0; i<100; i++)
		cvec_push_d(&vec, i+0.5);

	CU_ASSERT_EQUAL(100, vec.size);

	/* based on (x+1)*2 allocator macros */
	CU_ASSERT_EQUAL(126, vec.capacity);

	for (i=0; i<vec.size; i++)
		CU_ASSERT_EQUAL(i+0.5, vec.a[i]);

	cvec_free_d(&vec);
	CU_ASSERT(vec.capacity == 0)
	CU_ASSERT(vec.size == 0)
	CU_ASSERT(vec.a == NULL)
}

void insert_d_test()
{
	int i;
	double array[] = { 0.5, 1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 7.5, 8.5, 9.5 };

	cvector_d vec1;
	cvec_init_d(&vec1, array, 10);

	CU_ASSERT_EQUAL(vec1.size, 10);
	CU_ASSERT_EQUAL(vec1.capacity, 10+CVEC_D_START_SZ);

	for (i=0; i<vec1.size; i++)
		CU_ASSERT_EQUAL(vec1.a[i], i+0.5);

	for (i=0; i<10; i++)
		cvec_insert_d(&vec1, 0, -i-0.5);

	CU_ASSERT_EQUAL(vec1.size, 20);

	for (i=0; i<vec1.size; i++)
		CU_ASSERT_EQUAL(vec1.a[i], i-9.5 );

	cvec_free_d(&vec1);
}


void insert_array_d_test()
{
	int i;
	double array[] = { 0, 1, 0, 1, 0, 1 };
	cvector_d vec;
	cvec_d(&vec, 0, 0);

	for (i=0; i<CVEC_D_START_SZ*2-5; ++i)
		cvec_push_d(&vec, i);

	CU_ASSERT_EQUAL(vec.size, CVEC_D_START_SZ*2-5);

	cvec_insert_array_d(&vec, 30, array, 6);

	CU_ASSERT_EQUAL(vec.size, CVEC_D_START_SZ*2+1);

	for (i=0; i<vec.size; ++i) {
		if (i < 30) {
			CU_ASSERT_EQUAL(vec.a[i], i);
		} else if (i < 36) {
			CU_ASSERT_EQUAL(vec.a[i], (i%2));
		} else {
			CU_ASSERT_EQUAL(vec.a[i], i-6);
		}
	}

	cvec_free_d(&vec);
}

void copy_d_test()
{
	int i;
	cvector_d vec1 = { 0 };
	cvector_d vec2 = { 0 };

	for (i=0; i<123; i++)
		cvec_push_d(&vec1, i+0.5);

	CU_ASSERT_EQUAL(123, vec1.size);

	cvec_copy_d(&vec2, &vec1);

	CU_ASSERT_EQUAL(123, vec2.size);

	/* This is true for now, could change.  See TODO note above implementation */
	CU_ASSERT_EQUAL(vec1.capacity, vec2.capacity);

	for (i=0; i<vec1.size; i++)
		CU_ASSERT_EQUAL(vec1.a[i], vec2.a[i]);

	cvec_free_d(&vec1);
	cvec_free_d(&vec2);
}

void pop_d_test()
{
	int i;
	double temp;
	cvector_d vec1;
	cvec_d(&vec1, 0, 0);

	CU_ASSERT_EQUAL(vec1.capacity, CVEC_D_START_SZ);
	CU_ASSERT_EQUAL(vec1.size, 0);

	for (i=0; i<1000; i++)
		cvec_push_d(&vec1, i);

	CU_ASSERT_EQUAL(vec1.size, 1000);

	cvec_set_cap_d(&vec1, vec1.size);
	CU_ASSERT_EQUAL(vec1.size, vec1.capacity);

	for (i=999; i>=0; i--) {
		temp = cvec_pop_d(&vec1);
		CU_ASSERT_EQUAL(temp, i);
	}

	CU_ASSERT_EQUAL(vec1.size, 0);

	cvec_free_d(&vec1);
}

void replace_d_test()
{
	double array[] = { 0.5, 1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 7.5, 8.5, 9.5 };
	double ret;

	cvector_d vec;
	cvec_init_d(&vec, array, 10);

	CU_ASSERT_EQUAL(vec.a[4], 4.5);

	ret = cvec_replace_d(&vec, 4, 42);

	CU_ASSERT_EQUAL(ret, 4.5);
	CU_ASSERT_EQUAL(vec.a[4], 42);

	cvec_free_d(&vec);
}

void reserve_d_test()
{
	cvector_d* vec = cvec_d_heap(0, 0);

	cvec_reserve_d(vec, 20);
	CU_ASSERT(vec->capacity >= 20);

	cvec_free_d_heap(vec);
}



void set_capacity_d_test()
{
	int i;
	cvector_d vec1;
	cvec_d(&vec1, 0, 10);

	CU_ASSERT_EQUAL(vec1.capacity, 10);

	for (i=0; i<1000; i++)
		cvec_push_d(&vec1, i+0.5);

	CU_ASSERT(vec1.capacity >= 1000);
	CU_ASSERT(vec1.size == 1000);

	cvec_set_cap_d(&vec1, 500);

	CU_ASSERT(vec1.capacity == 500);
	CU_ASSERT(vec1.size == 500);

	for (i=0; i<vec1.size; i++)
		CU_ASSERT_EQUAL(vec1.a[i], i+0.5);

	cvec_free_d(&vec1);
}



void set_val_d_test()
{
	int i;
	cvector_d* vec = cvec_d_heap(20, 20);

	CU_ASSERT_EQUAL(vec->capacity, 20);

	cvec_set_val_sz_d(vec, 42.5);
	for (i=0; i<vec->size; i++)
		CU_ASSERT_EQUAL(vec->a[i], 42.5);

	cvec_set_val_cap_d(vec, 25.5);

	for (i=0; i<vec->capacity; i++)
		CU_ASSERT_EQUAL(vec->a[i], 25.5);

	cvec_free_d_heap(vec);
}






/* cvector_str tests */
void push_str_test()
{
	int i;
	char buffer[50];
	cvector_str* vec = cvec_str_heap(0, 0);

	CU_ASSERT_EQUAL(CVEC_STR_START_SZ, vec->capacity);
	CU_ASSERT_EQUAL(0, vec->size);

	for (i=0; i<50; i++) {
		sprintf(buffer, "hello %d", i);
		cvec_push_str(vec, buffer);
	}

	CU_ASSERT_EQUAL(50, vec->size);

	for (i=0; i<vec->size; i++) {
		sprintf(buffer, "hello %d", i);
		CU_ASSERT_STRING_EQUAL(vec->a[i], buffer);
	}


	cvec_free_str_heap(vec);
}


void erase_str_test()
{
	int i,j;
	char buffer[50];
	cvector_str vec1;
	cvec_str(&vec1, 0, 0);

	CU_ASSERT_EQUAL(CVEC_STR_START_SZ, vec1.capacity);

	for (i=0; i<100; i++) {
		sprintf(buffer, "hello %d", i);
		cvec_push_str(&vec1, buffer);
	}

	CU_ASSERT_EQUAL(100, vec1.size);

	cvec_erase_str(&vec1, 25, 74);

	CU_ASSERT_EQUAL(50, vec1.size);

	for (i=0,j=0; i<vec1.size; i++,j++) {
		sprintf(buffer, "hello %d", j);
		CU_ASSERT_STRING_EQUAL(vec1.a[i], buffer);

		if(i==24) j +=50;
	}

	cvec_free_str(&vec1);

}

void remove_str_test()
{
	int i,j;
	char buffer[50];
	char* strs[100];
	cvector_str vec1;
	cvec_str(&vec1, 0, 0);

	for (i=0; i<100; i++) {
		sprintf(buffer, "hello %d", i);
		cvec_push_str(&vec1, buffer);
	}

	CU_ASSERT_EQUAL(100, vec1.size);

	for (i=25; i<75; ++i)
		strs[i] = vec1.a[i];

	cvec_remove_str(&vec1, 25, 74);

	CU_ASSERT_EQUAL(50, vec1.size);

	for (i=0,j=0; i<vec1.size; i++,j++) {
		sprintf(buffer, "hello %d", j);
		CU_ASSERT_STRING_EQUAL(vec1.a[i], buffer);

		if(i==24) j +=50;
	}
	for (i=25; i<75; ++i) {
		sprintf(buffer, "hello %d", i);
		CU_ASSERT_STRING_EQUAL(strs[i], buffer);
		free(strs[i]);
	}

	cvec_free_str(&vec1);

}

/* zeroing is valid initialization and cvec_free_s returns to this state */
void zero_init_str_test()
{
	int i;
	char buffer[50];
	cvector_str vec = { 0 };
	
	CU_ASSERT(vec.capacity == 0)
	CU_ASSERT(vec.size == 0)
	CU_ASSERT(vec.a == NULL)

	for (i=0; i<50; i++) {
		sprintf(buffer, "hello %d", i);
		cvec_push_str(&vec, buffer);
	}

	CU_ASSERT_EQUAL(50, vec.size);

	/* based on (x+1)*2 allocator macros */
	CU_ASSERT_EQUAL(62, vec.capacity);

	for (i=0; i<vec.size; i++) {
		sprintf(buffer, "hello %d", i);
		CU_ASSERT_STRING_EQUAL(vec.a[i], buffer);
	}

	cvec_free_str(&vec);
	CU_ASSERT(vec.capacity == 0)
	CU_ASSERT(vec.size == 0)
	CU_ASSERT(vec.a == NULL)
}


void insert_str_test()
{
	char* array[] = { "hello 0", "hello 1", "hello 2", "hello 3", "hello 4",
			"hello 5", "hello 6", "hello 7", "hello 8", "hello 9" };

	char buffer[50];
	int i;

	cvector_str* vec = cvec_init_str_heap(array, 10);

	CU_ASSERT_EQUAL(vec->size, 10);

	for (i=0; i<vec->size; i++) {
		sprintf(buffer, "hello %d", i);
		CU_ASSERT_STRING_EQUAL(vec->a[i], buffer);
	}

	for (i=0; i<10; i++) {
		sprintf(buffer, "hello %d", -i);
		cvec_insert_str(vec, 0, buffer);
	}


	CU_ASSERT_EQUAL(vec->size, 20);

	for (i=0; i<vec->size; i++) {
		sprintf(buffer, "hello %d", i-((i<10)? 9 : 10));
		CU_ASSERT_STRING_EQUAL(vec->a[i], buffer);
	}

	cvec_free_str_heap(vec);
}


void insert_array_str_test()
{
	int i;
	char buffer[50];
	char* array[] = { "hello 0", "hello 1", "hello 2", "hello 3", "hello 4",
			"hello 5", "hello 6", "hello 7", "hello 8", "hello 9", "hello 10" };
	cvector_str vec;
	cvec_init_str(&vec, array, 10);

	CU_ASSERT_EQUAL(vec.size, 10);
	CU_ASSERT_EQUAL(vec.capacity, 10+CVEC_STR_START_SZ);

	cvec_insert_array_str(&vec, 0, array, 11);

	CU_ASSERT_EQUAL(vec.size, 21);
	CU_ASSERT_EQUAL(vec.capacity, 10+CVEC_STR_START_SZ);

	for (i=0; i<vec.size; ++i) {
		if (i < 11) {
			sprintf(buffer, "hello %d", i);
			CU_ASSERT_STRING_EQUAL(vec.a[i], buffer);
		}
		else {
			sprintf(buffer, "hello %d", i-11);
			CU_ASSERT_STRING_EQUAL(vec.a[i], buffer);
		}
	}

	cvec_free_str(&vec);
}


void copy_str_test()
{
	int i;
	cvector_str vec1 = { 0 };
	cvector_str vec2 = { 0 };
	char buffer[50];

	for (i=0; i<123; i++) {
		sprintf(buffer, "hello %d", i);
		cvec_push_str(&vec1, buffer);
	}

	CU_ASSERT_EQUAL(123, vec1.size);

	cvec_copy_str(&vec2, &vec1);

	CU_ASSERT_EQUAL(123, vec2.size);

	/* This is true for now, could change.  See TODO note above implementation */
	CU_ASSERT_EQUAL(vec1.capacity, vec2.capacity);

	for (i=0; i<vec1.size; i++) {
		CU_ASSERT_STRING_EQUAL(vec1.a[i], vec2.a[i]);
	}

	cvec_free_str(&vec1);
	cvec_free_str(&vec2);
}



void pop_str_test()
{
	int i;
	char buffer[50];
	char buffer2[50];

	cvector_str vec1;
	cvec_str(&vec1, 0, 10);

	CU_ASSERT_EQUAL(vec1.capacity, 10);

	cvec_push_str(&vec1, "whatever");
	CU_ASSERT_EQUAL(vec1.size, 1);
	cvec_pop_str(&vec1, NULL);
	CU_ASSERT_EQUAL(vec1.size, 0);

	for (i=0; i<1000; i++) {
		sprintf(buffer, "hello %d", i);
		cvec_push_str(&vec1, buffer);
	}

	CU_ASSERT_EQUAL(vec1.size, 1000);

	cvec_set_cap_str(&vec1, vec1.size);
	CU_ASSERT_EQUAL(vec1.size, vec1.capacity);

	for (i=999; i>=0; i--) {
		sprintf(buffer, "hello %d", i);
		cvec_pop_str(&vec1, buffer2);
		CU_ASSERT_STRING_EQUAL(buffer, buffer2);
	}

	CU_ASSERT_EQUAL(vec1.size, 0);

	cvec_free_str(&vec1);
}

void replace_str_test()
{
	int i, loc, to_replace[] = { 0, 125, 256 };
	char buffer[50];
	char buffer2[50];
	char retbuf[50];

	cvector_str vec;
	cvec_str(&vec, 0, 10);

	CU_ASSERT_EQUAL(vec.capacity, 10);

	for (i=0; i<1000; i++) {
		sprintf(buffer, "hello %d", i);
		cvec_push_str(&vec, buffer);
	}

	for (i=0; i<sizeof(to_replace)/sizeof(int); i++) {
		loc = to_replace[i];
		sprintf(buffer, "hello %d", loc);
		sprintf(buffer2, "goodbye %d", loc);
		CU_ASSERT_STRING_EQUAL(vec.a[loc], buffer);

		cvec_replace_str(&vec, loc, buffer2, retbuf);
		CU_ASSERT_STRING_EQUAL(vec.a[loc], buffer2);
		CU_ASSERT_STRING_EQUAL(retbuf, buffer);
	}
	cvec_free_str(&vec);
}

void reserve_str_test()
{
	cvector_str vec;
	cvec_str(&vec, 0, 100);

	cvec_reserve_str(&vec, 20);
	CU_ASSERT(vec.capacity >= 20);

	cvec_free_str(&vec);
}



void set_capacity_str_test()
{
	int i;
	char buffer[50];

	cvector_str vec;
	cvec_str(&vec, 1, 1);

	CU_ASSERT_EQUAL(vec.size, 1);
	CU_ASSERT_EQUAL(vec.capacity, 1);

	for (i=0; i<1000; i++) {
		sprintf(buffer, "hello %d", i);
		cvec_push_str(&vec, buffer);
	}

	CU_ASSERT(vec.capacity >= 1001);
	CU_ASSERT(vec.size == 1001);

	cvec_set_cap_str(&vec, 500);

	CU_ASSERT(vec.capacity == 500);
	CU_ASSERT(vec.size == 500);

	for (i=0; i<vec.size; i++) {
		sprintf(buffer, "hello %d", i-1);
		if (i) {
			CU_ASSERT_STRING_EQUAL(vec.a[i], buffer);
		} else {
			CU_ASSERT_EQUAL(vec.a[i], NULL);
		}
	}

	cvec_free_str(&vec);
}


void set_val_str_test()
{
	int i;
	cvector_str vec;
	cvec_str(&vec, 20, 0);

	CU_ASSERT_EQUAL(vec.size, 20)
	CU_ASSERT_EQUAL(vec.capacity, 20+CVEC_STR_START_SZ);

	cvec_set_val_sz_str(&vec, "42");

	for (i=0; i<vec.size; i++)
		CU_ASSERT_STRING_EQUAL(vec.a[i], "42");

	cvec_set_val_cap_str(&vec, "25");

	for (i=0; i<vec.capacity; i++)
		CU_ASSERT_STRING_EQUAL(vec.a[i], "25");

	cvec_free_str(&vec);
}

void move_str_test()
{
	int i, j;
	char* ret;
	char buffer[50];
	char* strs[] = {
		"one", "two", "three",
		"four", "five", "six",
		"seven", "eight", "nine",
		"ten"
	};

	cvector_str vec1;
	cvec_str(&vec1, 0, 10);

	cvec_pushm_str(&vec1, CVEC_STRDUP("whatever"));
	CU_ASSERT_EQUAL(vec1.size, 1);
	cvec_pop_str(&vec1, NULL);
	CU_ASSERT_EQUAL(vec1.size, 0);

	for (i=0; i<1000; i++) {
		sprintf(buffer, "hello %d", i);
		cvec_pushm_str(&vec1, CVEC_STRDUP(buffer));
	}

	CU_ASSERT_EQUAL(vec1.size, 1000);

	for (i=999; i>=0; i--) {
		sprintf(buffer, "hello %d", i);
		ret = cvec_popm_str(vec1);
		CU_ASSERT_STRING_EQUAL(buffer, ret);
		free(ret);
	}

	CU_ASSERT_EQUAL(vec1.size, 0);

	for (i=0; i<100; i++) {
		sprintf(buffer, "hello %d", i);
		cvec_push_str(&vec1, buffer);
	}
	cvec_insertm_str(&vec1, 50, CVEC_STRDUP("hello insertm"));
	cvec_replacem_str(vec1, 25, CVEC_STRDUP("hello replacem"), ret);

	CU_ASSERT_STRING_EQUAL(ret, "hello 25");
	free(ret);

	CU_ASSERT_EQUAL(vec1.size, 101);

	j = 0;
	for (i=0; i<101; i++) {
		if (i == 25) {
			CU_ASSERT_STRING_EQUAL(vec1.a[i], "hello replacem");
		} else if (i == 50) {
			CU_ASSERT_STRING_EQUAL(vec1.a[i], "hello insertm");
			j++;
		} else {
			sprintf(buffer, "hello %d", i-j);
			CU_ASSERT_STRING_EQUAL(vec1.a[i], buffer);
		}
	}

	cvec_insert_arraym_str(&vec1, 60, strs, 10);
	CU_ASSERT_EQUAL(vec1.size, 111);

	for (i=0; i<10; ++i) {
		CU_ASSERT_STRING_EQUAL(vec1.a[60+i], strs[i]);
	}
	cvec_remove_str(&vec1, 60, 69);

	CU_ASSERT_EQUAL(vec1.size, 101);

	cvec_free_str(&vec1);
}





/* vector tests */

t_struct mk_t_struct(double d, int i, char* word)
{
	/*could make this static since I'm just copying the values outside */
	t_struct a;
	a.d = d;
	a.i = i;
	strcpy(a.word, word);
	return a;
}

f_struct mk_f_struct(double d, int i, char* word)
{
	/*could make this static since I'm just copying the values outside */
	f_struct a;
	a.d = d;
	a.i = i;
	a.word = CVEC_STRDUP(word);
	return a;
}

f_struct set_f_struct(double d, int i, char* word)
{
	/*could make this static since I'm just copying the values outside */
	f_struct a;
	a.d = d;
	a.i = i;
	a.word = word;
	return a;
}


#define GET_T(X,Y) ((t_struct*)&X.a[Y*X.elem_size])
#define GET_F(X,Y) ((f_struct*)&X.a[Y*X.elem_size])

#define GET_TP(X,Y) ((t_struct*)&X->a[Y*X->elem_size])
#define GET_FP(X,Y) ((f_struct*)&X->a[Y*X->elem_size])

void free_f_struct(void* tmp)
{
	f_struct* f = (f_struct*)tmp;
	free(f->word);
	f->word = NULL;
}


int init_f_struct(void* dest, void* src)
{
	f_struct* d = (f_struct*)dest;
	f_struct* s = (f_struct*)src;
	d->i = s->i;
	d->d = s->d;
	d->word = CVEC_STRDUP(s->word);

	return d->word != NULL;
}



void push_void_test()
{
	char buffer[50];
	int i;
	t_struct temp;
	f_struct temp2;

	cvector_void vec1;
	cvector_void vec2;

	cvec_void(&vec1, 0, 0, sizeof(t_struct), NULL, NULL);
	cvec_void(&vec2, 0, 1,  sizeof(f_struct), free_f_struct, NULL);

	CU_ASSERT_EQUAL(CVEC_VOID_START_SZ, vec1.capacity);
	CU_ASSERT_EQUAL(0, vec1.size);

	CU_ASSERT_EQUAL(1, vec2.capacity);
	CU_ASSERT_EQUAL(0, vec2.size);

	for (i=0; i<100; i++) {
		sprintf(buffer, "%d", i);
		temp = mk_t_struct(i, i, buffer);
		temp2 = mk_f_struct(i, i, buffer);

		cvec_push_void(&vec1, &temp);
		cvec_push_void(&vec2, &temp2);
	}

	CU_ASSERT_EQUAL(100, vec1.size);
	CU_ASSERT_EQUAL(100, vec2.size);


	for (i=0; i<vec1.size; i++) {

		sprintf(buffer, "%d", i);
		CU_ASSERT_EQUAL(i, GET_T(vec1, i)->d);
		CU_ASSERT_EQUAL(i, GET_T(vec1, i)->i);
		CU_ASSERT_STRING_EQUAL(buffer, GET_T(vec1, i)->word);

		CU_ASSERT_EQUAL(i, GET_F(vec2, i)->d);
		CU_ASSERT_EQUAL(i, GET_F(vec2, i)->i);
		CU_ASSERT_STRING_EQUAL(buffer, GET_F(vec2, i)->word);
	}

	cvec_free_void(&vec1);
	cvec_free_void(&vec2);
}


void erase_void_test()
{
	char buffer[50];
	int i,j;

	cvector_void* vec1 = cvec_void_heap(100, 101, sizeof(t_struct), NULL, NULL);
	cvector_void* vec2 = cvec_void_heap(100, 0, sizeof(f_struct), free_f_struct, NULL);

	CU_ASSERT_EQUAL(101, vec1->capacity);
	CU_ASSERT_EQUAL(100, vec1->size);

	CU_ASSERT_EQUAL(CVEC_VOID_START_SZ+100, vec2->capacity);
	CU_ASSERT_EQUAL(100, vec2->size);

	for (i=0; i<100; i++) {
		sprintf(buffer, "%d", i);

		GET_TP(vec1, i)->d = i+0.5;
		GET_TP(vec1, i)->i = i;
		strcpy(GET_TP(vec1, i)->word, buffer);

		GET_FP(vec2, i)->d = i+0.5;
		GET_FP(vec2, i)->i = i;
		GET_FP(vec2, i)->word = CVEC_STRDUP(buffer);
	}

	CU_ASSERT_EQUAL(100, vec1->size);
	CU_ASSERT_EQUAL(100, vec2->size);

	cvec_erase_void(vec1, 25, 74);
	cvec_erase_void(vec2, 25, 74);

	CU_ASSERT_EQUAL(50, vec1->size);
	CU_ASSERT_EQUAL(50, vec2->size);

	for (i=0,j=0; i<vec1->size; i++,j++) {
		sprintf(buffer, "%d", j);
		CU_ASSERT_EQUAL(GET_TP(vec1, i)->d, j+0.5);
		CU_ASSERT_EQUAL(GET_TP(vec1, i)->i, j);
		CU_ASSERT_STRING_EQUAL(GET_TP(vec1, i)->word, buffer);

		CU_ASSERT_EQUAL(GET_FP(vec2, i)->d, j+0.5);
		CU_ASSERT_EQUAL(GET_FP(vec2, i)->i, j);
		CU_ASSERT_STRING_EQUAL(GET_FP(vec2, i)->word, buffer);

		if(i==24) j +=50;
	}

	cvec_free_void_heap(vec1);
	cvec_free_void_heap(vec2);
}

void remove_void_test()
{
	char buffer[50];
	char* strs[100];
	int i,j;

	cvector_void* vec2 = cvec_void_heap(100, 0, sizeof(f_struct), free_f_struct, NULL);

	CU_ASSERT_EQUAL(CVEC_VOID_START_SZ+100, vec2->capacity);
	CU_ASSERT_EQUAL(100, vec2->size);

	for (i=0; i<100; i++) {
		sprintf(buffer, "%d", i);

		GET_FP(vec2, i)->d = i+0.5;
		GET_FP(vec2, i)->i = i;
		GET_FP(vec2, i)->word = CVEC_STRDUP(buffer);
	}

	CU_ASSERT_EQUAL(100, vec2->size);

	for (i=25; i<75; i++)
		strs[i] = GET_FP(vec2, i)->word;

	cvec_remove_void(vec2, 25, 74);

	CU_ASSERT_EQUAL(50, vec2->size);

	for (i=0,j=0; i<vec2->size; i++,j++) {
		sprintf(buffer, "%d", j);

		CU_ASSERT_EQUAL(GET_FP(vec2, i)->d, j+0.5);
		CU_ASSERT_EQUAL(GET_FP(vec2, i)->i, j);
		CU_ASSERT_STRING_EQUAL(GET_FP(vec2, i)->word, buffer);

		if(i==24) j +=50;
	}

	cvec_free_void_heap(vec2);

	for (i=25; i<75; ++i) {
		sprintf(buffer, "%d", i);
		CU_ASSERT_STRING_EQUAL(strs[i], buffer);
		free(strs[i]);
	}

}


void insert_void_test()
{
	t_struct array[10];
	f_struct array2[10];
	int i;
	char buffer[50];

	cvector_void vec1, vec2;

	t_struct temp;
	f_struct temp2;

	for (i=0; i<10; i++) {
		sprintf(buffer, "hello %d", i);
		array[i].d = i+0.5;
		array[i].i = i;
		strcpy(array[i].word, buffer);

		array2[i].d = i+0.5;
		array2[i].i = i;
		array2[i].word = CVEC_STRDUP(buffer);
	}


	cvec_init_void(&vec1, array, 10, sizeof(t_struct), NULL, NULL);
	cvec_init_void(&vec2, array2, 10, sizeof(f_struct), free_f_struct, NULL);


	CU_ASSERT_EQUAL(vec1.size, 10);
	CU_ASSERT_EQUAL(vec2.size, 10);


	for (i=0; i<vec1.size; i++) {
		sprintf(buffer, "hello %d", i);
		CU_ASSERT_EQUAL(GET_T(vec1, i)->d, i+0.5);
		CU_ASSERT_EQUAL(GET_T(vec1, i)->i, i);
		CU_ASSERT_STRING_EQUAL(GET_T(vec1, i)->word, buffer);

		CU_ASSERT_EQUAL(GET_F(vec2, i)->d, i+0.5);
		CU_ASSERT_EQUAL(GET_F(vec2, i)->i, i);
		CU_ASSERT_STRING_EQUAL(GET_F(vec2, i)->word, buffer);
	}

	for (i=0; i<10; i++) {
		sprintf(buffer, "hello %d", -i);
		temp = mk_t_struct(-i-0.5, -i, buffer);
		temp2 = mk_f_struct(-i-0.5, -i, buffer);

		cvec_insert_void(&vec1, 0, &temp);
		cvec_insert_void(&vec2, 0, &temp2);
	}

	CU_ASSERT_EQUAL(vec1.size, 20);
	CU_ASSERT_EQUAL(vec2.size, 20);


	for (i=0; i<vec1.size; i++) {
		sprintf(buffer, "hello %d", i-((i<10)? 9 : 10));

		CU_ASSERT_EQUAL(GET_T(vec1, i)->d, i-0.5-9 );
		CU_ASSERT_EQUAL(GET_T(vec1, i)->i, i-((i<10)? 9 : 10) );
		CU_ASSERT_STRING_EQUAL(GET_T(vec1, i)->word, buffer);

		CU_ASSERT_EQUAL(GET_F(vec2, i)->d, i-0.5-9 );
		CU_ASSERT_EQUAL(GET_F(vec2, i)->i, i-((i<10)? 9 : 10) );
		CU_ASSERT_STRING_EQUAL(GET_F(vec2, i)->word, buffer);

	}

	cvec_free_void(&vec1);
	cvec_free_void(&vec2);
}


void insert_array_void_test()
{
	int i;
	float array1[] = { 0.5f, 1.5f, 7.5f, 8.5f, 9.5f };
	float array2[] = { 2.5f, 3.5f, 4.5f, 5.5f, 6.5f };
	cvector_void vec1;

	cvec_init_void(&vec1, array1, 5, sizeof(float), NULL, NULL);
	cvec_insert_array_void(&vec1, 2, array2, 5);
	CU_ASSERT_EQUAL(vec1.size, 10);

	for (i=0; i<vec1.size; ++i)
		CU_ASSERT_EQUAL(((float*)vec1.a)[i], i+0.5f);

	cvec_free_void(&vec1);
}


/* zeroing size, capacity, and a is valid initialization,
 * you still need to set elem_size and optionally elem_init/elem_free */
void zero_init_void_test()
{
	char buffer[50];
	int i;
	t_struct temp;
	f_struct temp2;

	cvector_void vec1 = { 0, 0, 0, sizeof(t_struct) };
	cvector_void vec2 = { 0, 0, 0, sizeof(f_struct), free_f_struct };

	CU_ASSERT(vec1.capacity == 0)
	CU_ASSERT(vec1.size == 0)
	CU_ASSERT(vec1.a == NULL)
	CU_ASSERT(vec2.capacity == 0)
	CU_ASSERT(vec2.size == 0)
	CU_ASSERT(vec2.a == NULL)

	for (i=0; i<100; i++) {
		sprintf(buffer, "%d", i);
		temp = mk_t_struct(i, i, buffer);
		temp2 = mk_f_struct(i, i, buffer);

		cvec_push_void(&vec1, &temp);
		cvec_push_void(&vec2, &temp2);
	}

	/* based on (x+1)*2 allocator macros */
	CU_ASSERT_EQUAL(126, vec1.capacity);
	CU_ASSERT_EQUAL(126, vec2.capacity);

	for (i=0; i<vec1.size; i++) {

		sprintf(buffer, "%d", i);
		CU_ASSERT_EQUAL(i, GET_T(vec1, i)->d);
		CU_ASSERT_EQUAL(i, GET_T(vec1, i)->i);
		CU_ASSERT_STRING_EQUAL(buffer, GET_T(vec1, i)->word);

		CU_ASSERT_EQUAL(i, GET_F(vec2, i)->d);
		CU_ASSERT_EQUAL(i, GET_F(vec2, i)->i);
		CU_ASSERT_STRING_EQUAL(buffer, GET_F(vec2, i)->word);
	}

	cvec_free_void(&vec1);
	cvec_free_void(&vec2);
	CU_ASSERT(vec1.capacity == 0)
	CU_ASSERT(vec1.size == 0)
	CU_ASSERT(vec1.a == NULL)
	CU_ASSERT(vec2.capacity == 0)
	CU_ASSERT(vec2.size == 0)
	CU_ASSERT(vec2.a == NULL)
}


void copy_void_test()
{
	char buffer[50];
	int i;
	t_struct temp;
	f_struct temp2;

	cvector_void vec1;
	cvector_void vec2;
	cvector_void vec3 = { 0 };
	cvector_void vec4 = { 0 };

	cvec_void(&vec1, 0, 0, sizeof(t_struct), NULL, NULL);
	cvec_void(&vec2, 0, 1,  sizeof(f_struct), free_f_struct, init_f_struct);

	CU_ASSERT_EQUAL(CVEC_VOID_START_SZ, vec1.capacity);
	CU_ASSERT_EQUAL(0, vec1.size);

	CU_ASSERT_EQUAL(1, vec2.capacity);
	CU_ASSERT_EQUAL(0, vec2.size);

	for (i=0; i<100; i++) {
		sprintf(buffer, "%d", i);
		temp = mk_t_struct(i, i, buffer);
		temp2 = set_f_struct(i, i, buffer);

		cvec_push_void(&vec1, &temp);
		cvec_push_void(&vec2, &temp2);
	}

	CU_ASSERT_EQUAL(100, vec1.size);
	CU_ASSERT_EQUAL(100, vec2.size);

	cvec_copy_void(&vec3, &vec1);
	cvec_copy_void(&vec4, &vec2);

	CU_ASSERT_EQUAL(vec3.size, vec1.size);
	CU_ASSERT_EQUAL(vec4.size, vec2.size);

	/* This is true for now, could change.  See TODO note above implementation */
	CU_ASSERT_EQUAL(vec3.capacity, vec1.capacity);
	CU_ASSERT_EQUAL(vec4.capacity, vec2.capacity);

	CU_ASSERT_EQUAL(vec3.elem_size, vec1.elem_size);
	CU_ASSERT_EQUAL(vec4.elem_size, vec2.elem_size);

	CU_ASSERT_EQUAL(vec3.elem_free, vec1.elem_free);
	CU_ASSERT_EQUAL(vec4.elem_free, vec2.elem_free);

	CU_ASSERT_EQUAL(vec3.elem_init, vec1.elem_init);
	CU_ASSERT_EQUAL(vec4.elem_init, vec2.elem_init);

	for (i=0; i<vec1.size; i++) {

		CU_ASSERT_EQUAL(GET_T(vec3, i)->d, GET_T(vec1, i)->d);
		CU_ASSERT_EQUAL(GET_T(vec3, i)->i, GET_T(vec1, i)->i);
		CU_ASSERT_STRING_EQUAL(GET_T(vec3, i)->word, GET_T(vec1, i)->word);

		CU_ASSERT_EQUAL(GET_F(vec4, i)->d, GET_F(vec2, i)->d);
		CU_ASSERT_EQUAL(GET_F(vec4, i)->i, GET_F(vec2, i)->i);
		CU_ASSERT_STRING_EQUAL(GET_F(vec4, i)->word, GET_F(vec2, i)->word);
	}

	cvec_free_void(&vec1);
	cvec_free_void(&vec2);
	cvec_free_void(&vec3);
	cvec_free_void(&vec4);
}



void pop_void_test()
{
	char buffer[50];
	int i;
	t_struct temp;
	f_struct temp2;

	cvector_void vec1, vec2;

	cvec_void(&vec1, 10, 0, sizeof(t_struct), NULL, NULL);
	cvec_void(&vec2, 10, 0, sizeof(f_struct), free_f_struct, NULL);

	CU_ASSERT_EQUAL(vec1.capacity, 10+CVEC_VOID_START_SZ);
	CU_ASSERT_EQUAL(vec2.capacity, 10+CVEC_VOID_START_SZ);

	for (i=0; i<vec2.size; ++i)
		GET_F(vec2, i)->word = CVEC_STRDUP("hello");


	strcpy(buffer, "test_pop_null");
	temp = mk_t_struct(i, i, buffer);
	temp2 = mk_f_struct(i, i, buffer);

	cvec_push_void(&vec1, &temp);
	cvec_push_void(&vec2, &temp2);
	CU_ASSERT_EQUAL(vec1.size, 11);
	CU_ASSERT_EQUAL(vec2.size, 11);
	cvec_pop_void(&vec1, NULL);
	cvec_pop_void(&vec2, NULL);
	CU_ASSERT_EQUAL(vec1.size, 10);
	CU_ASSERT_EQUAL(vec2.size, 10);


	for (i=0; i<1000; i++) {
		sprintf(buffer, "hello %d", i);
		temp = mk_t_struct(i, i, buffer);
		temp2 = mk_f_struct(i, i, buffer);

		cvec_push_void(&vec1, &temp);
		cvec_push_void(&vec2, &temp2);
	}

	cvec_set_cap_void(&vec1, vec1.size);
	CU_ASSERT_EQUAL(vec1.size, vec1.capacity);
	CU_ASSERT_EQUAL(vec1.size, 1010);

	cvec_set_cap_void(&vec2, vec2.size);
	CU_ASSERT_EQUAL(vec2.size, vec2.capacity);
	CU_ASSERT_EQUAL(vec2.size, 1010);



	for (i=999; i>=0; i--) {
		sprintf(buffer, "hello %d", i);
		cvec_pop_void(&vec1, &temp);
		cvec_pop_void(&vec2, &temp2);

		CU_ASSERT_EQUAL(temp.d, i );
		CU_ASSERT_EQUAL(temp.i, i );
		CU_ASSERT_STRING_EQUAL(temp.word, buffer);

		CU_ASSERT_EQUAL(temp2.d, i );
		CU_ASSERT_EQUAL(temp2.i, i );
		/*No CU_ASSERT_STRING_EQUAL here because the string
		was freed when it was popped */
	}

	CU_ASSERT_EQUAL(vec1.size, 10);
	CU_ASSERT_EQUAL(vec2.size, 10);

	cvec_free_void(&vec1);
	cvec_free_void(&vec2);
}

void replace_void_test()
{
	char buffer[50];
	char buffer2[50];
	int i, loc, to_replace[] = { 1, 25, 50 };
	t_struct temp, ret_t_struct;
	f_struct temp2, ret_f_struct, temp3;

	cvector_void vec1;
	cvector_void vec2;
	cvector_void vec3;

	cvec_void(&vec1, 0, 0, sizeof(t_struct), NULL, NULL);
	cvec_void(&vec2, 0, 0,  sizeof(f_struct), free_f_struct, NULL);
	cvec_void(&vec3, 0, 0,  sizeof(f_struct), free_f_struct, init_f_struct);


	for (i=0; i<100; i++) {
		sprintf(buffer, "%d", i);
		temp = mk_t_struct(i, i, buffer);
		temp2 = mk_f_struct(i, i, buffer);
		temp3 = set_f_struct(i, i, buffer);

		cvec_push_void(&vec1, &temp);
		cvec_push_void(&vec2, &temp2);
		cvec_push_void(&vec3, &temp3);
	}

	for (i=0; i<sizeof(to_replace)/sizeof(int); ++i) {
		loc = to_replace[i];
		sprintf(buffer, "%d", loc);
		CU_ASSERT_EQUAL(loc, GET_T(vec1, loc)->d);
		CU_ASSERT_EQUAL(loc, GET_T(vec1, loc)->i);
		CU_ASSERT_STRING_EQUAL(buffer, GET_T(vec1, loc)->word);

		CU_ASSERT_EQUAL(loc, GET_F(vec2, loc)->d);
		CU_ASSERT_EQUAL(loc, GET_F(vec2, loc)->i);
		CU_ASSERT_STRING_EQUAL(buffer, GET_F(vec2, loc)->word);

		CU_ASSERT_EQUAL(loc, GET_F(vec3, loc)->d);
		CU_ASSERT_EQUAL(loc, GET_F(vec3, loc)->i);
		CU_ASSERT_STRING_EQUAL(buffer, GET_F(vec3, loc)->word);

		sprintf(buffer2, "neg %d", -loc);
		temp = mk_t_struct(-loc, -loc, buffer2);
		temp2 = mk_f_struct(-loc, -loc, buffer2);
		temp3 = set_f_struct(-loc, -loc, buffer2);

		cvec_replace_void(&vec1, loc, &temp, &ret_t_struct);
		cvec_replace_void(&vec2, loc, &temp2, &ret_f_struct);

		CU_ASSERT_EQUAL(-loc, GET_T(vec1, loc)->d);
		CU_ASSERT_EQUAL(-loc, GET_T(vec1, loc)->i);
		CU_ASSERT_STRING_EQUAL(buffer2, GET_T(vec1, loc)->word);

		CU_ASSERT_EQUAL(-loc, GET_F(vec2, loc)->d);
		CU_ASSERT_EQUAL(-loc, GET_F(vec2, loc)->i);
		CU_ASSERT_STRING_EQUAL(buffer2, GET_F(vec2, loc)->word);

		CU_ASSERT_EQUAL(loc, ret_t_struct.d);
		CU_ASSERT_EQUAL(loc, ret_t_struct.i);
		CU_ASSERT_STRING_EQUAL(buffer, ret_t_struct.word);

		CU_ASSERT_EQUAL(loc, ret_f_struct.d);
		CU_ASSERT_EQUAL(loc, ret_f_struct.i);
		CU_ASSERT_STRING_EQUAL(buffer, ret_f_struct.word);

		free_f_struct(&ret_f_struct);

		/* Test replace with elem_init */
		cvec_replace_void(&vec3, loc, &temp3, &ret_f_struct);
		CU_ASSERT_EQUAL(-loc, GET_F(vec3, loc)->d);
		CU_ASSERT_EQUAL(-loc, GET_F(vec3, loc)->i);
		CU_ASSERT_STRING_EQUAL(buffer2, GET_F(vec3, loc)->word);

		CU_ASSERT_EQUAL(loc, ret_f_struct.d);
		CU_ASSERT_EQUAL(loc, ret_f_struct.i);
		CU_ASSERT_STRING_EQUAL(buffer, ret_f_struct.word);

		free_f_struct(&ret_f_struct);
	}

	/* Test replace with no ret val */
	loc = 20;
	sprintf(buffer, "%d", loc);
	sprintf(buffer2, "neg %d", -loc);
	temp = mk_t_struct(-loc, -loc, buffer2);
	temp2 = mk_f_struct(-loc, -loc, buffer2);
	temp3 = set_f_struct(-loc, -loc, buffer2);

	cvec_replace_void(&vec1, loc, &temp, NULL);
	cvec_replace_void(&vec2, loc, &temp2, NULL);
	cvec_replace_void(&vec3, loc, &temp2, NULL);

	CU_ASSERT_EQUAL(-loc, GET_T(vec1, loc)->d);
	CU_ASSERT_EQUAL(-loc, GET_T(vec1, loc)->i);
	CU_ASSERT_STRING_EQUAL(buffer2, GET_T(vec1, loc)->word);

	CU_ASSERT_EQUAL(-loc, GET_F(vec2, loc)->d);
	CU_ASSERT_EQUAL(-loc, GET_F(vec2, loc)->i);
	CU_ASSERT_STRING_EQUAL(buffer2, GET_F(vec2, loc)->word);

	CU_ASSERT_EQUAL(-loc, GET_F(vec3, loc)->d);
	CU_ASSERT_EQUAL(-loc, GET_F(vec3, loc)->i);
	CU_ASSERT_STRING_EQUAL(buffer2, GET_F(vec3, loc)->word);

	cvec_free_void(&vec1);
	cvec_free_void(&vec2);
	cvec_free_void(&vec3);
}

void reserve_void_test()
{
	cvector_void* vect = cvec_void_heap(0, 19, sizeof(t_struct), NULL, NULL);
	cvector_void* vec2 = cvec_void_heap(0, 21, sizeof(f_struct), NULL, NULL);

	CU_ASSERT_EQUAL(vect->capacity, 19);
	CU_ASSERT_EQUAL(vec2->capacity, 21);

	cvec_reserve_void(vect, 20);
	cvec_reserve_void(vec2, 20);

	CU_ASSERT(vect->capacity >= 20);
	CU_ASSERT(vec2->capacity >= 20);

	cvec_free_void_heap(vect);
	cvec_free_void_heap(vec2);
}


void set_capacity_void_test()
{
	char buffer[50];
	int i;
	t_struct temp;
	f_struct temp2;
	cvector_void vec1, vec2;

	cvec_void(&vec1, 0, 0, sizeof(t_struct), NULL, NULL);
	cvec_void(&vec2, 0, 0, sizeof(f_struct), free_f_struct, NULL);

	CU_ASSERT_EQUAL(vec1.size, 0);
	CU_ASSERT_EQUAL(vec1.capacity, CVEC_VOID_START_SZ);
	CU_ASSERT_EQUAL(vec2.size, 0);
	CU_ASSERT_EQUAL(vec2.capacity, CVEC_VOID_START_SZ);

	for (i=0; i<1000; i++) {
		sprintf(buffer, "hello %d", i);
		temp = mk_t_struct(i, i, buffer);
		temp2 = mk_f_struct(i, i, buffer);

		cvec_push_void(&vec1, &temp);
		cvec_push_void(&vec2, &temp2);
	}

	CU_ASSERT(vec1.capacity >= 1000);
	CU_ASSERT(vec1.size == 1000);

	CU_ASSERT(vec2.capacity >= 1000);
	CU_ASSERT(vec2.size == 1000);

	cvec_set_cap_void(&vec1, 500);
	cvec_set_cap_void(&vec2, 500);


	CU_ASSERT(vec1.capacity == 500);
	CU_ASSERT(vec1.size == 500);

	CU_ASSERT(vec2.capacity == 500);
	CU_ASSERT(vec2.size == 500);

	for (i=0; i<vec1.size; i++) {
		sprintf(buffer, "hello %d", i);

		CU_ASSERT_EQUAL(GET_T(vec1, i)->d, i );
		CU_ASSERT_EQUAL(GET_T(vec1, i)->i, i );
		CU_ASSERT_STRING_EQUAL(GET_T(vec1, i)->word, buffer);

		CU_ASSERT_EQUAL(GET_F(vec2, i)->d, i );
		CU_ASSERT_EQUAL(GET_F(vec2, i)->i, i );
		CU_ASSERT_STRING_EQUAL(GET_F(vec2, i)->word, buffer);
	}

	cvec_free_void(&vec1);
	cvec_free_void(&vec2);
}

void set_val_void_test()
{
	int i;
	cvector_void vec1, vec2;
	t_struct temp;
	f_struct temp2;

	cvec_void(&vec1, 20, 25, sizeof(t_struct), NULL, NULL);
	cvec_void(&vec2, 20, 0, sizeof(f_struct), free_f_struct, init_f_struct);

	CU_ASSERT_EQUAL(vec1.size, 20);
	CU_ASSERT_EQUAL(vec2.size, 20);

	CU_ASSERT_EQUAL(vec1.capacity, 25);
	CU_ASSERT_EQUAL(vec2.capacity, 20+CVEC_VOID_START_SZ);

	for (i=0; i<vec2.size; ++i)
		GET_F(vec2, i)->word = CVEC_STRDUP("hello");

	temp = mk_t_struct(42.5, 42, "hello");
	temp2 = mk_f_struct(42.5, 42, "hello");

	cvec_set_val_sz_void(&vec1, &temp);
	cvec_set_val_sz_void(&vec2, &temp2);

	free_f_struct(&temp2);

	for (i=0; i<vec1.size; i++) {
		CU_ASSERT_EQUAL(GET_T(vec1, i)->d, 42.5);
		CU_ASSERT_EQUAL(GET_T(vec1, i)->i, 42);
		CU_ASSERT_STRING_EQUAL(GET_T(vec1, i)->word, "hello");

		CU_ASSERT_EQUAL(GET_F(vec2, i)->d, 42.5);
		CU_ASSERT_EQUAL(GET_F(vec2, i)->i, 42);
		CU_ASSERT_STRING_EQUAL(GET_F(vec2, i)->word, "hello");
	}

	temp = mk_t_struct(25.5, 25, "goodbye");
	temp2 = mk_f_struct(25.5, 25, "goodbye");

	cvec_set_val_cap_void(&vec1, &temp);
	cvec_set_val_cap_void(&vec2, &temp2);

	free_f_struct(&temp2);

	/*difference here between having a free function and not
	if yes then size is set to capacity by set_val_cap. */
	CU_ASSERT_EQUAL(vec1.size, 20);
	CU_ASSERT_EQUAL(vec1.capacity, 25);

	CU_ASSERT_EQUAL(vec2.size, vec2.capacity);
	CU_ASSERT_EQUAL(vec2.capacity, 20+CVEC_VOID_START_SZ);


	for (i=0; i<vec2.capacity; i++) {
		if (i < vec1.capacity) {
			CU_ASSERT_EQUAL(GET_T(vec1, i)->d, 25.5);
			CU_ASSERT_EQUAL(GET_T(vec1, i)->i, 25);
			CU_ASSERT_STRING_EQUAL(GET_T(vec1, i)->word, "goodbye");
		}

		CU_ASSERT_EQUAL(GET_F(vec2, i)->d, 25.5);
		CU_ASSERT_EQUAL(GET_F(vec2, i)->i, 25);
		CU_ASSERT_STRING_EQUAL(GET_F(vec2, i)->word, "goodbye");
	}

	cvec_free_void(&vec1);
	cvec_free_void(&vec2);
}

void free_str(void* s)
{
	free(*((char**)s));
}

/** If we weren't testing the move functions
 * we could have src be just a char* to more closely
 * emulate cvector_str but meh. */
int init_str(void* dest, void* src)
{
	char** d = (char**)dest;
	char** s = (char**)src;

	*d = CVEC_STRDUP(*s);
	return *d != 0;
}

/*
 * TODO
 * Should I add another version that dereferences so you actually get type instead
 * of type* back?  Also should I add this to cvector_void.h or change
 * CVEC_GET_VOID's parameter order?
 */
#define GET_ELEMENT(VEC,I,TYPE) ((TYPE*)&(VEC).a[(I)*(VEC).elem_size])

/* direct mapping of move_str_test to show how and highlight
 * differences */
void move_void_test()
{
	int i, j;
	char* ret, *val;
	char buffer[50];
	char* strs[] = {
		"one", "two", "three",
		"four", "five", "six",
		"seven", "eight", "nine",
		"ten"
	};

	cvector_void vec1;
	cvec_void(&vec1, 0, 10, sizeof(char*), free_str, init_str);

	val = CVEC_STRDUP("whatever");
	cvec_pushm_void(&vec1, &val);
	CU_ASSERT_EQUAL(vec1.size, 1);
	cvec_pop_void(&vec1, NULL);
	CU_ASSERT_EQUAL(vec1.size, 0);

	for (i=0; i<1000; i++) {
		sprintf(buffer, "hello %d", i);
		val = CVEC_STRDUP(buffer);
		cvec_pushm_void(&vec1, &val);
	}

	CU_ASSERT_EQUAL(vec1.size, 1000);

	for (i=999; i>=0; i--) {
		sprintf(buffer, "hello %d", i);
		cvec_popm_void(&vec1, &ret);
		CU_ASSERT_STRING_EQUAL(buffer, ret);
		free(ret);
	}

	CU_ASSERT_EQUAL(vec1.size, 0);

	for (i=0; i<100; i++) {
		sprintf(buffer, "hello %d", i);
		val = buffer;
		cvec_push_void(&vec1, &val);
	}
	val = CVEC_STRDUP("hello insertm");
	cvec_insertm_void(&vec1, 50, &val);
	val = CVEC_STRDUP("hello replacem");
	cvec_replacem_void(&vec1, 25, &val, &ret);

	CU_ASSERT_STRING_EQUAL(ret, "hello 25");
	free(ret);

	CU_ASSERT_EQUAL(vec1.size, 101);

	j = 0;
	for (i=0; i<101; i++) {
		if (i == 25) {
			CU_ASSERT_STRING_EQUAL(*GET_ELEMENT(vec1, i, char*), "hello replacem");
		} else if (i == 50) {
			CU_ASSERT_STRING_EQUAL(*GET_ELEMENT(vec1, i, char*), "hello insertm");
			j++;
		} else {
			sprintf(buffer, "hello %d", i-j);
			CU_ASSERT_STRING_EQUAL(*GET_ELEMENT(vec1, i, char*), buffer);
		}
	}

	cvec_insert_arraym_void(&vec1, 60, strs, 10);
	CU_ASSERT_EQUAL(vec1.size, 111);

	for (i=0; i<10; ++i) {
		CU_ASSERT_STRING_EQUAL(*GET_ELEMENT(vec1, 60+i, char*), strs[i]);
	}
	cvec_remove_void(&vec1, 60, 69);

	CU_ASSERT_EQUAL(vec1.size, 101);

	cvec_free_void(&vec1);
}



void vector_of_vectors_test()
{
	int i, j, tmp_int;
	cvector_void vec1, vec2;
	cvector_i tmp_vec_i, *tmp_vec_i2;
	cvector_str tmp_vecs;
	char buffer[50];

	cvec_void(&vec1, 0, 0, sizeof(cvector_i), cvec_free_i, cvec_copyc_i);
	cvec_void(&vec2, 0, 0, sizeof(cvector_str), cvec_free_str, cvec_copyc_i);

	cvec_str(&tmp_vecs, 0, 0);

	CU_ASSERT_EQUAL(CVEC_STR_START_SZ, tmp_vecs.capacity);
	CU_ASSERT_EQUAL(0, tmp_vecs.size);

	for (i=0; i<50; i++) {
		sprintf(buffer, "hello %d", i);
		cvec_push_str(&tmp_vecs, buffer);
	}

	CU_ASSERT_EQUAL(vec1.size, 0);
	CU_ASSERT_EQUAL(vec1.capacity, CVEC_VOID_START_SZ);

	cvec_i(&tmp_vec_i, 0, 0);

	for (i=0; i<20; ++i)
		cvec_push_i(&tmp_vec_i, rand());

	for (i=0; i<20; ++i) {
		cvec_push_void(&vec1, &tmp_vec_i);
		for (j=0; j<500; ++j) {
			cvec_push_i((cvector_i*)(&vec1.a[i*vec1.elem_size]), j);
		}
		tmp_vec_i2 = cvec_get_void(&vec1, i);
		CU_ASSERT_EQUAL(tmp_vec_i2->size, 520);
	}

	CU_ASSERT_EQUAL(vec1.size, 20);

	for (i=0; i<20; ++i) {
		for (j=0; j<500; ++j) {
			tmp_int = cvec_pop_i(GET_ELEMENT(vec1, (vec1.size-1), cvector_i)); /*&vec1.a[(vec1.size-1)*vec1.elem_size]);    GET_ELEMENT(vec1, i, cvector_i));*/
			CU_ASSERT_EQUAL(tmp_int, 499-j);
		}
		tmp_vec_i2 = cvec_get_void(&vec1, vec1.size-1);
		CU_ASSERT_EQUAL(tmp_vec_i2->size, 20);

		for (j=0; j<20; j++) {
			CU_ASSERT_EQUAL(tmp_vec_i.a[i], tmp_vec_i2->a[i]);
		}

		cvec_pop_void(&vec1, NULL);
	}

	cvec_free_void(&vec1);
	cvec_free_void(&vec2);
	cvec_free_i(&tmp_vec_i);
	cvec_free_str(&tmp_vecs);
}


void template_test()
{
	int i;
	cvector_short vec;

	cvec_short(&vec, 0, 0);

	CU_ASSERT_EQUAL(CVEC_short_SZ, vec.capacity);
	CU_ASSERT_EQUAL(0, vec.size);

	for (i=0; i<100; i++)
		cvec_push_short(&vec, i);

	CU_ASSERT_EQUAL(100, vec.size);

	for (i=0; i<vec.size; i++)
		CU_ASSERT_EQUAL(i, vec.a[i]);

	cvec_free_short(&vec);
	CU_ASSERT(vec.capacity == 0)
	CU_ASSERT(vec.size == 0)
	CU_ASSERT(vec.a == NULL)
}

void template_test2()
{
	int i, j, loc;
	char buffer[50];
	f_struct temp, ret;
	cvector_f_struct vec;
	f_struct array[] = {
		{0, 0, "one"},
		{0, 0, "two"},
		{0, 0, "three"},
		{0, 0, "four"},
		{0, 0, "five"},
		{0, 0, "six"},
		{0, 0, "seven"},
		{0, 0, "eight"},
		{0, 0, "nine"},
		{0, 0, "ten"}
	};

	cvec_f_struct(&vec, 20, 0, free_f_struct, init_f_struct);

	CU_ASSERT_EQUAL(vec.size, 20);
	CU_ASSERT_EQUAL(vec.capacity, 20+CVEC_f_struct_SZ);

	for (i=0; i<vec.size; ++i)
		vec.a[i].word = CVEC_STRDUP("hello");

	temp = mk_f_struct(42.5, 42, "hello");

	cvec_set_val_sz_f_struct(&vec, &temp);

	free_f_struct(&temp);

	for (i=0; i<vec.size; i++) {
		CU_ASSERT_EQUAL(vec.a[i].d, 42.5);
		CU_ASSERT_EQUAL(vec.a[i].i, 42);
		CU_ASSERT_STRING_EQUAL(vec.a[i].word, "hello");
	}

	temp = mk_f_struct(25.5, 25, "goodbye");

	cvec_set_val_cap_f_struct(&vec, &temp);

	free_f_struct(&temp);

	/*
	 * difference here between having a free function and not 
	 * if yes then size is set to capacity by set_val_cap.
	*/
	CU_ASSERT_EQUAL(vec.size, vec.capacity);
	CU_ASSERT_EQUAL(vec.capacity, 20+CVEC_f_struct_SZ);


	for (i=0; i<vec.capacity; i++) {
		CU_ASSERT_EQUAL(vec.a[i].d, 25.5);
		CU_ASSERT_EQUAL(vec.a[i].i, 25);
		CU_ASSERT_STRING_EQUAL(vec.a[i].word, "goodbye");
	}

	loc = 20;
	sprintf(buffer, "neg %d", -loc);
	temp = set_f_struct(-loc, -loc, buffer);
	cvec_replace_f_struct(&vec, loc, &temp, NULL);

	CU_ASSERT_EQUAL(-loc, vec.a[loc].d);
	CU_ASSERT_EQUAL(-loc, vec.a[loc].i);
	CU_ASSERT_STRING_EQUAL(buffer, vec.a[loc].word);

	cvec_free_f_struct(&vec);

	cvec_f_struct(&vec, 0, 0, free_f_struct, NULL);
	CU_ASSERT_EQUAL(vec.capacity, CVEC_f_struct_SZ);

	for (i=0; i<100; i++) {
		sprintf(buffer, "%d", i);
		temp = mk_f_struct(i, i, buffer);

		cvec_push_f_struct(&vec, &temp);
	}

	CU_ASSERT_EQUAL(100, vec.size);

	for (i=0; i<vec.size; i++) {
		sprintf(buffer, "%d", i);
		CU_ASSERT_EQUAL(i, vec.a[i].d);
		CU_ASSERT_EQUAL(i, vec.a[i].i);
		CU_ASSERT_STRING_EQUAL(buffer, vec.a[i].word);
	}

	for (i=vec.size-1; i >= 10; i--) {
		sprintf(buffer, "%d", i);
		cvec_pop_f_struct(&vec, &temp);

		CU_ASSERT_EQUAL(temp.d, i );
		CU_ASSERT_EQUAL(temp.i, i );
		/*No CU_ASSERT_STRING_EQUAL here because the string
		was freed when it was popped */
	}

	CU_ASSERT_EQUAL(10, vec.size);

	/* Test popm */
	cvec_popm_f_struct(&vec, &temp);
	sprintf(buffer, "%d", 9);
	CU_ASSERT_EQUAL(temp.d, 9);
	CU_ASSERT_EQUAL(temp.i, 9);
	CU_ASSERT_STRING_EQUAL(buffer, temp.word);
	free_f_struct(&temp);

	CU_ASSERT_EQUAL(9, vec.size);

	/* Test remove */
	for (i=4; i<7; i++) {
		/* could just call free(vec.a[i].word) */
		free_f_struct(&vec.a[i]);
	}
	cvec_remove_f_struct(&vec, 4, 6);

	CU_ASSERT_EQUAL(6, vec.size);

	for (i=0; i<vec.size; i++) {
		int j = (i < 4) ? i : i+3;
		sprintf(buffer, "%d", j);
		CU_ASSERT_EQUAL(j, vec.a[i].d);
		CU_ASSERT_EQUAL(j, vec.a[i].i);
		CU_ASSERT_STRING_EQUAL(buffer, vec.a[i].word);
	}

	cvec_free_f_struct(&vec);
	CU_ASSERT_EQUAL(vec.size, 0);
	CU_ASSERT_EQUAL(vec.capacity, 0);

	/* test rest of m functions match move_str and move_void tests */
	cvec_f_struct(&vec, 0, 10, free_f_struct, init_f_struct);
	CU_ASSERT_EQUAL(vec.size, 0);
	CU_ASSERT_EQUAL(vec.capacity, 10);

	temp.word = CVEC_STRDUP("blah");
	cvec_pushm_f_struct(&vec, &temp);
	CU_ASSERT_EQUAL(vec.size, 1);
	cvec_pop_f_struct(&vec, NULL);
	CU_ASSERT_EQUAL(vec.size, 0);

	for (i=0; i<1000; i++) {
		sprintf(buffer, "hello %d", i);
		temp.d = i;
		temp.i = i;
		temp.word = CVEC_STRDUP(buffer);
		cvec_pushm_f_struct(&vec, &temp);
	}
	CU_ASSERT_EQUAL(vec.size, 1000);

	for (i=999; i>=0; i--) {
		sprintf(buffer, "hello %d", i);
		cvec_popm_f_struct(&vec, &temp);
		CU_ASSERT_EQUAL(i, temp.i);
		CU_ASSERT_EQUAL(i, temp.d);
		CU_ASSERT_STRING_EQUAL(buffer, temp.word);
		free(temp.word);
	}
	CU_ASSERT_EQUAL(vec.size, 0);

	for (i=0; i<100; i++) {
		sprintf(buffer, "hello %d", i);
		temp.d = i;
		temp.i = i;
		temp.word = buffer;
		cvec_push_f_struct(&vec, &temp);
	}

	temp.word = CVEC_STRDUP("hello insertm");
	cvec_insertm_f_struct(&vec, 50, &temp);

	temp.word = CVEC_STRDUP("hello replacem");
	cvec_replacem_f_struct(&vec, 25, &temp, &ret);

	CU_ASSERT_STRING_EQUAL(ret.word, "hello 25");
	free(ret.word);

	CU_ASSERT_EQUAL(vec.size, 101);

	j = 0;
	for (i=0; i<101; i++) {
		if (i == 25) {
			CU_ASSERT_STRING_EQUAL(vec.a[i].word, "hello replacem");
		} else if (i == 50) {
			CU_ASSERT_STRING_EQUAL(vec.a[i].word, "hello insertm");
			j++;
		} else {
			sprintf(buffer, "hello %d", i-j);
			CU_ASSERT_STRING_EQUAL(vec.a[i].word, buffer);
		}
	}

	cvec_insert_arraym_f_struct(&vec, 60, array, 10);

	CU_ASSERT_EQUAL(vec.size, 111);

	for (i=0; i<10; ++i) {
		CU_ASSERT_STRING_EQUAL(vec.a[60+i].word, array[i].word);
	}
	cvec_remove_f_struct(&vec, 60, 69);

	CU_ASSERT_EQUAL(vec.size, 101);

	cvec_free_f_struct(&vec);
	CU_ASSERT(vec.capacity == 0)
	CU_ASSERT(vec.size == 0)
	CU_ASSERT(vec.a == NULL)
}














