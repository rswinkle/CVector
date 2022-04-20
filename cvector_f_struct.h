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

#ifndef CVECTOR_f_struct_H
#define CVECTOR_f_struct_H

#include <stdlib.h>


#ifdef __cplusplus
extern "C" {
#endif


typedef struct cvector_f_struct
{
	f_struct* a;
	size_t size;
	size_t capacity;
	void (*elem_free)(void*);
	int (*elem_init)(void*, void*);
} cvector_f_struct;

extern size_t CVEC_f_struct_SZ;

int cvec_f_struct(cvector_f_struct* vec, size_t size, size_t capacity, void(*elem_free)(void*), int(*elem_init)(void*, void*));
int cvec_init_f_struct(cvector_f_struct* vec, f_struct* vals, size_t num, void(*elem_free)(void*), int(*elem_init)(void*, void*));

cvector_f_struct* cvec_f_struct_heap(size_t size, size_t capacity, void (*elem_free)(void*), int(*elem_init)(void*, void*));
cvector_f_struct* cvec_init_f_struct_heap(f_struct* vals, size_t num, void (*elem_free)(void*), int(*elem_init)(void*, void*));

int cvec_copyc_f_struct(void* dest, void* src);
int cvec_copy_f_struct(cvector_f_struct* dest, cvector_f_struct* src);

int cvec_push_f_struct(cvector_f_struct* vec, f_struct* val);
void cvec_pop_f_struct(cvector_f_struct* vec, f_struct* ret);

int cvec_pushm_f_struct(cvector_f_struct* vec, f_struct* a);
void cvec_popm_f_struct(cvector_f_struct* vec, f_struct* ret);
int cvec_insertm_f_struct(cvector_f_struct* vec, size_t i, f_struct* a);
int cvec_insert_arraym_f_struct(cvector_f_struct* vec, size_t i, f_struct* a, size_t num);
void cvec_replacem_f_struct(cvector_f_struct* vec, size_t i, f_struct* a, f_struct* ret);

int cvec_extend_f_struct(cvector_f_struct* vec, size_t num);
int cvec_insert_f_struct(cvector_f_struct* vec, size_t i, f_struct* a);
int cvec_insert_array_f_struct(cvector_f_struct* vec, size_t i, f_struct* a, size_t num);
int cvec_replace_f_struct(cvector_f_struct* vec, size_t i, f_struct* a, f_struct* ret);
void cvec_erase_f_struct(cvector_f_struct* vec, size_t start, size_t end);
void cvec_remove_f_struct(cvector_f_struct* vec, size_t start, size_t end);
int cvec_reserve_f_struct(cvector_f_struct* vec, size_t size);
int cvec_set_cap_f_struct(cvector_f_struct* vec, size_t size);
int cvec_set_val_sz_f_struct(cvector_f_struct* vec, f_struct* val);
int cvec_set_val_cap_f_struct(cvector_f_struct* vec, f_struct* val);

f_struct* cvec_back_f_struct(cvector_f_struct* vec);

void cvec_clear_f_struct(cvector_f_struct* vec);
void cvec_free_f_struct_heap(void* vec);
void cvec_free_f_struct(void* vec);

#ifdef __cplusplus
}
#endif


/* CVECTOR_f_struct_H */
#endif

#ifdef CVECTOR_f_struct_IMPLEMENTATION



size_t CVEC_f_struct_SZ = 20;

#define CVEC_f_struct_ALLOCATOR(x) ((x+1) * 2)

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


/*  general vector */

cvector_f_struct* cvec_f_struct_heap(size_t size, size_t capacity, void(*elem_free)(void*), int(*elem_init)(void*, void*))
{
	cvector_f_struct* vec;
	if (!(vec = (cvector_f_struct*)CVEC_MALLOC(sizeof(cvector_f_struct)))) {
		CVEC_ASSERT(vec != NULL);
		return NULL;
	}

	vec->size = size;
	vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + CVEC_f_struct_SZ;

	if (!(vec->a = (f_struct*)CVEC_MALLOC(vec->capacity * sizeof(f_struct)))) {
		CVEC_ASSERT(vec->a != NULL);
		CVEC_FREE(vec);
		return NULL;
	}

	vec->elem_free = elem_free;
	vec->elem_init = elem_init;

	return vec;
}

