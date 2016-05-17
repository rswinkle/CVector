#ifdef TESTING_CVECTOR_H
	#define CVECTOR_IMPLEMENTATION
	#include "cvector.h"
#else
	#include "vector_all.h"
#endif

#define DO_TEMPLATE_TEST 1
#ifdef DO_TEMPLATE_TEST
/*replace with or add your own generated file and edit the template test*/

#define VECTOR_short_IMPLEMENTATION
#define VECTOR_f_struct_IMPLEMENTATION
#include "vector_short.h"
#include "vector_f_struct.h"
#endif

#include "test_types.h"

#include <CUnit/Automated.h>


/* vector_i tests */
void push_i_test()
{
	int i;
	vector_i vec;
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
	vector_i vec;
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



void insert_i_test()
{
	int array[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	int i;

	vector_i vec;
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
	int array[] = { 0, 1, 0, 1, 0, 1 };
	vector_i vec;
	cvec_i(&vec, 0, 0);

	for (i=0; i<CVEC_I_START_SZ*2-5; ++i)
		cvec_push_i(&vec, i);

	CU_ASSERT_EQUAL(vec.size, CVEC_I_START_SZ*2-5);
	CU_ASSERT_EQUAL(vec.capacity, CVEC_I_START_SZ*2);

	cvec_insert_array_i(&vec, 30, array, 6);

	CU_ASSERT_EQUAL(vec.size, CVEC_I_START_SZ*2+1);
	CU_ASSERT_EQUAL(vec.capacity, 3*CVEC_I_START_SZ+6);

	for (i=0; i<vec.size; ++i) {
		if (i < 30) {
			CU_ASSERT_EQUAL(vec.a[i], i);
		} else if (i < 36) {
			CU_ASSERT_EQUAL(vec.a[i], (i%2));
		} else {
			CU_ASSERT_EQUAL(vec.a[i], i-6);
		}
	}

	cvec_free_i(&vec);
}



void pop_i_test()
{
	int i, temp;
	vector_i vec;
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

	vector_i vec;
	cvec_init_i(&vec, array, 10);

	CU_ASSERT_EQUAL(vec.a[4], 4);

	ret = cvec_replace_i(&vec, 4, 42);

	CU_ASSERT_EQUAL(ret, 4);
	CU_ASSERT_EQUAL(vec.a[4], 42);

	cvec_free_i(&vec);
}

void reserve_i_test()
{
	vector_i* vec = cvec_i_heap(0, 0);

	CU_ASSERT_EQUAL(vec->size, 0);
	CU_ASSERT_EQUAL(vec->capacity, CVEC_I_START_SZ);

	cvec_reserve_i(vec, 20);
	CU_ASSERT(vec->capacity >= 20);

	cvec_free_i_heap(vec);
}


void set_capacity_i_test()
{
	int i;
	vector_i* vec = cvec_i_heap(0, 0);

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
	vector_i vec;
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





/* vector_d tests */
void push_d_test()
{
	int i;
	vector_d vec;
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
	vector_d vec;
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



void insert_d_test()
{
	int i;
	double array[] = { 0.5, 1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 7.5, 8.5, 9.5 };

	vector_d vec1;
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
	vector_d vec;
	cvec_d(&vec, 0, 0);

	for (i=0; i<CVEC_D_START_SZ*2-5; ++i)
		cvec_push_d(&vec, i);

	CU_ASSERT_EQUAL(vec.size, CVEC_D_START_SZ*2-5);
	CU_ASSERT_EQUAL(vec.capacity, CVEC_D_START_SZ*2);

	cvec_insert_array_d(&vec, 30, array, 6);

	CU_ASSERT_EQUAL(vec.size, CVEC_D_START_SZ*2+1);
	CU_ASSERT_EQUAL(vec.capacity, 3*CVEC_D_START_SZ+6);

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


void pop_d_test()
{
	int i;
	double temp;
	vector_d vec1;
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

	vector_d vec;
	cvec_init_d(&vec, array, 10);

	CU_ASSERT_EQUAL(vec.a[4], 4.5);

	ret = cvec_replace_d(&vec, 4, 42);

	CU_ASSERT_EQUAL(ret, 4.5);
	CU_ASSERT_EQUAL(vec.a[4], 42);

	cvec_free_d(&vec);
}

void reserve_d_test()
{
	vector_d* vec = cvec_d_heap(0, 0);

	cvec_reserve_d(vec, 20);
	CU_ASSERT(vec->capacity >= 20);

	cvec_free_d_heap(vec);
}



void set_capacity_d_test()
{
	int i;
	vector_d vec1;
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
	vector_d* vec = cvec_d_heap(20, 20);

	CU_ASSERT_EQUAL(vec->capacity, 20);

	cvec_set_val_sz_d(vec, 42.5);
	for (i=0; i<vec->size; i++)
		CU_ASSERT_EQUAL(vec->a[i], 42.5);

	cvec_set_val_cap_d(vec, 25.5);

	for (i=0; i<vec->capacity; i++)
		CU_ASSERT_EQUAL(vec->a[i], 25.5);

	cvec_free_d_heap(vec);
}






/* vector_str tests */
void push_str_test()
{
	int i;
	char buffer[50];
	vector_str* vec = cvec_str_heap(0, 0);

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
	vector_str vec1;
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



void insert_str_test()
{
	char* array[] = { "hello 0", "hello 1", "hello 2", "hello 3", "hello 4",
			"hello 5", "hello 6", "hello 7", "hello 8", "hello 9" };

	char buffer[50];
	int i;

	vector_str* vec = cvec_init_str_heap(array, 10);

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
	vector_str vec;
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







void pop_str_test()
{
	int i;
	char buffer[50];
	char buffer2[50];

	vector_str vec1;
	cvec_str(&vec1, 0, 10);

	CU_ASSERT_EQUAL(vec1.capacity, 10);

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

	vector_str vec;
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
	vector_str vec;
	cvec_str(&vec, 0, 100);

	cvec_reserve_str(&vec, 20);
	CU_ASSERT(vec.capacity >= 20);

	cvec_free_str(&vec);
}



void set_capacity_str_test()
{
	int i;
	char buffer[50];

	vector_str vec;
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
	vector_str vec;
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
	a.word = mystrdup(word);
	return a;
}


#define GET_T(X,Y) ((t_struct*)&X.a[Y*X.elem_size])
#define GET_F(X,Y) ((f_struct*)&X.a[Y*X.elem_size])

#define GET_TP(X,Y) ((t_struct*)&X->a[Y*X->elem_size])
#define GET_FP(X,Y) ((f_struct*)&X->a[Y*X->elem_size])

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



void push_void_test()
{
	char buffer[50];
	int i;
	t_struct temp;
	f_struct temp2;

	vector_void vec1;
	vector_void vec2;

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

	vector_void* vec1 = cvec_void_heap(100, 101, sizeof(t_struct), NULL, NULL);
	vector_void* vec2 = cvec_void_heap(100, 0, sizeof(f_struct), free_f_struct, NULL);

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
		GET_FP(vec2, i)->word = mystrdup(buffer);
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



void insert_void_test()
{
	t_struct array[10];
	f_struct array2[10];
	int i;
	char buffer[50];

	vector_void vec1, vec2;

	t_struct temp;
	f_struct temp2;

	for (i=0; i<10; i++) {
		sprintf(buffer, "hello %d", i);
		array[i].d = i+0.5;
		array[i].i = i;
		strcpy(array[i].word, buffer);

		array2[i].d = i+0.5;
		array2[i].i = i;
		array2[i].word = mystrdup(buffer);
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
	vector_void vec1;

	cvec_init_void(&vec1, array1, 5, sizeof(float), NULL, NULL);
	cvec_insert_array_void(&vec1, 2, array2, 5);
	CU_ASSERT_EQUAL(vec1.size, 10);

	for (i=0; i<vec1.size; ++i)
		CU_ASSERT_EQUAL(((float*)vec1.a)[i], i+0.5f);

	cvec_free_void(&vec1);
}







void pop_void_test()
{
	char buffer[50];
	int i;
	t_struct temp;
	f_struct temp2;

	vector_void vec1, vec2;

	cvec_void(&vec1, 10, 0, sizeof(t_struct), NULL, NULL);
	cvec_void(&vec2, 10, 0, sizeof(f_struct), free_f_struct, NULL);

	CU_ASSERT_EQUAL(vec1.capacity, 10+CVEC_VOID_START_SZ);
	CU_ASSERT_EQUAL(vec2.capacity, 10+CVEC_VOID_START_SZ);

	for (i=0; i<vec2.size; ++i)
		GET_F(vec2, i)->word = mystrdup("hello");

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
	f_struct temp2, ret_f_struct;

	vector_void vec1;
	vector_void vec2;

	cvec_void(&vec1, 0, 0, sizeof(t_struct), NULL, NULL);
	cvec_void(&vec2, 0, 0,  sizeof(f_struct), free_f_struct, NULL);


	for (i=0; i<100; i++) {
		sprintf(buffer, "%d", i);
		temp = mk_t_struct(i, i, buffer);
		temp2 = mk_f_struct(i, i, buffer);

		cvec_push_void(&vec1, &temp);
		cvec_push_void(&vec2, &temp2);
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

		sprintf(buffer2, "neg %d", -loc);
		temp = mk_t_struct(-loc, -loc, buffer2);
		temp2 = mk_f_struct(-loc, -loc, buffer2);

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
	}

	cvec_free_void(&vec1);
	cvec_free_void(&vec2);
}

void reserve_void_test()
{
	vector_void* vect = cvec_void_heap(0, 19, sizeof(t_struct), NULL, NULL);
	vector_void* vec2 = cvec_void_heap(0, 21, sizeof(f_struct), NULL, NULL);

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
	vector_void vec1, vec2;

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
	vector_void vec1, vec2;
	t_struct temp;
	f_struct temp2;

	cvec_void(&vec1, 20, 25, sizeof(t_struct), NULL, NULL);
	cvec_void(&vec2, 20, 0, sizeof(f_struct), free_f_struct, init_f_struct);

	CU_ASSERT_EQUAL(vec1.size, 20);
	CU_ASSERT_EQUAL(vec2.size, 20);

	CU_ASSERT_EQUAL(vec1.capacity, 25);
	CU_ASSERT_EQUAL(vec2.capacity, 20+CVEC_VOID_START_SZ);

	for (i=0; i<vec2.size; ++i)
		GET_F(vec2, i)->word = mystrdup("hello");

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



#define GET_ELEMENT(X,Y,TYPE) ((TYPE*)&X.a[Y*X.elem_size])

void vector_of_vectors_test()
{
	int i, j, tmp_int;
	vector_void vec1, vec2;
	vector_i tmp_vec_i, *tmp_vec_i2;
	vector_str tmp_vecs;
	char buffer[50];

	cvec_void(&vec1, 0, 0, sizeof(vector_i), cvec_free_i, cvec_i_copy);
	cvec_void(&vec2, 0, 0, sizeof(vector_str), cvec_free_str, cvec_str_copy);

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
			cvec_push_i((vector_i*)(&vec1.a[i*vec1.elem_size]), j);
		}
		tmp_vec_i2 = cvec_get_void(&vec1, i);
		CU_ASSERT_EQUAL(tmp_vec_i2->size, 520);
	}

	CU_ASSERT_EQUAL(vec1.size, 20);

	for (i=0; i<20; ++i) {
		for (j=0; j<500; ++j) {
			tmp_int = cvec_pop_i(GET_ELEMENT(vec1, (vec1.size-1), vector_i)); /*&vec1.a[(vec1.size-1)*vec1.elem_size]);    GET_ELEMENT(vec1, i, vector_i));*/
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
#ifdef DO_TEMPLATE_TEST
	int i;
	vector_short vec;

	vec_short(&vec, 0, 0);

	CU_ASSERT_EQUAL(VEC_short_SZ, vec.capacity);
	CU_ASSERT_EQUAL(0, vec.size);

	for (i=0; i<100; i++)
		push_short(&vec, i);

	CU_ASSERT_EQUAL(100, vec.size);

	for (i=0; i<vec.size; i++)
		CU_ASSERT_EQUAL(i, vec.a[i]);

	free_vec_short(&vec);
#endif
}

void template_test2()
{
#ifdef DO_TEMPLATE_TEST
	int i;
	char buffer[50];
	f_struct temp;
	vector_f_struct vec;

	vec_f_struct(&vec, 20, 0, free_f_struct, init_f_struct);

	CU_ASSERT_EQUAL(vec.size, 20);
	CU_ASSERT_EQUAL(vec.capacity, 20+VEC_f_struct_SZ);

	for (i=0; i<vec.size; ++i)
		vec.a[i].word = mystrdup("hello");

	temp = mk_f_struct(42.5, 42, "hello");

	set_val_sz_f_struct(&vec, &temp);

	free_f_struct(&temp);

	for (i=0; i<vec.size; i++) {
		CU_ASSERT_EQUAL(vec.a[i].d, 42.5);
		CU_ASSERT_EQUAL(vec.a[i].i, 42);
		CU_ASSERT_STRING_EQUAL(vec.a[i].word, "hello");
	}

	temp = mk_f_struct(25.5, 25, "goodbye");

	set_val_cap_f_struct(&vec, &temp);

	free_f_struct(&temp);

	/*difference here between having a free function and not
	if yes then size is set to capacity by set_val_cap. */
	CU_ASSERT_EQUAL(vec.size, vec.capacity);
	CU_ASSERT_EQUAL(vec.capacity, 20+VEC_f_struct_SZ);


	for (i=0; i<vec.capacity; i++) {
		CU_ASSERT_EQUAL(vec.a[i].d, 25.5);
		CU_ASSERT_EQUAL(vec.a[i].i, 25);
		CU_ASSERT_STRING_EQUAL(vec.a[i].word, "goodbye");
	}

	free_vec_f_struct(&vec);

	vec_f_struct(&vec, 0, 0, free_f_struct, NULL);
	CU_ASSERT_EQUAL(vec.capacity, VEC_f_struct_SZ);

	for (i=0; i<100; i++) {
		sprintf(buffer, "%d", i);
		temp = mk_f_struct(i, i, buffer);

		push_f_struct(&vec, &temp);
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
		pop_f_struct(&vec, &temp);

		CU_ASSERT_EQUAL(temp.d, i );
		CU_ASSERT_EQUAL(temp.i, i );
		/*No CU_ASSERT_STRING_EQUAL here because the string
		was freed when it was popped */
	}

	CU_ASSERT_EQUAL(10, vec.size);

	free_vec_f_struct(&vec);

#endif
}














