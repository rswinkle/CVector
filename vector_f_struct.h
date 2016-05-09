#ifndef VECTOR_f_struct_H
#define VECTOR_f_struct_H

#include "test_types.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#ifdef __cplusplus
extern "C" {
#endif


typedef struct vector_f_struct
{
	f_struct* a;
	size_t size;
	size_t capacity;
	void (*elem_init)(void*, void*);
	void (*elem_free)(void*);
} vector_f_struct;

extern size_t VEC_f_struct_SZ;

int vec_f_struct(vector_f_struct* vec, size_t size, size_t capacity, void(*elem_free)(void*), void(*elem_init)(void*, void*));
int init_vec_f_struct(vector_f_struct* vec, f_struct* vals, size_t num, void(*elem_free)(void*), void(*elem_init)(void*, void*));

vector_f_struct* vec_f_struct_heap(size_t size, size_t capacity, void (*elem_free)(void*), void(*elem_init)(void*, void*));
vector_f_struct* init_vec_f_struct_heap(f_struct* vals, size_t num, void (*elem_free)(void*), void(*elem_init)(void*, void*));

void vec_f_struct_copy(void* dest, void* src);

int push_f_struct(vector_f_struct* vec, f_struct* val);
void pop_f_struct(vector_f_struct* vec, f_struct* ret);

int extend_f_struct(vector_f_struct* vec, size_t num);
int insert_f_struct(vector_f_struct* vec, size_t i, f_struct* a);
int insert_array_f_struct(vector_f_struct* vec, size_t i, f_struct* a, size_t num);
void replace_f_struct(vector_f_struct* vec, size_t i, f_struct* a, f_struct* ret);
void erase_f_struct(vector_f_struct* vec, size_t start, size_t end);
int reserve_f_struct(vector_f_struct* vec, size_t size);
int set_capacity_f_struct(vector_f_struct* vec, size_t size);
void set_val_sz_f_struct(vector_f_struct* vec, f_struct* val);
void set_val_cap_f_struct(vector_f_struct* vec, f_struct* val);

f_struct* back_f_struct(vector_f_struct* vec);

void clear_f_struct(vector_f_struct* vec);
void free_vec_f_struct_heap(void* vec);
void free_vec_f_struct(void* vec);

#ifdef __cplusplus
}
#endif


/* VECTOR_f_struct_H */
#endif

#ifdef VECTOR_f_struct_IMPLEMENTATION

#include <assert.h>

#define STDERR(X) fprintf(stderr, X)


size_t VEC_f_struct_SZ = 20;


#define VEC_f_struct_ALLOCATOR(x) ((x) * 2)






/*  general vector */

vector_f_struct* vec_f_struct_heap(size_t size, size_t capacity, void(*elem_free)(void*), void(*elem_init)(void*, void*))
{
	vector_f_struct* vec;
	if (!(vec = (vector_f_struct*)malloc(sizeof(vector_f_struct)))) {
		assert(vec != NULL);
		return NULL;
	}

	vec->size = size;
	vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + VEC_f_struct_SZ;

	/*not calloc here and init_vec as in vector_s because elem_free cannot be calling free directly*/
	if (!(vec->a = (f_struct*)malloc(vec->capacity * sizeof(f_struct)))) {
		assert(vec->a != NULL);
		free(vec);
		return NULL;
	}

	vec->elem_free = elem_free;
	vec->elem_init = elem_init;

	return vec;
}



vector_f_struct* init_vec_f_struct_heap(f_struct* vals, size_t num, void(*elem_free)(void*), void(*elem_init)(void*, void*))
{
	vector_f_struct* vec;
	size_t i;
	
	if (!(vec = (vector_f_struct*)malloc(sizeof(vector_f_struct)))) {
		assert(vec != NULL);
		return NULL;
	}

	vec->capacity = num + VEC_f_struct_SZ;
	vec->size = num;
	if (!(vec->a = (f_struct*)malloc(vec->capacity * sizeof(f_struct)))) {
		assert(vec->a != NULL);
		free(vec);
		return NULL;
	}

	if (elem_init) {
		for (i=0; i<num; ++i) {
			elem_init(&vec->a[i], &vals[i]);
		}
	} else {
		memcpy(vec->a, vals, sizeof(f_struct)*num);
	}
	
	vec->elem_free = elem_free;
	vec->elem_init = elem_init;

	return vec;
}

int vec_f_struct(vector_f_struct* vec, size_t size, size_t capacity, void(*elem_free)(void*), void(*elem_init)(void*, void*))
{
	vec->size = size;
	vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + VEC_f_struct_SZ;

	if (!(vec->a = (f_struct*)malloc(vec->capacity * sizeof(f_struct)))) {
		assert(vec->a != NULL);
		vec->size = vec->capacity = 0;
		return 0;
	}

	vec->elem_free = elem_free;
	vec->elem_init = elem_init;

	return 1;
}

