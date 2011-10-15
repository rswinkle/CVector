#include "vector.h"
#include <CUnit/Automated.h>


/* vector_i tests */
void pushi_test()
{
	int i;
	vector_i* vec = vec_i(0);

	CU_ASSERT_EQUAL(VEC_I_START_SZ, vec->capacity);
	CU_ASSERT_EQUAL(0, vec->size);

	for(i=0; i<100; i++)
		push_backi(vec, i);

	CU_ASSERT_EQUAL(100, vec->size);

	for(i=0; i<vec->size; i++)
		CU_ASSERT_EQUAL(i, vec->a[i]);

	free_veci(vec);
}


void erasei_test()
{
	int i,j;
	vector_i* vec = vec_i(100);

	CU_ASSERT_EQUAL(VEC_I_START_SZ+100, vec->capacity);
	CU_ASSERT_EQUAL(100, vec->size);


	for(i=0; i<100; i++)
		vec->a[i] = i;

	CU_ASSERT_EQUAL(100, vec->size);

	erasei(vec, 25, 74);

	CU_ASSERT_EQUAL(50, vec->size);

	for(i=0,j=0; i<vec->size; i++,j++) {
		CU_ASSERT_EQUAL(j, vec->a[i]);
		if(i==24) j +=50;
	}

	free_veci(vec);
}



