#ifndef CVECTOR_TYPE_H
#define CVECTOR_TYPE_H

#include <stdlib.h>


#ifdef __cplusplus
extern "C" {
#endif


typedef struct cvector_TYPE
{
	TYPE* a;
	size_t size;
	size_t capacity;
	void (*elem_free)(void*);
	void (*elem_init)(void*, void*);
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



size_t CVEC_TYPE_SZ = 20;

#define CVEC_TYPE_ALLOCATOR(x) ((x+1) * 2)

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

cvector_TYPE* cvec_TYPE_heap(size_t size, size_t capacity, void(*elem_free)(void*), void(*elem_init)(void*, void*))
{
	cvector_TYPE* vec;
	if (!(vec = (cvector_TYPE*)CVEC_MALLOC(sizeof(cvector_TYPE)))) {
		CVEC_ASSERT(vec != NULL);
		return NULL;
	}

	vec->size = size;
	vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + CVEC_TYPE_SZ;

	if (!(vec->a = (TYPE*)CVEC_MALLOC(vec->capacity * sizeof(TYPE)))) {
		CVEC_ASSERT(vec->a != NULL);
		CVEC_FREE(vec);
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
	
	if (!(vec = (cvector_TYPE*)CVEC_MALLOC(sizeof(cvector_TYPE)))) {
		CVEC_ASSERT(vec != NULL);
		return NULL;
	}

	vec->capacity = num + CVEC_TYPE_SZ;
	vec->size = num;
	if (!(vec->a = (TYPE*)CVEC_MALLOC(vec->capacity * sizeof(TYPE)))) {
		CVEC_ASSERT(vec->a != NULL);
		CVEC_FREE(vec);
		return NULL;
	}

	if (elem_init) {
		for (i=0; i<num; ++i) {
			elem_init(&vec->a[i], &vals[i]);
		}
	} else {
		CVEC_MEMMOVE(vec->a, vals, sizeof(TYPE)*num);
	}
	
	vec->elem_free = elem_free;
	vec->elem_init = elem_init;

	return vec;
}

int cvec_TYPE(cvector_TYPE* vec, size_t size, size_t capacity, void(*elem_free)(void*), void(*elem_init)(void*, void*))
{
	vec->size = size;
	vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + CVEC_TYPE_SZ;

	if (!(vec->a = (TYPE*)CVEC_MALLOC(vec->capacity * sizeof(TYPE)))) {
		CVEC_ASSERT(vec->a != NULL);
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
	if (!(vec->a = (TYPE*)CVEC_MALLOC(vec->capacity * sizeof(TYPE)))) {
		CVEC_ASSERT(vec->a != NULL);
		vec->size = vec->capacity = 0;
		return 0;
	}

	if (elem_init) {
		for (i=0; i<num; ++i) {
			elem_init(&vec->a[i], &vals[i]);
		}
	} else {
		CVEC_MEMMOVE(vec->a, vals, sizeof(TYPE)*num);
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
	if (!(vec1->a = (TYPE*)CVEC_MALLOC(vec2->capacity*sizeof(TYPE)))) {
		CVEC_ASSERT(vec1->a != NULL);
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
		CVEC_MEMMOVE(vec1->a, vec2->a, vec1->size*sizeof(TYPE));
	}
}

int cvec_push_TYPE(cvector_TYPE* vec, TYPE* a)
{
	TYPE* tmp;
	size_t tmp_sz;
	if (vec->capacity == vec->size) {
		tmp_sz = CVEC_TYPE_ALLOCATOR(vec->capacity);
		if (!(tmp = (TYPE*)CVEC_REALLOC(vec->a, sizeof(TYPE)*tmp_sz))) {
			CVEC_ASSERT(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}
	if (vec->elem_init) {
		vec->elem_init(&vec->a[vec->size], a);
	} else {
		CVEC_MEMMOVE(&vec->a[vec->size], a, sizeof(TYPE));
	}
	
	vec->size++;
	return 1;
}

void cvec_pop_TYPE(cvector_TYPE* vec, TYPE* ret)
{
	vec->size--;
	if (ret) {
		CVEC_MEMMOVE(ret, &vec->a[vec->size], sizeof(TYPE));
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
		if (!(tmp = (TYPE*)CVEC_REALLOC(vec->a, sizeof(TYPE)*tmp_sz))) {
			CVEC_ASSERT(tmp != NULL);
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
	if (vec->capacity == vec->size) {
		tmp_sz = CVEC_TYPE_ALLOCATOR(vec->capacity);
		if (!(tmp = (TYPE*)CVEC_REALLOC(vec->a, sizeof(TYPE)*tmp_sz))) {
			CVEC_ASSERT(tmp != NULL);
			return 0;
		}
		
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}
	CVEC_MEMMOVE(&vec->a[i+1], &vec->a[i], (vec->size-i)*sizeof(TYPE));

	if (vec->elem_init) {
		vec->elem_init(&vec->a[i], a);
	} else {
		CVEC_MEMMOVE(&vec->a[i], a, sizeof(TYPE));
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
		if (!(tmp = (TYPE*)CVEC_REALLOC(vec->a, sizeof(TYPE)*tmp_sz))) {
			CVEC_ASSERT(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}

	CVEC_MEMMOVE(&vec->a[i+num], &vec->a[i], (vec->size-i)*sizeof(TYPE));
	if (vec->elem_init) {
		for (j=0; j<num; ++j) {
			vec->elem_init(&vec->a[j+i], &a[j]);
		}
	} else {
		CVEC_MEMMOVE(&vec->a[i], a, num*sizeof(TYPE));
	}
	vec->size += num;
	return 1;
}

void cvec_replace_TYPE(cvector_TYPE* vec, size_t i, TYPE* a, TYPE* ret)
{
	if (ret)
		CVEC_MEMMOVE(ret, &vec->a[i], sizeof(TYPE));
	CVEC_MEMMOVE(&vec->a[i], a, sizeof(TYPE));
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
	CVEC_MEMMOVE(&vec->a[start], &vec->a[end+1], (vec->size-1-end)*sizeof(TYPE));
	vec->size -= d;
}

int cvec_reserve_TYPE(cvector_TYPE* vec, size_t size)
{
	TYPE* tmp;
	if (vec->capacity < size) {
		if (!(tmp = (TYPE*)CVEC_REALLOC(vec->a, sizeof(TYPE)*(size+CVEC_TYPE_SZ)))) {
			CVEC_ASSERT(tmp != NULL);
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

	if (!(tmp = (TYPE*)CVEC_REALLOC(vec->a, sizeof(TYPE)*size))) {
		CVEC_ASSERT(tmp != NULL);
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
			CVEC_MEMMOVE(&vec->a[i], val, sizeof(TYPE));
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
			CVEC_MEMMOVE(&vec->a[i], val, sizeof(TYPE));
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
	if (!tmp) return;
	if (tmp->elem_free) {
		for (i=0; i<tmp->size; i++) {
			tmp->elem_free(&tmp->a[i]);
		}
	}
	CVEC_FREE(tmp->a);
	CVEC_FREE(tmp);
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

	CVEC_FREE(tmp->a);

	tmp->size = 0;
	tmp->capacity = 0;
}


#endif