cvector_f_struct* cvec_init_f_struct_heap(f_struct* vals, size_t num, void(*elem_free)(void*), int(*elem_init)(void*, void*))
{
	cvector_f_struct* vec;
	size_t i;
	
	if (!(vec = (cvector_f_struct*)CVEC_MALLOC(sizeof(cvector_f_struct)))) {
		CVEC_ASSERT(vec != NULL);
		return NULL;
	}

	vec->capacity = num + CVEC_f_struct_SZ;
	vec->size = num;
	if (!(vec->a = (f_struct*)CVEC_MALLOC(vec->capacity * sizeof(f_struct)))) {
		CVEC_ASSERT(vec->a != NULL);
		CVEC_FREE(vec);
		return NULL;
	}

	if (elem_init) {
		for (i=0; i<num; ++i) {
			if (!elem_init(&vec->a[i], &vals[i])) {
				CVEC_ASSERT(0);
				CVEC_FREE(vec->a);
				CVEC_FREE(vec);
				return NULL;
			}
		}
	} else {
		CVEC_MEMMOVE(vec->a, vals, sizeof(f_struct)*num);
	}
	
	vec->elem_free = elem_free;
	vec->elem_init = elem_init;

	return vec;
}

int cvec_f_struct(cvector_f_struct* vec, size_t size, size_t capacity, void(*elem_free)(void*), int(*elem_init)(void*, void*))
{
	vec->size = size;
	vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + CVEC_f_struct_SZ;

	if (!(vec->a = (f_struct*)CVEC_MALLOC(vec->capacity * sizeof(f_struct)))) {
		CVEC_ASSERT(vec->a != NULL);
		vec->size = vec->capacity = 0;
		return 0;
	}

	vec->elem_free = elem_free;
	vec->elem_init = elem_init;

	return 1;
}

int cvec_init_f_struct(cvector_f_struct* vec, f_struct* vals, size_t num, void(*elem_free)(void*), int(*elem_init)(void*, void*))
{
	size_t i;
	
	vec->capacity = num + CVEC_f_struct_SZ;
	vec->size = num;
	if (!(vec->a = (f_struct*)CVEC_MALLOC(vec->capacity * sizeof(f_struct)))) {
		CVEC_ASSERT(vec->a != NULL);
		vec->size = vec->capacity = 0;
		return 0;
	}

	if (elem_init) {
		for (i=0; i<num; ++i) {
			if (!elem_init(&vec->a[i], &vals[i])) {
				CVEC_ASSERT(0);
				return 0;
			}
		}
	} else {
		CVEC_MEMMOVE(vec->a, vals, sizeof(f_struct)*num);
	}

	vec->elem_free = elem_free;
	vec->elem_init = elem_init;

	return 1;
}

int cvec_copyc_f_struct(void* dest, void* src)
{
	cvector_f_struct* vec1 = (cvector_f_struct*)dest;
	cvector_f_struct* vec2 = (cvector_f_struct*)src;

	vec1->a = NULL;
	vec1->size = 0;
	vec1->capacity = 0;

	return cvec_copy_f_struct(vec1, vec2);
}

int cvec_copy_f_struct(cvector_f_struct* dest, cvector_f_struct* src)
{
	int i;
	f_struct* tmp = NULL;
	if (!(tmp = (f_struct*)CVEC_REALLOC(dest->a, src->capacity*sizeof(f_struct)))) {
		CVEC_ASSERT(tmp != NULL);
		return 0;
	}
	dest->a = tmp;

	if (src->elem_init) {
		for (i=0; i<src->size; ++i) {
			if (!src->elem_init(&dest->a[i], &src->a[i])) {
				CVEC_ASSERT(0);
				return 0;
			}
		}
	} else {
		/* could use memcpy here since we know we just allocated dest->a */
		CVEC_MEMMOVE(dest->a, src->a, src->size*sizeof(f_struct));
	}

	dest->size = src->size;
	dest->capacity = src->capacity;
	dest->elem_free = src->elem_free;
	dest->elem_init = src->elem_init;
	return 1;
}