void inserti_test()
{
	int array[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	int i;

	vector_i* vec = init_vec_i(array, 10);

	CU_ASSERT_EQUAL(vec->size, 10);

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
	int i, temp;
	vector_i* vec = init_vec_i(NULL, 10);

	CU_ASSERT_EQUAL(vec->capacity, 10+VEC_I_START_SZ);

	for(i=0; i<1000; i++) {
		push_backi(vec, i);

	}
	CU_ASSERT_EQUAL(vec->size, 1000);


	set_capacityi(vec, vec->size);
	CU_ASSERT_EQUAL(vec->size, vec->capacity);

	for(i=999; i>=0; i--) {
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


void set_capacityi_test()
{
	int i;
	vector_i* vec = vec_i(0);

	for(i=0; i<1000; i++)
		push_backi(vec, i);

	CU_ASSERT( vec->capacity>=1000 );
	CU_ASSERT( vec->size==1000);

	set_capacityi(vec, 500);

	CU_ASSERT( vec->capacity==500 );
	CU_ASSERT( vec->size==500);

	for(i=0; i<vec->size; i++)
		CU_ASSERT_EQUAL(vec->a[i], i);

	free_veci(vec);
}




void set_vali_test()
{
	int i;
	vector_i* vec = vec_i(20);

	set_val_szi(vec, 42);
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
	int i;
	vector_d* vec = vec_d(0);

	CU_ASSERT_EQUAL(VEC_D_START_SZ, vec->capacity);
	CU_ASSERT_EQUAL(0, vec->size);


	for(i=0; i<100; i++)
		push_backd(vec, i+0.5);

	CU_ASSERT_EQUAL(100, vec->size);

	for(i=0; i<vec->size; i++)
		CU_ASSERT_EQUAL(i+0.5, vec->a[i]);

	free_vecd(vec);
}


void erased_test()
{
	int i,j;
	vector_d* vec = vec_d(100);

	CU_ASSERT_EQUAL(VEC_D_START_SZ+100, vec->capacity);
	CU_ASSERT_EQUAL(100, vec->size);


	for(i=0; i<100; i++)
		vec->a[i] = i+0.5;

	CU_ASSERT_EQUAL(100, vec->size);

	erased(vec, 25, 74);

	CU_ASSERT_EQUAL(50, vec->size);

	for(i=0,j=0; i<vec->size; i++,j++) {
		CU_ASSERT_EQUAL(j+0.5, vec->a[i]);
		if(i==24) j +=50;
	}

	free_vecd(vec);
}



void insertd_test()
{
	int i;
	double array[] = { 0.5, 1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 7.5, 8.5, 9.5 };

	vector_d* vec = init_vec_d(array, 10);

	CU_ASSERT_EQUAL(vec->size, 10);


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
	int i;
	double temp;
	vector_d* vec = init_vec_d(NULL, 10);

	CU_ASSERT_EQUAL(vec->capacity, 10+VEC_D_START_SZ);

	for(i=0; i<1000; i++)
		push_backd(vec, i);

	CU_ASSERT_EQUAL(vec->size, 1000);

	set_capacityd(vec, vec->size);
	CU_ASSERT_EQUAL(vec->size, vec->capacity);

	for(i=999; i>=0; i--) {
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



void set_capacityd_test()
{
	int i;
	vector_d* vec = vec_d(0);

	for(i=0; i<1000; i++)
		push_backd(vec, i+0.5);

	CU_ASSERT( vec->capacity>=1000 );
	CU_ASSERT( vec->size==1000);

	set_capacityd(vec, 500);

	CU_ASSERT( vec->capacity==500 );
	CU_ASSERT( vec->size==500);

	for(i=0; i<vec->size; i++)
		CU_ASSERT_EQUAL(vec->a[i], i+0.5);

	free_vecd(vec);
}



void set_vald_test()
{
	int i;
	vector_d* vec = vec_d(20);

	set_val_szd(vec, 42.5);
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
	int i;
	char buffer[50];
	vector_s* vec = vec_s(0);

	CU_ASSERT_EQUAL(VEC_S_START_SZ, vec->capacity);
	CU_ASSERT_EQUAL(0, vec->size);

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


void erases_test()
{
	int i,j;
	char buffer[50];
	vector_s* vec = vec_s(0);

	CU_ASSERT_EQUAL(VEC_S_START_SZ, vec->capacity);

	for(i=0; i<100; i++) {
		sprintf(buffer, "hello %d", i);
		push_backs(vec, buffer);
	}

	CU_ASSERT_EQUAL(100, vec->size);

	erases(vec, 25, 74);

	CU_ASSERT_EQUAL(50, vec->size);

	for(i=0,j=0; i<vec->size; i++,j++) {
		sprintf(buffer, "hello %d", j);
		CU_ASSERT_STRING_EQUAL(vec->a[i], buffer);

		if(i==24) j +=50;
	}

	free_vecs(vec);

}



void inserts_test()
{
	char* array[] = { "hello 0", "hello 1", "hello 2", "hello 3", "hello 4",
			"hello 5", "hello 6", "hello 7", "hello 8", "hello 9" };

	char buffer[50];
	int i;

	vector_s* vec = init_vec_s(array, 10);

	CU_ASSERT_EQUAL(vec->size, 10);

	for(i=0; i<vec->size; i++) {
		sprintf(buffer, "hello %d", i);
		CU_ASSERT_STRING_EQUAL(vec->a[i], buffer);
	}

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
	int i;
	char buffer[50];
	char buffer2[50];

	vector_s* vec = init_vec_s(NULL, 10);

	CU_ASSERT_EQUAL(vec->capacity, 10+VEC_S_START_SZ);

	for(i=0; i<1000; i++) {
		sprintf(buffer, "hello %d", i);
		push_backs(vec, buffer);
	}

	CU_ASSERT_EQUAL(vec->size, 1000);

	set_capacitys(vec, vec->size);
	CU_ASSERT_EQUAL(vec->size, vec->capacity);

	for(i=999; i>=0; i--) {
		sprintf(buffer, "hello %d", i);
		pop_backs(vec, buffer2);
		CU_ASSERT_STRING_EQUAL(buffer, buffer2);
	}

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



void set_capacitys_test()
{
	int i;
	char buffer[50];

	vector_s* vec = vec_s(0);

	for(i=0; i<1000; i++) {
		sprintf(buffer, "hello %d", i);
		push_backs(vec, buffer);
	}

	CU_ASSERT( vec->capacity>=1000 );
	CU_ASSERT( vec->size==1000);

	set_capacitys(vec, 500);

	CU_ASSERT( vec->capacity==500 );
	CU_ASSERT( vec->size==500);

	for(i=0; i<vec->size; i++) {
		sprintf(buffer, "hello %d", i);
		CU_ASSERT_STRING_EQUAL(vec->a[i], buffer);
	}

	free_vecs(vec);
}


void set_vals_test()
{
	int i;
	vector_s* vec = vec_s(20);

	set_val_szs(vec, "42");

	for(i=0; i<vec->size; i++)
		CU_ASSERT_STRING_EQUAL(vec->a[i], "42");

	set_val_caps(vec, "25");

	for(i=0; i<vec->capacity; i++)
		CU_ASSERT_STRING_EQUAL(vec->a[i], "25");

	free_vecs(vec);
}





/* vector tests */

/** Structure used to test generic vector. */
typedef struct
{
	double d;
	int i;
	char word[30];
} t_struct;

/** Structure used to test generic vector. */
typedef struct
{
	double d;
	int i;
	char* word;
} f_struct;




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




#define GET_T(X,Y) ((t_struct*)&X->a[Y*X->elem_size])
#define GET_F(X,Y) ((f_struct*)&X->a[Y*X->elem_size])


void free_f_struct(void* tmp)
{
	f_struct* f = (f_struct*)tmp;
	if( f->word!=NULL ) {
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



void push_test()
{
	char buffer[50];
	int i;
	t_struct temp;
	f_struct temp2;

	vector* vec1 = vec(0, sizeof(t_struct), NULL, NULL);
	vector* vec2 = vec(0, sizeof(f_struct), free_f_struct, NULL);

	CU_ASSERT_EQUAL(VEC_START_SZ, vec1->capacity);
	CU_ASSERT_EQUAL(0, vec1->size);

	CU_ASSERT_EQUAL(VEC_START_SZ, vec2->capacity);
	CU_ASSERT_EQUAL(0, vec2->size);

	for(i=0; i<100; i++) {
		sprintf(buffer, "%d", i);
		temp = mk_t_struct(i, i, buffer);
		temp2 = mk_f_struct(i, i, buffer);

		push_back(vec1, &temp);
		push_back(vec2, &temp2);
	}

	CU_ASSERT_EQUAL(100, vec1->size);
	CU_ASSERT_EQUAL(100, vec2->size);


	for(i=0; i<vec1->size; i++) {

		sprintf(buffer, "%d", i);
		CU_ASSERT_EQUAL(i, GET_T(vec1, i)->d);
		CU_ASSERT_EQUAL(i, GET_T(vec1, i)->i);
		CU_ASSERT_STRING_EQUAL(buffer, GET_T(vec1, i)->word);

		CU_ASSERT_EQUAL(i, GET_F(vec2, i)->d);
		CU_ASSERT_EQUAL(i, GET_F(vec2, i)->i);
		CU_ASSERT_STRING_EQUAL(buffer, GET_F(vec2, i)->word);
	}

	free_vec(vec1);
	free_vec(vec2);
}


void erase_test()
{
	char buffer[50];
	int i,j;

	vector* vec1 = vec(100, sizeof(t_struct), NULL, NULL);
	vector* vec2 = vec(100, sizeof(f_struct), free_f_struct, NULL);

	CU_ASSERT_EQUAL(VEC_START_SZ+100, vec1->capacity);
	CU_ASSERT_EQUAL(100, vec1->size);

	CU_ASSERT_EQUAL(VEC_START_SZ+100, vec2->capacity);
	CU_ASSERT_EQUAL(100, vec2->size);

	for(i=0; i<100; i++) {
		sprintf(buffer, "%d", i);

		GET_T(vec1, i)->d = i+0.5;
		GET_T(vec1, i)->i = i;
		strcpy(GET_T(vec1, i)->word, buffer);

		GET_F(vec2, i)->d = i+0.5;
		GET_F(vec2, i)->i = i;
		GET_F(vec2, i)->word = mystrdup(buffer);
	}

	CU_ASSERT_EQUAL(100, vec1->size);
	CU_ASSERT_EQUAL(100, vec2->size);

	erase(vec1, 25, 74);
	erase(vec2, 25, 74);

	CU_ASSERT_EQUAL(50, vec1->size);
	CU_ASSERT_EQUAL(50, vec2->size);

	for(i=0,j=0; i<vec1->size; i++,j++) {
		sprintf(buffer, "%d", j);
		CU_ASSERT_EQUAL(GET_T(vec1, i)->d, j+0.5);
		CU_ASSERT_EQUAL(GET_T(vec1, i)->i, j);
		CU_ASSERT_STRING_EQUAL(GET_T(vec1, i)->word, buffer);

		CU_ASSERT_EQUAL(GET_F(vec2, i)->d, j+0.5);
		CU_ASSERT_EQUAL(GET_F(vec2, i)->i, j);
		CU_ASSERT_STRING_EQUAL(GET_F(vec2, i)->word, buffer);

		if(i==24) j +=50;
	}

	free_vec(vec1);
	free_vec(vec2);
}



void insert_test()
{
	t_struct array[10];
	f_struct array2[10];
	int i;
	char buffer[50];

	vector *vec, *vec2;

	t_struct temp;
	f_struct temp2;

	for(i=0; i<10; i++) {
		sprintf(buffer, "hello %d", i);
		array[i].d = i+0.5;
		array[i].i = i;
		strcpy(array[i].word, buffer);

		array2[i].d = i+0.5;
		array2[i].i = i;
		array2[i].word = mystrdup(buffer);
	}


	vec = init_vec(array, 10, sizeof(t_struct), NULL, NULL);
	vec2 = init_vec(array2, 10, sizeof(f_struct), free_f_struct, NULL);


	CU_ASSERT_EQUAL(vec->size, 10);
	CU_ASSERT_EQUAL(vec2->size, 10);


	for(i=0; i<vec->size; i++) {
		sprintf(buffer, "hello %d", i);
		CU_ASSERT_EQUAL(GET_T(vec, i)->d, i+0.5);
		CU_ASSERT_EQUAL(GET_T(vec, i)->i, i);
		CU_ASSERT_STRING_EQUAL(GET_T(vec, i)->word, buffer);

		CU_ASSERT_EQUAL(GET_F(vec2, i)->d, i+0.5);
		CU_ASSERT_EQUAL(GET_F(vec2, i)->i, i);
		CU_ASSERT_STRING_EQUAL(GET_F(vec2, i)->word, buffer);
	}

	for(i=0; i<10; i++) {
		sprintf(buffer, "hello %d", -i);
		temp = mk_t_struct(-i-0.5, -i, buffer);
		temp2 = mk_f_struct(-i-0.5, -i, buffer);

		insert(vec, 0, &temp);
		insert(vec2, 0, &temp2);
	}

	CU_ASSERT_EQUAL(vec->size, 20);
	CU_ASSERT_EQUAL(vec2->size, 20);


	for(i=0; i<vec->size; i++) {
		sprintf(buffer, "hello %d", i-((i<10)? 9 : 10));

		CU_ASSERT_EQUAL(GET_T(vec, i)->d, i-0.5-9 );
		CU_ASSERT_EQUAL(GET_T(vec, i)->i, i-((i<10)? 9 : 10) );
		CU_ASSERT_STRING_EQUAL(GET_T(vec, i)->word, buffer);

		CU_ASSERT_EQUAL(GET_F(vec2, i)->d, i-0.5-9 );
		CU_ASSERT_EQUAL(GET_F(vec2, i)->i, i-((i<10)? 9 : 10) );
		CU_ASSERT_STRING_EQUAL(GET_F(vec2, i)->word, buffer);

	}

	free_vec(vec);
	free_vec(vec2);
}



void pop_test()
{
	char buffer[50];
	int i;
	t_struct temp;
	f_struct temp2;

	vector* vec = init_vec(NULL, 10, sizeof(t_struct), NULL, NULL);
	vector* vec2 = init_vec(NULL, 10, sizeof(f_struct), free_f_struct, NULL);

	CU_ASSERT_EQUAL(vec->capacity, 10+VEC_START_SZ);
	CU_ASSERT_EQUAL(vec2->capacity, 10+VEC_START_SZ);

	for(i=0; i<1000; i++) {
		sprintf(buffer, "hello %d", i);
		temp = mk_t_struct(i, i, buffer);
		temp2 = mk_f_struct(i, i, buffer);

		push_back(vec, &temp);
		push_back(vec2, &temp2);
	}

	set_capacity(vec, vec->size);
	CU_ASSERT_EQUAL(vec->size, vec->capacity);
	CU_ASSERT_EQUAL(vec->size, 1000);

	set_capacity(vec2, vec2->size);
	CU_ASSERT_EQUAL(vec2->size, vec2->capacity);
	CU_ASSERT_EQUAL(vec2->size, 1000);



	for(i=999; i>=0; i--) {
		sprintf(buffer, "hello %d", i);
		pop_back(vec, &temp);
		pop_back(vec2, &temp2);

		CU_ASSERT_EQUAL(temp.d, i );
		CU_ASSERT_EQUAL(temp.i, i );
		CU_ASSERT_STRING_EQUAL(temp.word, buffer);

		CU_ASSERT_EQUAL(temp2.d, i );
		CU_ASSERT_EQUAL(temp2.i, i );
		/*No CU_ASSERT_STRING_EQUAL here because the string
		was freed when it was popped */
	}

	CU_ASSERT_EQUAL(vec->size, 0);
	CU_ASSERT_EQUAL(vec2->size, 0);

	free_vec(vec);
	free_vec(vec2);
}


void reserve_test()
{
	vector* vect = vec(0, sizeof(t_struct), NULL, NULL);
	vector* vec2 = vec(0, sizeof(f_struct), NULL, NULL);

	reserve(vect, 20);
	reserve(vec2, 20);

	CU_ASSERT( vect->capacity>=20 );
	CU_ASSERT( vec2->capacity>=20 );

	free_vec(vect);
	free_vec(vec2);
}


void set_capacity_test()
{
	char buffer[50];
	int i;
	t_struct temp;
	f_struct temp2;
	vector* vec1 = vec(0, sizeof(t_struct), NULL, NULL);
	vector* vec2 = vec(0, sizeof(f_struct), free_f_struct, NULL);

	for(i=0; i<1000; i++) {
		sprintf(buffer, "hello %d", i);
		temp = mk_t_struct(i, i, buffer);
		temp2 = mk_f_struct(i, i, buffer);

		push_back(vec1, &temp);
		push_back(vec2, &temp2);
	}

	CU_ASSERT( vec1->capacity>=1000 );
	CU_ASSERT( vec1->size==1000);

	CU_ASSERT( vec2->capacity>=1000 );
	CU_ASSERT( vec2->size==1000);

	set_capacity(vec1, 500);
	set_capacity(vec2, 500);


	CU_ASSERT( vec1->capacity==500 );
	CU_ASSERT( vec1->size==500 );

	CU_ASSERT( vec2->capacity==500 );
	CU_ASSERT( vec2->size==500 );

	for(i=0; i<vec1->size; i++) {
		sprintf(buffer, "hello %d", i);

		CU_ASSERT_EQUAL(GET_T(vec1, i)->d, i );
		CU_ASSERT_EQUAL(GET_T(vec1, i)->i, i );
		CU_ASSERT_STRING_EQUAL(GET_T(vec1, i)->word, buffer);

		CU_ASSERT_EQUAL(GET_F(vec2, i)->d, i );
		CU_ASSERT_EQUAL(GET_F(vec2, i)->i, i );
		CU_ASSERT_STRING_EQUAL(GET_F(vec2, i)->word, buffer);
	}

	free_vec(vec1);
	free_vec(vec2);
}




void set_val_test()
{
	int i;
	vector* vec1 = vec(20, sizeof(t_struct), NULL, NULL);
	vector* vec2 = vec(20, sizeof(f_struct), free_f_struct, init_f_struct);
	t_struct temp;
	f_struct temp2;


	CU_ASSERT_EQUAL(vec1->size, 20);
	CU_ASSERT_EQUAL(vec2->size, 20);

	CU_ASSERT_EQUAL(vec1->capacity, 20+VEC_START_SZ);
	CU_ASSERT_EQUAL(vec2->capacity, 20+VEC_START_SZ);

	temp = mk_t_struct(42.5, 42, "hello");
	temp2 = mk_f_struct(42.5, 42, "hello");

	set_val_sz(vec1, &temp);
	set_val_sz(vec2, &temp2);

	free_f_struct(&temp2);

	for(i=0; i<vec1->size; i++) {
		CU_ASSERT_EQUAL(GET_T(vec1, i)->d, 42.5);
		CU_ASSERT_EQUAL(GET_T(vec1, i)->i, 42);
		CU_ASSERT_STRING_EQUAL(GET_T(vec1, i)->word, "hello");

		CU_ASSERT_EQUAL(GET_F(vec2, i)->d, 42.5);
		CU_ASSERT_EQUAL(GET_F(vec2, i)->i, 42);
		CU_ASSERT_STRING_EQUAL(GET_F(vec2, i)->word, "hello");
	}

	temp = mk_t_struct(25.5, 25, "goodbye");
	temp2 = mk_f_struct(25.5, 25, "goodbye");

	set_val_cap(vec1, &temp);
	set_val_cap(vec2, &temp2);

	free_f_struct(&temp2);

	/*difference here between having a free function and not
	if yes then size is set to capacity by set_val_cap. */
	CU_ASSERT_EQUAL(vec1->size, 20);
	CU_ASSERT_EQUAL(vec1->capacity, 20+VEC_START_SZ);

	CU_ASSERT_EQUAL(vec2->size, vec2->capacity);
	CU_ASSERT_EQUAL(vec2->capacity, 20+VEC_START_SZ);


	for(i=0; i<vec1->capacity; i++) {
		CU_ASSERT_EQUAL(GET_T(vec1, i)->d, 25.5);
		CU_ASSERT_EQUAL(GET_T(vec1, i)->i, 25);
		CU_ASSERT_STRING_EQUAL(GET_T(vec1, i)->word, "goodbye");

		CU_ASSERT_EQUAL(GET_F(vec2, i)->d, 25.5);
		CU_ASSERT_EQUAL(GET_F(vec2, i)->i, 25);
		CU_ASSERT_STRING_EQUAL(GET_F(vec2, i)->word, "goodbye");
	}

	free_vec(vec1);
	free_vec(vec2);
}



















