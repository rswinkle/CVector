#ifndef CVECTOR_TYPE_H
#define CVECTOR_TYPE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Data structure for TYPE vector. */
typedef struct cvector_TYPE
{
	TYPE* a;           /**< Array. */
	size_t size;       /**< Current size (amount you use when manipulating array directly). */
	size_t capacity;   /**< Allocated size of array; always >= size. */
} cvector_TYPE;



extern size_t CVEC_TYPE_SZ;

int cvec_TYPE(cvector_TYPE* vec, size_t size, size_t capacity);
int cvec_init_TYPE(cvector_TYPE* vec, TYPE* vals, size_t num);

cvector_TYPE* cvec_TYPE_heap(size_t size, size_t capacity);
cvector_TYPE* cvec_init_TYPE_heap(TYPE* vals, size_t num);

void cvec_TYPE_copy(void* dest, void* src);

int cvec_push_TYPE(cvector_TYPE* vec, TYPE a);
TYPE cvec_pop_TYPE(cvector_TYPE* vec);

int cvec_extend_TYPE(cvector_TYPE* vec, size_t num);
int cvec_insert_TYPE(cvector_TYPE* vec, size_t i, TYPE a);
int cvec_insert_array_TYPE(cvector_TYPE* vec, size_t i, TYPE* a, size_t num);
TYPE cvec_replace_TYPE(cvector_TYPE* vec, size_t i, TYPE a);
void cvec_erase_TYPE(cvector_TYPE* vec, size_t start, size_t end);
int cvec_reserve_TYPE(cvector_TYPE* vec, size_t size);
int cvec_set_cap_TYPE(cvector_TYPE* vec, size_t size);
void cvec_set_val_sz_TYPE(cvector_TYPE* vec, TYPE val);
void cvec_set_val_cap_TYPE(cvector_TYPE* vec, TYPE val);

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

#define CVEC_TYPE_ALLOCATOR(x) ((x) * 2)

size_t CVEC_TYPE_SZ = 50;

cvector_TYPE* cvec_TYPE_heap(size_t size, size_t capacity)
{
	cvector_TYPE* vec;
	if (!(vec = (cvector_TYPE*)malloc(sizeof(cvector_TYPE)))) {
		assert(vec != NULL);
		return NULL;
	}

	vec->size = size;
	vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + CVEC_TYPE_SZ;

	if (!(vec->a = (TYPE*)malloc(vec->capacity*sizeof(TYPE)))) {
		assert(vec->a != NULL);
		free(vec);
		return NULL;
	}

	return vec;
}

cvector_TYPE* cvec_init_TYPE_heap(TYPE* vals, size_t num)
{
	cvector_TYPE* vec;
	
	if (!(vec = (cvector_TYPE*)malloc(sizeof(cvector_TYPE)))) {
		assert(vec != NULL);
		return NULL;
	}

	vec->capacity = num + CVEC_TYPE_SZ;
	vec->size = num;
	if (!(vec->a = (TYPE*)malloc(vec->capacity*sizeof(TYPE)))) {
		assert(vec->a != NULL);
		free(vec);
		return NULL;
	}

	memcpy(vec->a, vals, sizeof(TYPE)*num);

	return vec;
}

int cvec_TYPE(cvector_TYPE* vec, size_t size, size_t capacity)
{
	vec->size = size;
	vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + CVEC_TYPE_SZ;

	if (!(vec->a = (TYPE*)malloc(vec->capacity*sizeof(TYPE)))) {
		assert(vec->a != NULL);
		vec->size = vec->capacity = 0;
		return 0;
	}

	return 1;
}

int cvec_init_TYPE(cvector_TYPE* vec, TYPE* vals, size_t num)
{
	vec->capacity = num + CVEC_TYPE_SZ;
	vec->size = num;
	if (!(vec->a = (TYPE*)malloc(vec->capacity*sizeof(TYPE)))) {
		assert(vec->a != NULL);
		vec->size = vec->capacity = 0;
		return 0;
	}

	memcpy(vec->a, vals, sizeof(TYPE)*num);

	return 1;
}