int cvec_push_f_struct(cvector_f_struct* vec, f_struct* a)
{
	f_struct* tmp;
	size_t tmp_sz;
	if (vec->capacity == vec->size) {
		tmp_sz = CVEC_f_struct_ALLOCATOR(vec->capacity);
		if (!(tmp = (f_struct*)CVEC_REALLOC(vec->a, sizeof(f_struct)*tmp_sz))) {
			CVEC_ASSERT(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}
	if (vec->elem_init) {
		if (!vec->elem_init(&vec->a[vec->size], a)) {
			CVEC_ASSERT(0);
			return 0;
		}
	} else {
		CVEC_MEMMOVE(&vec->a[vec->size], a, sizeof(f_struct));
	}
	
	vec->size++;
	return 1;
}

int cvec_pushm_f_struct(cvector_f_struct* vec, f_struct* a)
{
	f_struct* tmp;
	size_t tmp_sz;
	if (vec->capacity == vec->size) {
		tmp_sz = CVEC_f_struct_ALLOCATOR(vec->capacity);
		if (!(tmp = (f_struct*)CVEC_REALLOC(vec->a, sizeof(f_struct)*tmp_sz))) {
			CVEC_ASSERT(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}
	CVEC_MEMMOVE(&vec->a[vec->size], a, sizeof(f_struct));
	
	vec->size++;
	return 1;
}

void cvec_pop_f_struct(cvector_f_struct* vec, f_struct* ret)
{
	vec->size--;
	if (ret) {
		CVEC_MEMMOVE(ret, &vec->a[vec->size], sizeof(f_struct));
	}

	if (vec->elem_free) {
		vec->elem_free(&vec->a[vec->size]);
	}
}

void cvec_popm_f_struct(cvector_f_struct* vec, f_struct* ret)
{
	vec->size--;
	if (ret) {
		CVEC_MEMMOVE(ret, &vec->a[vec->size], sizeof(f_struct));
	}
}

/** Return pointer to last element */
f_struct* cvec_back_f_struct(cvector_f_struct* vec)
{
	return &vec->a[vec->size-1];
}

int cvec_extend_f_struct(cvector_f_struct* vec, size_t num)
{
	f_struct* tmp;
	size_t tmp_sz;
	if (vec->capacity < vec->size + num) {
		tmp_sz = vec->capacity + num + CVEC_f_struct_SZ;
		if (!(tmp = (f_struct*)CVEC_REALLOC(vec->a, sizeof(f_struct)*tmp_sz))) {
			CVEC_ASSERT(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}

	vec->size += num;
	return 1;
}

int cvec_insert_f_struct(cvector_f_struct* vec, size_t i, f_struct* a)
{
	f_struct* tmp;
	size_t tmp_sz;
	if (vec->capacity == vec->size) {
		tmp_sz = CVEC_f_struct_ALLOCATOR(vec->capacity);
		if (!(tmp = (f_struct*)CVEC_REALLOC(vec->a, sizeof(f_struct)*tmp_sz))) {
			CVEC_ASSERT(tmp != NULL);
			return 0;
		}
		
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}
	CVEC_MEMMOVE(&vec->a[i+1], &vec->a[i], (vec->size-i)*sizeof(f_struct));

	if (vec->elem_init) {
		if (!vec->elem_init(&vec->a[i], a)) {
			CVEC_ASSERT(0);
			return 0;
		}
	} else {
		CVEC_MEMMOVE(&vec->a[i], a, sizeof(f_struct));
	}

	vec->size++;
	return 1;
}

int cvec_insertm_f_struct(cvector_f_struct* vec, size_t i, f_struct* a)
{
	f_struct* tmp;
	size_t tmp_sz;
	if (vec->capacity == vec->size) {
		tmp_sz = CVEC_f_struct_ALLOCATOR(vec->capacity);
		if (!(tmp = (f_struct*)CVEC_REALLOC(vec->a, sizeof(f_struct)*tmp_sz))) {
			CVEC_ASSERT(tmp != NULL);
			return 0;
		}
		
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}
	CVEC_MEMMOVE(&vec->a[i+1], &vec->a[i], (vec->size-i)*sizeof(f_struct));

	CVEC_MEMMOVE(&vec->a[i], a, sizeof(f_struct));

	vec->size++;
	return 1;
}

int cvec_insert_array_f_struct(cvector_f_struct* vec, size_t i, f_struct* a, size_t num)
{
	f_struct* tmp;
	size_t tmp_sz, j;
	if (vec->capacity < vec->size + num) {
		tmp_sz = vec->capacity + num + CVEC_f_struct_SZ;
		if (!(tmp = (f_struct*)CVEC_REALLOC(vec->a, sizeof(f_struct)*tmp_sz))) {
			CVEC_ASSERT(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}

	CVEC_MEMMOVE(&vec->a[i+num], &vec->a[i], (vec->size-i)*sizeof(f_struct));
	if (vec->elem_init) {
		for (j=0; j<num; ++j) {
			if (!vec->elem_init(&vec->a[j+i], &a[j])) {
				CVEC_ASSERT(0);
				return 0;
			}
		}
	} else {
		CVEC_MEMMOVE(&vec->a[i], a, num*sizeof(f_struct));
	}
	vec->size += num;
	return 1;
}

int cvec_insert_arraym_f_struct(cvector_f_struct* vec, size_t i, f_struct* a, size_t num)
{
	f_struct* tmp;
	size_t tmp_sz;
	if (vec->capacity < vec->size + num) {
		tmp_sz = vec->capacity + num + CVEC_VOID_START_SZ;
		if (!(tmp = (f_struct*)CVEC_REALLOC(vec->a, sizeof(f_struct)*tmp_sz))) {
			CVEC_ASSERT(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}

	CVEC_MEMMOVE(&vec->a[i+num], &vec->a[i], (vec->size-i)*sizeof(f_struct));

	CVEC_MEMMOVE(&vec->a[i], a, num*sizeof(f_struct));
	vec->size += num;
	return 1;
}

int cvec_replace_f_struct(cvector_f_struct* vec, size_t i, f_struct* a, f_struct* ret)
{
	if (ret) {
		CVEC_MEMMOVE(ret, &vec->a[i], sizeof(f_struct));
	} else if (vec->elem_free) {
		vec->elem_free(&vec->a[i]);
	}

	if (vec->elem_init) {
		if (!vec->elem_init(&vec->a[i], a)) {
			CVEC_ASSERT(0);
			return 0;
		}
	} else {
		CVEC_MEMMOVE(&vec->a[i], a, sizeof(f_struct));
	}
	return 1;
}

void cvec_replacem_f_struct(cvector_f_struct* vec, size_t i, f_struct* a, f_struct* ret)
{
	if (ret) {
		CVEC_MEMMOVE(ret, &vec->a[i], sizeof(f_struct));
	}

	CVEC_MEMMOVE(&vec->a[i], a, sizeof(f_struct));
}

void cvec_erase_f_struct(cvector_f_struct* vec, size_t start, size_t end)
{
	size_t i;
	size_t d = end - start + 1;
	if (vec->elem_free) {
		for (i=start; i<=end; i++) {
			vec->elem_free(&vec->a[i]);
		}
	}
	CVEC_MEMMOVE(&vec->a[start], &vec->a[end+1], (vec->size-1-end)*sizeof(f_struct));
	vec->size -= d;
}

void cvec_remove_f_struct(cvector_f_struct* vec, size_t start, size_t end)
{
	size_t d = end - start + 1;
	CVEC_MEMMOVE(&vec->a[start], &vec->a[end+1], (vec->size-1-end)*sizeof(f_struct));
	vec->size -= d;
}

int cvec_reserve_f_struct(cvector_f_struct* vec, size_t size)
{
	f_struct* tmp;
	if (vec->capacity < size) {
		if (!(tmp = (f_struct*)CVEC_REALLOC(vec->a, sizeof(f_struct)*(size+CVEC_f_struct_SZ)))) {
			CVEC_ASSERT(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = size + CVEC_f_struct_SZ;
	}
	return 1;
}

int cvec_set_cap_f_struct(cvector_f_struct* vec, size_t size)
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

	if (!(tmp = (f_struct*)CVEC_REALLOC(vec->a, sizeof(f_struct)*size))) {
		CVEC_ASSERT(tmp != NULL);
		return 0;
	}
	vec-> a = tmp;
	return 1;
}

int cvec_set_val_sz_f_struct(cvector_f_struct* vec, f_struct* val)
{
	size_t i;

	if (vec->elem_free) {
		for(i=0; i<vec->size; i++) {
			vec->elem_free(&vec->a[i]);
		}
	}
	
	if (vec->elem_init) {
		for (i=0; i<vec->size; i++) {
			if (!vec->elem_init(&vec->a[i], val)) {
				CVEC_ASSERT(0);
				return 0;
			}
		}
	} else {
		for (i=0; i<vec->size; i++) {
			CVEC_MEMMOVE(&vec->a[i], val, sizeof(f_struct));
		}
	}
	return 1;
}

int cvec_set_val_cap_f_struct(cvector_f_struct* vec, f_struct* val)
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
			if (!vec->elem_init(&vec->a[i], val)) {
				CVEC_ASSERT(0);
				return 0;
			}
		}
	} else {
		for (i=0; i<vec->capacity; i++) {
			CVEC_MEMMOVE(&vec->a[i], val, sizeof(f_struct));
		}
	}
	return 1;
}

void cvec_clear_f_struct(cvector_f_struct* vec)
{
	size_t i;
	if (vec->elem_free) {
		for (i=0; i<vec->size; ++i) {
			vec->elem_free(&vec->a[i]);
		}
	}
	vec->size = 0;
}

void cvec_free_f_struct_heap(void* vec)
{
	size_t i;
	cvector_f_struct* tmp = (cvector_f_struct*)vec;
	if (!tmp) return;
	if (tmp->elem_free) {
		for (i=0; i<tmp->size; i++) {
			tmp->elem_free(&tmp->a[i]);
		}
	}
	CVEC_FREE(tmp->a);
	CVEC_FREE(tmp);
}

void cvec_free_f_struct(void* vec)
{
	size_t i;
	cvector_f_struct* tmp = (cvector_f_struct*)vec;
	if (tmp->elem_free) {
		for (i=0; i<tmp->size; i++) {
			tmp->elem_free(&tmp->a[i]);
		}
	}

	CVEC_FREE(tmp->a);

	tmp->size = 0;
	tmp->capacity = 0;
}


#endif
