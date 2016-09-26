#ifndef CVECTOR_short_H
#define CVECTOR_short_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Data structure for short vector. */
typedef struct cvector_short
{
	short* a;           /**< Array. */
	size_t size;       /**< Current size (amount you use when manipulating array directly). */
	size_t capacity;   /**< Allocated size of array; always >= size. */
} cvector_short;



extern size_t CVEC_short_SZ;

int cvec_short(cvector_short* vec, size_t size, size_t capacity);
int cvec_init_short(cvector_short* vec, short* vals, size_t num);

cvector_short* cvec_short_heap(size_t size, size_t capacity);
cvector_short* cvec_init_short_heap(short* vals, size_t num);

void cvec_short_copy(void* dest, void* src);

int cvec_push_short(cvector_short* vec, short a);
short cvec_pop_short(cvector_short* vec);

int cvec_extend_short(cvector_short* vec, size_t num);
int cvec_insert_short(cvector_short* vec, size_t i, short a);
int cvec_insert_array_short(cvector_short* vec, size_t i, short* a, size_t num);
short cvec_replace_short(cvector_short* vec, size_t i, short a);
void cvec_erase_short(cvector_short* vec, size_t start, size_t end);
int cvec_reserve_short(cvector_short* vec, size_t size);
int cvec_set_cap_short(cvector_short* vec, size_t size);
void cvec_set_val_sz_short(cvector_short* vec, short val);
void cvec_set_val_cap_short(cvector_short* vec, short val);

short* cvec_back_short(cvector_short* vec);

void cvec_clear_short(cvector_short* vec);
void cvec_free_short_heap(void* vec);
void cvec_free_short(void* vec);

#ifdef __cplusplus
}
#endif

/* CVECTOR_short_H */
#endif


#ifdef CVECTOR_short_IMPLEMENTATION

#include <assert.h>

#define CVEC_short_ALLOCATOR(x) ((x+1) * 2)

size_t CVEC_short_SZ = 50;

cvector_short* cvec_short_heap(size_t size, size_t capacity)
{
	cvector_short* vec;
	if (!(vec = (cvector_short*)malloc(sizeof(cvector_short)))) {
		assert(vec != NULL);
		return NULL;
	}

	vec->size = size;
	vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + CVEC_short_SZ;

	if (!(vec->a = (short*)malloc(vec->capacity*sizeof(short)))) {
		assert(vec->a != NULL);
		free(vec);
		return NULL;
	}

	return vec;
}

cvector_short* cvec_init_short_heap(short* vals, size_t num)
{
	cvector_short* vec;
	
	if (!(vec = (cvector_short*)malloc(sizeof(cvector_short)))) {
		assert(vec != NULL);
		return NULL;
	}

	vec->capacity = num + CVEC_short_SZ;
	vec->size = num;
	if (!(vec->a = (short*)malloc(vec->capacity*sizeof(short)))) {
		assert(vec->a != NULL);
		free(vec);
		return NULL;
	}

	memmove(vec->a, vals, sizeof(short)*num);

	return vec;
}

int cvec_short(cvector_short* vec, size_t size, size_t capacity)
{
	vec->size = size;
	vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + CVEC_short_SZ;

	if (!(vec->a = (short*)malloc(vec->capacity*sizeof(short)))) {
		assert(vec->a != NULL);
		vec->size = vec->capacity = 0;
		return 0;
	}

	return 1;
}

int cvec_init_short(cvector_short* vec, short* vals, size_t num)
{
	vec->capacity = num + CVEC_short_SZ;
	vec->size = num;
	if (!(vec->a = (short*)malloc(vec->capacity*sizeof(short)))) {
		assert(vec->a != NULL);
		vec->size = vec->capacity = 0;
		return 0;
	}

	memmove(vec->a, vals, sizeof(short)*num);

	return 1;
}

void cvec_short_copy(void* dest, void* src)
{
	cvector_short* vec1 = (cvector_short*)dest;
	cvector_short* vec2 = (cvector_short*)src;
	
	vec1->size = 0;
	vec1->capacity = 0;
	
	/*not much else we can do here*/
	if (!(vec1->a = (short*)malloc(vec2->capacity*sizeof(short)))) {
		assert(vec1->a != NULL);
		return;
	}
	
	memmove(vec1->a, vec2->a, vec2->size*sizeof(short));
	vec1->size = vec2->size;
	vec1->capacity = vec2->capacity;
}


