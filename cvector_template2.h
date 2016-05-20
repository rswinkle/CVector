#ifndef CVECTOR_TYPE_H
#define CVECTOR_TYPE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#ifdef __cplusplus
extern "C" {
#endif


typedef struct cvector_TYPE
{
	TYPE* a;
	size_t size;
	size_t capacity;
	void (*elem_init)(void*, void*);
	void (*elem_free)(void*);
} cvector_TYPE;

extern size_t CVEC_TYPE_SZ;

int cvec_TYPE(cvector_TYPE* vec, size_t size, size_t capacity, void(*elem_free)(void*), void(*elem_init)(void*, void*));
int cvec_init_TYPE(cvector_TYPE* vec, TYPE* vals, size_t num, void(*elem_free)(void*), void(*elem_init)(void*, void*));

cvector_TYPE* cvec_TYPE_heap(size_t size, size_t capacity, void (*elem_free)(void*), void(*elem_init)(void*, void*));
cvector_TYPE* cvec_init_TYPE_heap(TYPE* vals, size_t num, void (*elem_free)(void*), void(*elem_init)(void*, void*));

void cvec_TYPE_copy(void* dest, void* src);

int cvec_push_TYPE(cvector_TYPE* vec, TYPE* val);
void cvec_pop_TYPE(cvector_TYPE* vec, TYPE* ret);

int cvec_extend_TYPE(cvector_TYPE* vec, size_t num);
int cvec_insert_TYPE(cvector_TYPE* vec, size_t i, TYPE* a);
int cvec_insert_array_TYPE(cvector_TYPE* vec, size_t i, TYPE* a, size_t num);
void cvec_replace_TYPE(cvector_TYPE* vec, size_t i, TYPE* a, TYPE* ret);
void cvec_erase_TYPE(cvector_TYPE* vec, size_t start, size_t end);
int cvec_reserve_TYPE(cvector_TYPE* vec, size_t size);
int cvec_set_cap_TYPE(cvector_TYPE* vec, size_t size);
void cvec_set_val_sz_TYPE(cvector_TYPE* vec, TYPE* val);
void cvec_set_val_cap_TYPE(cvector_TYPE* vec, TYPE* val);

TYPE* cvec_back_TYPE(cvector_TYPE* vec);

void cvec_clear_TYPE(cvector_TYPE* vec);
void cvec_free_TYPE_heap(void* vec);
void cvec_free_TYPE(void* vec);

#ifdef __cplusplus
}
#endif


/* CVECTOR_TYPE_H */
#endif

#ifdef CVECTOR_TYPE_IMPLEMENTATION

#include <assert.h>


size_t CVEC_TYPE_SZ = 20;


#define CVEC_TYPE_ALLOCATOR(x) ((x) * 2)






/*  general vector */

cvector_TYPE* cvec_TYPE_heap(size_t size, size_t capacity, void(*elem_free)(void*), void(*elem_init)(void*, void*))
{
	cvector_TYPE* vec;
	if (!(vec = (cvector_TYPE*)malloc(sizeof(cvector_TYPE)))) {
		assert(vec != NULL);
		return NULL;
	}

	vec->size = size;
	vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + CVEC_TYPE_SZ;

	/*not calloc here and init_vec as in vector_s because elem_free cannot be calling free directly*/
	if (!(vec->a = (TYPE*)malloc(vec->capacity * sizeof(TYPE)))) {
		assert(vec->a != NULL);
		free(vec);
		return NULL;
	}

	vec->elem_free = elem_free;
	vec->elem_init = elem_init;

	return vec;
}

cvector_TYPE* cvec_init_TYPE_heap(TYPE* vals, size_t num, void(*elem_free)(void*), void(*elem_init)(void*, void*))
{
	cvector_TYPE* vec;
	size_t i;
	
	if (!(vec = (cvector_TYPE*)malloc(sizeof(cvector_TYPE)))) {
		assert(vec != NULL);
		return NULL;
	}

	vec->capacity = num + CVEC_TYPE_SZ;
	vec->size = num;
	if (!(vec->a = (TYPE*)malloc(vec->capacity * sizeof(TYPE)))) {
		assert(vec->a != NULL);
		free(vec);
		return NULL;
	}

	if (elem_init) {
		for (i=0; i<num; ++i) {
			elem_init(&vec->a[i], &vals[i]);
		}
	} else {
		memcpy(vec->a, vals, sizeof(TYPE)*num);
	}
	
	vec->elem_free = elem_free;
	vec->elem_init = elem_init;

	return vec;
}