int init_vec_f_struct(vector_f_struct* vec, f_struct* vals, size_t num, void(*elem_free)(void*), void(*elem_init)(void*, void*))
{
	size_t i;
	
	vec->capacity = num + VEC_f_struct_SZ;
	vec->size = num;
	if (!(vec->a = (f_struct*)malloc(vec->capacity * sizeof(f_struct)))) {
		assert(vec->a != NULL);
		vec->size = vec->capacity = 0;
		return 0;
	}

	if (elem_init) {
		for (i=0; i<num; ++i) {
			elem_init(&vec->a[i], &vals[i]);
		}
	} else {
		memcpy(vec->a, vals, sizeof(f_struct)*num);
	}

	vec->elem_free = elem_free;
	vec->elem_init = elem_init;

	return 1;
}


void vec_f_struct_copy(void* dest, void* src)
{
	size_t i;
	vector_f_struct* vec1 = (vector_f_struct*)dest;
	vector_f_struct* vec2 = (vector_f_struct*)src;
	
	vec1->size = 0;
	vec1->capacity = 0;
	
	/*not much else we can do here*/
	if (!(vec1->a = (f_struct*)malloc(vec2->capacity*sizeof(f_struct)))) {
		assert(vec1->a != NULL);
		return;
	}

	vec1->size = vec2->size;
	vec1->capacity = vec2->capacity;
	vec1->elem_init = vec2->elem_init;
	vec1->elem_free = vec2->elem_free;
	
	if (vec1->elem_init) {
		for (i=0; i<vec1->size; ++i) {
			vec1->elem_init(&vec1->a[i], &vec2->a[i]);
		}
	} else {
		memcpy(vec1->a, vec2->a, vec1->size*sizeof(f_struct));
	}
}


