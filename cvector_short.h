/*

CVector 4.0 MIT Licensed vector (dynamic array) library in strict C89
http://www.robertwinkler.com/projects/cvector.html
http://www.robertwinkler.com/CVector/docs/

Besides the docs and all the Doxygen comments, see cvector_tests.c for
examples of how to use it or look at any of these other projects for
more practical examples:

https://github.com/rswinkle/C_Interpreter
https://github.com/rswinkle/CPIM2
https://github.com/rswinkle/spelling_game
https://github.com/rswinkle/c_bigint
http://portablegl.com/

The MIT License (MIT)

Copyright (c) 2011-2022 Robert Winkler

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
documentation files (the "Software"), to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and
to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.
*/

#ifndef CVECTOR_short_H
#define CVECTOR_short_H

#include <stdlib.h>

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
int cvec_copyc_short(void* dest, void* src);
int cvec_copy_short(cvector_short* dest, cvector_short* src);

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

size_t CVEC_short_SZ = 50;

#define CVEC_short_ALLOCATOR(x) ((x+1) * 2)


#if defined(CVEC_MALLOC) && defined(CVEC_FREE) && defined(CVEC_REALLOC)
/* ok */
#elif !defined(CVEC_MALLOC) && !defined(CVEC_FREE) && !defined(CVEC_REALLOC)
/* ok */
#else
#error "Must define all or none of CVEC_MALLOC, CVEC_FREE, and CVEC_REALLOC."
#endif

#ifndef CVEC_MALLOC
#define CVEC_MALLOC(sz)      malloc(sz)
#define CVEC_REALLOC(p, sz)  realloc(p, sz)
#define CVEC_FREE(p)         free(p)
#endif

#ifndef CVEC_MEMMOVE
#include <string.h>
#define CVEC_MEMMOVE(dst, src, sz)  memmove(dst, src, sz)
#endif

#ifndef CVEC_ASSERT
#include <assert.h>
#define CVEC_ASSERT(x)       assert(x)
#endif

cvector_short* cvec_short_heap(size_t size, size_t capacity)
{
	cvector_short* vec;
	if (!(vec = (cvector_short*)CVEC_MALLOC(sizeof(cvector_short)))) {
		CVEC_ASSERT(vec != NULL);
		return NULL;
	}

	vec->size = size;
	vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + CVEC_short_SZ;

	if (!(vec->a = (short*)CVEC_MALLOC(vec->capacity*sizeof(short)))) {
		CVEC_ASSERT(vec->a != NULL);
		CVEC_FREE(vec);
		return NULL;
	}

	return vec;
}

cvector_short* cvec_init_short_heap(short* vals, size_t num)
{
	cvector_short* vec;
	
	if (!(vec = (cvector_short*)CVEC_MALLOC(sizeof(cvector_short)))) {
		CVEC_ASSERT(vec != NULL);
		return NULL;
	}

	vec->capacity = num + CVEC_short_SZ;
	vec->size = num;
	if (!(vec->a = (short*)CVEC_MALLOC(vec->capacity*sizeof(short)))) {
		CVEC_ASSERT(vec->a != NULL);
		CVEC_FREE(vec);
		return NULL;
	}

	CVEC_MEMMOVE(vec->a, vals, sizeof(short)*num);

	return vec;
}

int cvec_short(cvector_short* vec, size_t size, size_t capacity)
{
	vec->size = size;
	vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + CVEC_short_SZ;

	if (!(vec->a = (short*)CVEC_MALLOC(vec->capacity*sizeof(short)))) {
		CVEC_ASSERT(vec->a != NULL);
		vec->size = vec->capacity = 0;
		return 0;
	}

	return 1;
}

int cvec_init_short(cvector_short* vec, short* vals, size_t num)
{
	vec->capacity = num + CVEC_short_SZ;
	vec->size = num;
	if (!(vec->a = (short*)CVEC_MALLOC(vec->capacity*sizeof(short)))) {
		CVEC_ASSERT(vec->a != NULL);
		vec->size = vec->capacity = 0;
		return 0;
	}

	CVEC_MEMMOVE(vec->a, vals, sizeof(short)*num);

	return 1;
}

int cvec_copyc_short(void* dest, void* src)
{
	cvector_short* vec1 = (cvector_short*)dest;
	cvector_short* vec2 = (cvector_short*)src;

	vec1->a = NULL;
	vec1->size = 0;
	vec1->capacity = 0;

	return cvec_copy_short(vec1, vec2);
}

int cvec_copy_short(cvector_short* dest, cvector_short* src)
{
	short* tmp = NULL;
	if (!(tmp = (short*)CVEC_REALLOC(dest->a, src->capacity*sizeof(short)))) {
		CVEC_ASSERT(tmp != NULL);
		return 0;
	}
	dest->a = tmp;

	CVEC_MEMMOVE(dest->a, src->a, src->size*sizeof(short));
	dest->size = src->size;
	dest->capacity = src->capacity;
	return 1;
}


int cvec_push_short(cvector_short* vec, short a)
{
	short* tmp;
	size_t tmp_sz;
	if (vec->capacity > vec->size) {
		vec->a[vec->size++] = a;
	} else {
		tmp_sz = CVEC_short_ALLOCATOR(vec->capacity);
		if (!(tmp = (short*)CVEC_REALLOC(vec->a, sizeof(short)*tmp_sz))) {
			CVEC_ASSERT(tmp != NULL);
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
		if (!(tmp = (short*)CVEC_REALLOC(vec->a, sizeof(short)*tmp_sz))) {
			CVEC_ASSERT(tmp != NULL);
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
		CVEC_MEMMOVE(&vec->a[i+1], &vec->a[i], (vec->size-i)*sizeof(short));
		vec->a[i] = a;
	} else {
		tmp_sz = CVEC_short_ALLOCATOR(vec->capacity);
		if (!(tmp = (short*)CVEC_REALLOC(vec->a, sizeof(short)*tmp_sz))) {
			CVEC_ASSERT(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		CVEC_MEMMOVE(&vec->a[i+1], &vec->a[i], (vec->size-i)*sizeof(short));
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
		if (!(tmp = (short*)CVEC_REALLOC(vec->a, sizeof(short)*tmp_sz))) {
			CVEC_ASSERT(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}

	CVEC_MEMMOVE(&vec->a[i+num], &vec->a[i], (vec->size-i)*sizeof(short));
	CVEC_MEMMOVE(&vec->a[i], a, num*sizeof(short));
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
	CVEC_MEMMOVE(&vec->a[start], &vec->a[end+1], (vec->size-1-end)*sizeof(short));
	vec->size -= d;
}


int cvec_reserve_short(cvector_short* vec, size_t size)
{
	short* tmp;
	if (vec->capacity < size) {
		if (!(tmp = (short*)CVEC_REALLOC(vec->a, sizeof(short)*(size+CVEC_short_SZ)))) {
			CVEC_ASSERT(tmp != NULL);
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

	if (!(tmp = (short*)CVEC_REALLOC(vec->a, sizeof(short)*size))) {
		CVEC_ASSERT(tmp != NULL);
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
	if (!tmp) return;
	CVEC_FREE(tmp->a);
	CVEC_FREE(tmp);
}

void cvec_free_short(void* vec)
{
	cvector_short* tmp = (cvector_short*)vec;
	CVEC_FREE(tmp->a);
	tmp->size = 0;
	tmp->capacity = 0;
}

#endif