int cvec_push_short(cvector_short* vec, short a)
{
	short* tmp;
	size_t tmp_sz;
	if (vec->capacity > vec->size) {
		vec->a[vec->size++] = a;
	} else {
		tmp_sz = CVEC_short_ALLOCATOR(vec->capacity);
		if (!(tmp = (short*)realloc(vec->a, sizeof(short)*tmp_sz))) {
			assert(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->a[vec->size++] = a;
		vec->capacity = tmp_sz;
	}
	return 1;
}

short cvec_pop_short(cvector_short* vec)
{
	return vec->a[--vec->size];
}

short* cvec_back_short(cvector_short* vec)
{
	return &vec->a[vec->size-1];
}

int cvec_extend_short(cvector_short* vec, size_t num)
{
	short* tmp;
	size_t tmp_sz;
	if (vec->capacity < vec->size + num) {
		tmp_sz = vec->capacity + num + CVEC_short_SZ;
		if (!(tmp = (short*)realloc(vec->a, sizeof(short)*tmp_sz))) {
			assert(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}

	vec->size += num;
	return 1;
}

int cvec_insert_short(cvector_short* vec, size_t i, short a)
{
	short* tmp;
	size_t tmp_sz;
	if (vec->capacity > vec->size) {
		memmove(&vec->a[i+1], &vec->a[i], (vec->size-i)*sizeof(short));
		vec->a[i] = a;
	} else {
		tmp_sz = CVEC_short_ALLOCATOR(vec->capacity);
		if (!(tmp = (short*)realloc(vec->a, sizeof(short)*tmp_sz))) {
			assert(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		memmove(&vec->a[i+1], &vec->a[i], (vec->size-i)*sizeof(short));
		vec->a[i] = a;
		vec->capacity = tmp_sz;
	}

	vec->size++;
	return 1;
}

int cvec_insert_array_short(cvector_short* vec, size_t i, short* a, size_t num)
{
	short* tmp;
	size_t tmp_sz;
	if (vec->capacity < vec->size + num) {
		tmp_sz = vec->capacity + num + CVEC_short_SZ;
		if (!(tmp = (short*)realloc(vec->a, sizeof(short)*tmp_sz))) {
			assert(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}

	memmove(&vec->a[i+num], &vec->a[i], (vec->size-i)*sizeof(short));
	memmove(&vec->a[i], a, num*sizeof(short));
	vec->size += num;
	return 1;
}

short cvec_replace_short(cvector_short* vec, size_t i, short a)
{
	short tmp = vec->a[i];
	vec->a[i] = a;
	return tmp;
}

void cvec_erase_short(cvector_short* vec, size_t start, size_t end)
{
	size_t d = end - start + 1;
	memmove(&vec->a[start], &vec->a[end+1], (vec->size-1-end)*sizeof(short));
	vec->size -= d;
}


int cvec_reserve_short(cvector_short* vec, size_t size)
{
	short* tmp;
	if (vec->capacity < size) {
		if (!(tmp = (short*)realloc(vec->a, sizeof(short)*(size+CVEC_short_SZ)))) {
			assert(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = size + CVEC_short_SZ;
	}
	return 1;
}

int cvec_set_cap_short(cvector_short* vec, size_t size)
{
	short* tmp;
	if (size < vec->size) {
		vec->size = size;
	}

	if (!(tmp = (short*)realloc(vec->a, sizeof(short)*size))) {
		assert(tmp != NULL);
		return 0;
	}
	vec->a = tmp;
	vec->capacity = size;
	return 1;
}

void cvec_set_val_sz_short(cvector_short* vec, short val)
{
	size_t i;
	for (i=0; i<vec->size; i++) {
		vec->a[i] = val;
	}
}

void cvec_set_val_cap_short(cvector_short* vec, short val)
{
	size_t i;
	for (i=0; i<vec->capacity; i++) {
		vec->a[i] = val;
	}
}

void cvec_clear_short(cvector_short* vec) { vec->size = 0; }

void cvec_free_short_heap(void* vec)
{
	cvector_short* tmp = (cvector_short*)vec;
	free(tmp->a);
	free(tmp);
}

void cvec_free_short(void* vec)
{
	cvector_short* tmp = (cvector_short*)vec;
	free(tmp->a);
	tmp->size = 0;
	tmp->capacity = 0;
}

#endif