int push_f_struct(vector_f_struct* vec, f_struct* a)
{
	f_struct* tmp;
	size_t tmp_sz;
	if (vec->capacity > vec->size) {
		if (vec->elem_init) {
			vec->elem_init(&vec->a[vec->size], a);
		} else {
			memcpy(&vec->a[vec->size], a, sizeof(f_struct));
		}
	} else {
		tmp_sz = VEC_f_struct_ALLOCATOR(vec->capacity);
		if (!(tmp = (f_struct*)realloc(vec->a, sizeof(f_struct)*tmp_sz))) {
			assert(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		
		if (vec->elem_init) {
			vec->elem_init(&vec->a[vec->size], a);
		} else {
			memcpy(&vec->a[vec->size], a, sizeof(f_struct));
		}
		
		vec->capacity = tmp_sz;
	}
	
	vec->size++;
	return 1;
}


void pop_f_struct(vector_f_struct* vec, f_struct* ret)
{
	if (ret) {
		memcpy(ret, &vec->a[--vec->size], sizeof(f_struct));
	} else {
		vec->size--;
	}

	if (vec->elem_free) {
		vec->elem_free(&vec->a[vec->size]);
	}
}

/** Return pointer to last element */
f_struct* back_f_struct(vector_f_struct* vec)
{
	return &vec->a[vec->size-1];
}




int extend_f_struct(vector_f_struct* vec, size_t num)
{
	f_struct* tmp;
	size_t tmp_sz;
	if (vec->capacity < vec->size + num) {
		tmp_sz = vec->capacity + num + VEC_f_struct_SZ;
		if (!(tmp = (f_struct*)realloc(vec->a, sizeof(f_struct)*tmp_sz))) {
			assert(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}

	vec->size += num;
	return 1;
}


int insert_f_struct(vector_f_struct* vec, size_t i, f_struct* a)
{
	f_struct* tmp;
	size_t tmp_sz;
	if (vec->capacity > vec->size) {
		memmove(&vec->a[i+1], &vec->a[i], (vec->size-i)*sizeof(f_struct));

		if (vec->elem_init) {
			vec->elem_init(&vec->a[i], a);
		} else {
			memcpy(&vec->a[i], a, sizeof(f_struct));
		}
	} else {
		tmp_sz = VEC_f_struct_ALLOCATOR(vec->capacity);
		if (!(tmp = (f_struct*)realloc(vec->a, sizeof(f_struct)*tmp_sz))) {
			assert(tmp != NULL);
			return 0;
		}
		
		vec->a = tmp;
		memmove(&vec->a[i+1], &vec->a[i], (vec->size-i)*sizeof(f_struct));
		
		if (vec->elem_init) {
			vec->elem_init(&vec->a[i], a);
		} else {
			memcpy(&vec->a[i], a, sizeof(f_struct));
		}
		
		vec->capacity = tmp_sz;
	}

	vec->size++;
	return 1;
}

int insert_array_f_struct(vector_f_struct* vec, size_t i, f_struct* a, size_t num)
{
	f_struct* tmp;
	size_t tmp_sz, j;
	if (vec->capacity < vec->size + num) {
		tmp_sz = vec->capacity + num + VEC_f_struct_SZ;
		if (!(tmp = (f_struct*)realloc(vec->a, sizeof(f_struct)*tmp_sz))) {
			assert(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}

	memmove(&vec->a[i+num], &vec->a[i], (vec->size-i)*sizeof(f_struct));
	if (vec->elem_init) {
		for (j=0; j<num; ++j) {
			vec->elem_init(&vec->a[j+i], &a[j]);
		}
	} else {
		memcpy(&vec->a[i], a, num*sizeof(f_struct));
	}
	vec->size += num;
	return 1;
}

void replace_f_struct(vector_f_struct* vec, size_t i, f_struct* a, f_struct* ret)
{
	if (ret)
		memmove(ret, &vec->a[i], sizeof(f_struct));
	memmove(&vec->a[i], a, sizeof(f_struct));
}

void erase_f_struct(vector_f_struct* vec, size_t start, size_t end)
{
	size_t i;
	size_t d = end - start + 1;
	if (vec->elem_free) {
		for (i=start; i<=end; i++) {
			vec->elem_free(&vec->a[i]);
		}
	}
	memmove(&vec->a[start], &vec->a[end+1], (vec->size-1-end)*sizeof(f_struct));
	vec->size -= d;
}


int reserve_f_struct(vector_f_struct* vec, size_t size)
{
	f_struct* tmp;
	if (vec->capacity < size) {
		if (!(tmp = (f_struct*)realloc(vec->a, sizeof(f_struct)*(size+VEC_f_struct_SZ)))) {
			assert(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = size + VEC_f_struct_SZ;
	}
	return 1;
}


int set_capacity_f_struct(vector_f_struct* vec, size_t size)
{
	size_t i;
	f_struct* tmp;
	if (size < vec->size) {
		if (vec->elem_free) {
			for (i=vec->size-1; i>=size; i--) {
				vec->elem_free(&vec->a[i]);
			}
		}
		vec->size = size;
	}

	vec->capacity = size;

	if (!(tmp = (f_struct*)realloc(vec->a, sizeof(f_struct)*size))) {
		assert(tmp != NULL);
		return 0;
	}
	vec-> a = tmp;
	return 1;
}



void set_val_sz_f_struct(vector_f_struct* vec, f_struct* val)
{
	size_t i;

	if (vec->elem_free) {
		for(i=0; i<vec->size; i++) {
			vec->elem_free(&vec->a[i]);
		}
	}
	
	if (vec->elem_init) {
		for (i=0; i<vec->size; i++) {
			vec->elem_init(&vec->a[i], val);
		}
	} else {
		for (i=0; i<vec->size; i++) {
			memcpy(&vec->a[i], val, sizeof(f_struct));
		}
	}
}


void set_val_cap_f_struct(vector_f_struct* vec, f_struct* val)
{
	size_t i;
	if (vec->elem_free) {
		for (i=0; i<vec->size; i++) {
			vec->elem_free(&vec->a[i]);
		}
		vec->size = vec->capacity;
	}

	if (vec->elem_init) {
		for (i=0; i<vec->capacity; i++) {
			vec->elem_init(&vec->a[i], val);
		}
	} else {
		for (i=0; i<vec->capacity; i++) {
			memcpy(&vec->a[i], val, sizeof(f_struct));
		}
	}
}


void clear_f_struct(vector_f_struct* vec)
{
	size_t i;
	if (vec->elem_free) {
		for (i=0; i<vec->size; ++i) {
			vec->elem_free(&vec->a[i]);
		}
	}
	vec->size = 0;
}


void free_vec_f_struct_heap(void* vec)
{
	size_t i;
	vector_f_struct* tmp = (vector_f_struct*)vec;
	if (tmp->elem_free) {
		for (i=0; i<tmp->size; i++) {
			tmp->elem_free(&tmp->a[i]);
		}
	}
	free(tmp->a);
	free(tmp);
}


void free_vec_f_struct(void* vec)
{
	size_t i;
	vector_f_struct* tmp = (vector_f_struct*)vec;
	if (tmp->elem_free) {
		for (i=0; i<tmp->size; i++) {
			tmp->elem_free(&tmp->a[i]);
		}
	}

	free(tmp->a);

	tmp->size = 0;
	tmp->capacity = 0;
}


#endif