int cvec_TYPE(cvector_TYPE* vec, size_t size, size_t capacity, void(*elem_free)(void*), void(*elem_init)(void*, void*))
{
	vec->size = size;
	vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + CVEC_TYPE_SZ;

	if (!(vec->a = (TYPE*)malloc(vec->capacity * sizeof(TYPE)))) {
		assert(vec->a != NULL);
		vec->size = vec->capacity = 0;
		return 0;
	}

	vec->elem_free = elem_free;
	vec->elem_init = elem_init;

	return 1;
}

int cvec_init_TYPE(cvector_TYPE* vec, TYPE* vals, size_t num, void(*elem_free)(void*), void(*elem_init)(void*, void*))
{
	size_t i;
	
	vec->capacity = num + CVEC_TYPE_SZ;
	vec->size = num;
	if (!(vec->a = (TYPE*)malloc(vec->capacity * sizeof(TYPE)))) {
		assert(vec->a != NULL);
		vec->size = vec->capacity = 0;
		return 0;
	}

	if (elem_init) {
		for (i=0; i<num; ++i) {
			elem_init(&vec->a[i], &vals[i]);
		}
	} else {
		memcpy(vec->a, vals, sizeof(TYPE)*num);
	}

	vec->elem_free = elem_free;
	vec->elem_init = elem_init;

	return 1;
}


void cvec_TYPE_copy(void* dest, void* src)
{
	size_t i;
	cvector_TYPE* vec1 = (cvector_TYPE*)dest;
	cvector_TYPE* vec2 = (cvector_TYPE*)src;
	
	vec1->size = 0;
	vec1->capacity = 0;
	
	/*not much else we can do here*/
	if (!(vec1->a = (TYPE*)malloc(vec2->capacity*sizeof(TYPE)))) {
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
		memcpy(vec1->a, vec2->a, vec1->size*sizeof(TYPE));
	}
}