void cvec_TYPE_copy(void* dest, void* src)
{
	cvector_TYPE* vec1 = (cvector_TYPE*)dest;
	cvector_TYPE* vec2 = (cvector_TYPE*)src;
	
	vec1->size = 0;
	vec1->capacity = 0;
	
	/*not much else we can do here*/
	if (!(vec1->a = (TYPE*)malloc(vec2->capacity*sizeof(TYPE)))) {
		assert(vec1->a != NULL);
		return;
	}
	
	memcpy(vec1->a, vec2->a, vec2->size*sizeof(TYPE));
	vec1->size = vec2->size;
	vec1->capacity = vec2->capacity;
}


int cvec_push_TYPE(cvector_TYPE* vec, TYPE a)
{
	TYPE* tmp;
	size_t tmp_sz;
	if (vec->capacity > vec->size) {
		vec->a[vec->size++] = a;
	} else {
		tmp_sz = CVEC_TYPE_ALLOCATOR(vec->capacity);
		if (!(tmp = (TYPE*)realloc(vec->a, sizeof(TYPE)*tmp_sz))) {
			assert(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->a[vec->size++] = a;
		vec->capacity = tmp_sz;
	}
	return 1;
}

TYPE cvec_pop_TYPE(cvector_TYPE* vec)
{
	return vec->a[--vec->size];
}

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

int cvec_insert_TYPE(cvector_TYPE* vec, size_t i, TYPE a)
{
	TYPE* tmp;
	size_t tmp_sz;
	if (vec->capacity > vec->size) {
		memmove(&vec->a[i+1], &vec->a[i], (vec->size-i)*sizeof(TYPE));
		vec->a[i] = a;
	} else {
		tmp_sz = CVEC_TYPE_ALLOCATOR(vec->capacity);
		if (!(tmp = (TYPE*)realloc(vec->a, sizeof(TYPE)*tmp_sz))) {
			assert(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		memmove(&vec->a[i+1], &vec->a[i], (vec->size-i)*sizeof(TYPE));
		vec->a[i] = a;
		vec->capacity = tmp_sz;
	}

	vec->size++;
	return 1;
}

int cvec_insert_array_TYPE(cvector_TYPE* vec, size_t i, TYPE* a, size_t num)
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

	memmove(&vec->a[i+num], &vec->a[i], (vec->size-i)*sizeof(TYPE));
	memcpy(&vec->a[i], a, num*sizeof(TYPE));
	vec->size += num;
	return 1;
}

TYPE cvec_replace_TYPE(cvector_TYPE* vec, size_t i, TYPE a)
{
	TYPE tmp = vec->a[i];
	vec->a[i] = a;
	return tmp;
}

void cvec_erase_TYPE(cvector_TYPE* vec, size_t start, size_t end)
{
	size_t d = end - start + 1;
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
	TYPE* tmp;
	if (size < vec->size) {
		vec->size = size;
	}

	if (!(tmp = (TYPE*)realloc(vec->a, sizeof(TYPE)*size))) {
		assert(tmp != NULL);
		return 0;
	}
	vec->a = tmp;
	vec->capacity = size;
	return 1;
}

void cvec_set_val_sz_TYPE(cvector_TYPE* vec, TYPE val)
{
	size_t i;
	for (i=0; i<vec->size; i++) {
		vec->a[i] = val;
	}
}

void cvec_set_val_cap_TYPE(cvector_TYPE* vec, TYPE val)
{
	size_t i;
	for (i=0; i<vec->capacity; i++) {
		vec->a[i] = val;
	}
}

void cvec_clear_TYPE(cvector_TYPE* vec) { vec->size = 0; }

void cvec_free_TYPE_heap(void* vec)
{
	cvector_TYPE* tmp = (cvector_TYPE*)vec;
	free(tmp->a);
	free(tmp);
}

void cvec_free_TYPE(void* vec)
{
	cvector_TYPE* tmp = (cvector_TYPE*)vec;
	free(tmp->a);
	tmp->size = 0;
	tmp->capacity = 0;
}

#endif
