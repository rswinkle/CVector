#ifndef VECTOR_short_H
#define VECTOR_short_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Data structure for short vector. */
typedef struct vector_short
{
	short* a;           /**< Array. */
	size_t size;       /**< Current size (amount you use when manipulating array directly). */
	size_t capacity;   /**< Allocated size of array; always >= size. */
} vector_short;



extern size_t VEC_short_SZ;

int vec_short(vector_short* vec, size_t size, size_t capacity);
int init_vec_short(vector_short* vec, short* vals, size_t num);

vector_short* vec_short_heap(size_t size, size_t capacity);
vector_short* init_vec_short_heap(short* vals, size_t num);

void vec_short_copy(void* dest, void* src);

int push_short(vector_short* vec, short a);
short pop_short(vector_short* vec);

int extend_short(vector_short* vec, size_t num);
int insert_short(vector_short* vec, size_t i, short a);
int insert_array_short(vector_short* vec, size_t i, short* a, size_t num);
void erase_short(vector_short* vec, size_t start, size_t end);
int reserve_short(vector_short* vec, size_t size);
int set_capacity_short(vector_short* vec, size_t size);
void set_val_sz_short(vector_short* vec, short val);
void set_val_cap_short(vector_short* vec, short val);

short* back_short(vector_short* vec);

void clear_short(vector_short* vec);
void free_vec_short_heap(void* vec);
void free_vec_short(void* vec);

#ifdef __cplusplus
}
#endif

/* VECTOR_short_H */
#endif


#ifdef VECTOR_short_IMPLEMENTATION

#include <assert.h>

#define VEC_short_ALLOCATOR(x) ((x) * 2)


size_t VEC_short_SZ = 50;



vector_short* vec_short_heap(size_t size, size_t capacity)
{
	vector_short* vec;
	if (!(vec = (vector_short*)malloc(sizeof(vector_short)))) {
		assert(vec != NULL);
		return NULL;
	}

	vec->size = size;
	vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + VEC_short_SZ;

	if (!(vec->a = (short*)malloc(vec->capacity*sizeof(short)))) {
		assert(vec->a != NULL);
		free(vec);
		return NULL;
	}

	return vec;
}


vector_short* init_vec_short_heap(short* vals, size_t num)
{
	vector_short* vec;
	
	if (!(vec = (vector_short*)malloc(sizeof(vector_short)))) {
		assert(vec != NULL);
		return NULL;
	}

	vec->capacity = num + VEC_short_SZ;
	vec->size = num;
	if (!(vec->a = (short*)malloc(vec->capacity*sizeof(short)))) {
		assert(vec->a != NULL);
		free(vec);
		return NULL;
	}

	memcpy(vec->a, vals, sizeof(short)*num);

	return vec;
}


int vec_short(vector_short* vec, size_t size, size_t capacity)
{
	vec->size = size;
	vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + VEC_short_SZ;

	if (!(vec->a = (short*)malloc(vec->capacity*sizeof(short)))) {
		assert(vec->a != NULL);
		vec->size = vec->capacity = 0;
		return 0;
	}

	return 1;
}


int init_vec_short(vector_short* vec, short* vals, size_t num)
{
	vec->capacity = num + VEC_short_SZ;
	vec->size = num;
	if (!(vec->a = (short*)malloc(vec->capacity*sizeof(short)))) {
		assert(vec->a != NULL);
		vec->size = vec->capacity = 0;
		return 0;
	}

	memcpy(vec->a, vals, sizeof(short)*num);

	return 1;
}

void vec_short_copy(void* dest, void* src)
{
	vector_short* vec1 = (vector_short*)dest;
	vector_short* vec2 = (vector_short*)src;
	
	vec1->size = 0;
	vec1->capacity = 0;
	
	/*not much else we can do here*/
	if (!(vec1->a = (short*)malloc(vec2->capacity*sizeof(short)))) {
		assert(vec1->a != NULL);
		return;
	}
	
	memcpy(vec1->a, vec2->a, vec2->size*sizeof(short));
	vec1->size = vec2->size;
	vec1->capacity = vec2->capacity;
}





int push_short(vector_short* vec, short a)
{
	short* tmp;
	size_t tmp_sz;
	if (vec->capacity > vec->size) {
		vec->a[vec->size++] = a;
	} else {
		tmp_sz = VEC_short_ALLOCATOR(vec->capacity);
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



short pop_short(vector_short* vec)
{
	return vec->a[--vec->size];
}

short* back_short(vector_short* vec)
{
	return &vec->a[vec->size-1];
}


int extend_short(vector_short* vec, size_t num)
{
	short* tmp;
	size_t tmp_sz;
	if (vec->capacity < vec->size + num) {
		tmp_sz = vec->capacity + num + VEC_short_SZ;
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


int insert_short(vector_short* vec, size_t i, short a)
{
	short* tmp;
	size_t tmp_sz;
	if (vec->capacity > vec->size) {
		memmove(&vec->a[i+1], &vec->a[i], (vec->size-i)*sizeof(short));
		vec->a[i] = a;
	} else {
		tmp_sz = VEC_short_ALLOCATOR(vec->capacity);
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


int insert_array_short(vector_short* vec, size_t i, short* a, size_t num)
{
	short* tmp;
	size_t tmp_sz;
	if (vec->capacity < vec->size + num) {
		tmp_sz = vec->capacity + num + VEC_short_SZ;
		if (!(tmp = (short*)realloc(vec->a, sizeof(short)*tmp_sz))) {
			assert(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}

	memmove(&vec->a[i+num], &vec->a[i], (vec->size-i)*sizeof(short));
	memcpy(&vec->a[i], a, num*sizeof(short));
	vec->size += num;
	return 1;
}


void erase_short(vector_short* vec, size_t start, size_t end)
{
	size_t d = end - start + 1;
	memmove(&vec->a[start], &vec->a[end+1], (vec->size-1-end)*sizeof(short));
	vec->size -= d;
}


int reserve_short(vector_short* vec, size_t size)
{
	short* tmp;
	if (vec->capacity < size) {
		if (!(tmp = (short*)realloc(vec->a, sizeof(short)*(size+VEC_short_SZ)))) {
			assert(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = size + VEC_short_SZ;
	}
	return 1;
}




int set_capacity_short(vector_short* vec, size_t size)
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



void set_val_sz_short(vector_short* vec, short val)
{
	size_t i;
	for (i=0; i<vec->size; i++) {
		vec->a[i] = val;
	}
}


void set_val_cap_short(vector_short* vec, short val)
{
	size_t i;
	for (i=0; i<vec->capacity; i++) {
		vec->a[i] = val;
	}
}


void clear_short(vector_short* vec) { vec->size = 0; }

void free_vec_short_heap(void* vec)
{
	vector_short* tmp = (vector_short*)vec;
	free(tmp->a);
	free(tmp);
}

void free_vec_short(void* vec)
{
	vector_short* tmp = (vector_short*)vec;
	free(tmp->a);
	tmp->size = 0;
	tmp->capacity = 0;
}

#endif