int cvec_push_TYPE(cvector_TYPE* vec, TYPE* a)
{
	TYPE* tmp;
	size_t tmp_sz;
	if (vec->capacity > vec->size) {
		if (vec->elem_init) {
			vec->elem_init(&vec->a[vec->size], a);
		} else {
			memcpy(&vec->a[vec->size], a, sizeof(TYPE));
		}
	} else {
		tmp_sz = CVEC_TYPE_ALLOCATOR(vec->capacity);
		if (!(tmp = (TYPE*)realloc(vec->a, sizeof(TYPE)*tmp_sz))) {
			assert(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		
		if (vec->elem_init) {
			vec->elem_init(&vec->a[vec->size], a);
		} else {
			memcpy(&vec->a[vec->size], a, sizeof(TYPE));
		}
		
		vec->capacity = tmp_sz;
	}
	
	vec->size++;
	return 1;
}

void cvec_pop_TYPE(cvector_TYPE* vec, TYPE* ret)
{
	if (ret) {
		memcpy(ret, &vec->a[--vec->size], sizeof(TYPE));
	} else {
		vec->size--;
	}

	if (vec->elem_free) {
		vec->elem_free(&vec->a[vec->size]);
	}
}

/** Return pointer to last element */
TYPE* cvec_back_TYPE(cvector_TYPE* vec)
{
	return &vec->a[vec->size-1];
}

int cvec_extend_TYPE(cvector_TYPE* vec, size_t num)
{
	TYPE* tmp;
	size_t tmp_sz;
	if (vec->capacity < vec->size + num) {
		tmp_sz = vec->capacity + num + CVEC_TYPE_SZ;
		if (!(tmp = (TYPE*)realloc(vec->a, sizeof(TYPE)*tmp_sz))) {
			assert(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}

	vec->size += num;
	return 1;
}

int cvec_insert_TYPE(cvector_TYPE* vec, size_t i, TYPE* a)
{
	TYPE* tmp;
	size_t tmp_sz;
	if (vec->capacity > vec->size) {
		memmove(&vec->a[i+1], &vec->a[i], (vec->size-i)*sizeof(TYPE));

		if (vec->elem_init) {
			vec->elem_init(&vec->a[i], a);
		} else {
			memcpy(&vec->a[i], a, sizeof(TYPE));
		}
	} else {
		tmp_sz = CVEC_TYPE_ALLOCATOR(vec->capacity);
		if (!(tmp = (TYPE*)realloc(vec->a, sizeof(TYPE)*tmp_sz))) {
			assert(tmp != NULL);
			return 0;
		}
		
		vec->a = tmp;
		memmove(&vec->a[i+1], &vec->a[i], (vec->size-i)*sizeof(TYPE));
		
		if (vec->elem_init) {
			vec->elem_init(&vec->a[i], a);
		} else {
			memcpy(&vec->a[i], a, sizeof(TYPE));
		}
		
		vec->capacity = tmp_sz;
	}

	vec->size++;
	return 1;
}

int cvec_insert_array_TYPE(cvector_TYPE* vec, size_t i, TYPE* a, size_t num)
{
	TYPE* tmp;
	size_t tmp_sz, j;
	if (vec->capacity < vec->size + num) {
		tmp_sz = vec->capacity + num + CVEC_TYPE_SZ;
		if (!(tmp = (TYPE*)realloc(vec->a, sizeof(TYPE)*tmp_sz))) {
			assert(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}

	memmove(&vec->a[i+num], &vec->a[i], (vec->size-i)*sizeof(TYPE));
	if (vec->elem_init) {
		for (j=0; j<num; ++j) {
			vec->elem_init(&vec->a[j+i], &a[j]);
		}
	} else {
		memcpy(&vec->a[i], a, num*sizeof(TYPE));
	}
	vec->size += num;
	return 1;
}

void cvec_replace_TYPE(cvector_TYPE* vec, size_t i, TYPE* a, TYPE* ret)
{
	if (ret)
		memmove(ret, &vec->a[i], sizeof(TYPE));
	memmove(&vec->a[i], a, sizeof(TYPE));
}

void cvec_erase_TYPE(cvector_TYPE* vec, size_t start, size_t end)
{
	size_t i;
	size_t d = end - start + 1;
	if (vec->elem_free) {
		for (i=start; i<=end; i++) {
			vec->elem_free(&vec->a[i]);
		}
	}
	memmove(&vec->a[start], &vec->a[end+1], (vec->size-1-end)*sizeof(TYPE));
	vec->size -= d;
}

int cvec_reserve_TYPE(cvector_TYPE* vec, size_t size)
{
	TYPE* tmp;
	if (vec->capacity < size) {
		if (!(tmp = (TYPE*)realloc(vec->a, sizeof(TYPE)*(size+CVEC_TYPE_SZ)))) {
			assert(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = size + CVEC_TYPE_SZ;
	}
	return 1;
}

int cvec_set_cap_TYPE(cvector_TYPE* vec, size_t size)
{
	size_t i;
	TYPE* tmp;
	if (size < vec->size) {
		if (vec->elem_free) {
			for (i=vec->size-1; i>=size; i--) {
				vec->elem_free(&vec->a[i]);
			}
		}
		vec->size = size;
	}

	vec->capacity = size;

	if (!(tmp = (TYPE*)realloc(vec->a, sizeof(TYPE)*size))) {
		assert(tmp != NULL);
		return 0;
	}
	vec-> a = tmp;
	return 1;
}

void cvec_set_val_sz_TYPE(cvector_TYPE* vec, TYPE* val)
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
			memcpy(&vec->a[i], val, sizeof(TYPE));
		}
	}
}

void cvec_set_val_cap_TYPE(cvector_TYPE* vec, TYPE* val)
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
			memcpy(&vec->a[i], val, sizeof(TYPE));
		}
	}
}

void cvec_clear_TYPE(cvector_TYPE* vec)
{
	size_t i;
	if (vec->elem_free) {
		for (i=0; i<vec->size; ++i) {
			vec->elem_free(&vec->a[i]);
		}
	}
	vec->size = 0;
}

void cvec_free_TYPE_heap(void* vec)
{
	size_t i;
	cvector_TYPE* tmp = (cvector_TYPE*)vec;
	if (tmp->elem_free) {
		for (i=0; i<tmp->size; i++) {
			tmp->elem_free(&tmp->a[i]);
		}
	}
	free(tmp->a);
	free(tmp);
}

void cvec_free_TYPE(void* vec)
{
	size_t i;
	cvector_TYPE* tmp = (cvector_TYPE*)vec;
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
