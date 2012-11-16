#include "vector_TYPE.h"

#include <assert.h>

#define STDERR(X) fprintf(stderr, X)

#define VEC_TYPE_ALLOCATOR(x) ((x) * 2)


size_t VECTOR_TYPE_SZ = 50;



vector_TYPE* vec_TYPE_heap(size_t size, size_t capacity)
{
	vector_TYPE* vec;
	if (!(vec = malloc(sizeof(vector_TYPE)))) {
		assert(vec != NULL);
		return NULL;
	}

	vec->size = (size > 0) ? size : 0;
	vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + VECTOR_TYPE_SZ;

	if (!(vec->a = malloc(vec->capacity*sizeof(TYPE)))) {
		assert(vec->a != NULL);
		free(vec);
		return NULL;
	}

	return vec;
}


vector_TYPE* init_vec_TYPE_heap(TYPE* vals, size_t num)
{
	vector_TYPE* vec;
	
	if (!vals || num < 1) {
		return NULL;
	}
	
	if (!(vec = malloc(sizeof(vector_TYPE)))) {
		assert(vec != NULL);
		return NULL;
	}

	vec->capacity = num + VECTOR_TYPE_SZ;
	vec->size = num;
	if (!(vec->a = malloc(vec->capacity*sizeof(TYPE)))) {
		assert(vec->a != NULL);
		free(vec);
		return NULL;
	}

	memcpy(vec->a, vals, sizeof(TYPE)*num);

	return vec;
}


int vec_TYPE(vector_TYPE* vec, size_t size, size_t capacity)
{
	vec->size = (size > 0) ? size : 0;
	vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + VECTOR_TYPE_SZ;

	if (!(vec->a = malloc(vec->capacity*sizeof(TYPE)))) {
		assert(vec->a != NULL);
		vec->size = vec->capacity = 0;
		return 0;
	}

	return 1;
}


int init_vec_TYPE(vector_TYPE* vec, TYPE* vals, size_t num)
{
	if (!vals || num < 1) {
		return 0;
	}

	vec->capacity = num + VECTOR_TYPE_SZ;
	vec->size = num;
	if (!(vec->a = malloc(vec->capacity*sizeof(TYPE)))) {
		assert(vec->a != NULL);
		vec->size = vec->capacity = 0;
		return 0;
	}

	memcpy(vec->a, vals, sizeof(TYPE)*num);

	return 1;
}

void vec_TYPE_copy(void* dest, void* src)
{
	size_t i;
	vector_TYPE* vec1 = dest;
	vector_TYPE* vec2 = src;
	
	vec1->size = 0;
	vec1->capacity = 0;
	
	/*not much else we can do here*/
	if (!(vec1->a = malloc(vec2->capacity*sizeof(int)))) {
		assert(vec1->a != NULL);
		return;
	}
	
	memcpy(vec1->a, vec2->a, vec2->size*sizeof(TYPE));
	vec1->size = vec2->size;
	vec1->capacity = vec2->capacity;
}





int push_TYPE(vector_TYPE* vec, TYPE a)
{
	void* tmp;
	size_t tmp_sz;
	if (vec->capacity > vec->size) {
		vec->a[vec->size++] = a;
	} else {
		tmp_sz = VEC_TYPE_ALLOCATOR(vec->capacity);
		if (!(tmp = realloc(vec->a, sizeof(TYPE)*tmp_sz))) {
			assert(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->a[vec->size++] = a;
		vec->capacity = tmp_sz;
	}
	return 1;
}



TYPE pop_TYPE(vector_TYPE* vec)
{
	return vec->a[--vec->size];
}

TYPE* back_TYPE(vector_TYPE* vec)
{
	return &vec->a[vec->size-1];
}


int extend_TYPE(vector_TYPE* vec, size_t num)
{
	void* tmp;
	size_t tmp_sz;
	if (vec->capacity < vec->size + num) {
		tmp_sz = vec->capacity + num + VECTOR_TYPE_SZ;
		if (!(tmp = realloc(vec->a, sizeof(TYPE)*tmp_sz))) {
			assert(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}

	vec->size += num;
	return 1;
}


int insert_TYPE(vector_TYPE* vec, size_t i, TYPE a)
{
	void* tmp;
	size_t tmp_sz;
	if (vec->capacity > vec->size) {
		memmove(&vec->a[i+1], &vec->a[i], (vec->size-i)*sizeof(TYPE));
		vec->a[i] = a;
	} else {
		tmp_sz = VEC_TYPE_ALLOCATOR(vec->capacity);
		if (!(tmp = realloc(vec->a, sizeof(TYPE)*tmp_sz))) {
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


int insert_array_TYPE(vector_TYPE* vec, size_t i, TYPE* a, size_t num)
{
	void* tmp;
	size_t tmp_sz;
	if (vec->capacity < vec->size + num) {
		tmp_sz = vec->capacity + num + VECTOR_TYPE_SZ;
		if (!(tmp = realloc(vec->a, sizeof(TYPE)*tmp_sz))) {
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


void erase_TYPE(vector_TYPE* vec, size_t start, size_t end)
{
	size_t d = end - start + 1;
	memmove(&vec->a[start], &vec->a[end+1], (vec->size-1-end)*sizeof(TYPE));
	vec->size -= d;
}


int reserve_TYPE(vector_TYPE* vec, size_t size)
{
	void* tmp;
	if (vec->capacity < size) {
		if (!(tmp = realloc(vec->a, sizeof(TYPE)*(size+VECTOR_TYPE_SZ)))) {
			assert(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = size + VECTOR_TYPE_SZ;
	}
	return 1;
}




int set_capacity_TYPE(vector_TYPE* vec, size_t size)
{
	void* tmp;
	if (size < vec->size) {
		vec->size = size;
	}

	if (!(tmp = realloc(vec->a, sizeof(TYPE)*size))) {
		assert(tmp != NULL);
		return 0;
	}
	vec->a = tmp;
	vec->capacity = size;
	return 1;
}



void set_val_sz_TYPE(vector_TYPE* vec, TYPE val)
{
	size_t i;
	for (i=0; i<vec->size; i++) {
		vec->a[i] = val;
	}
}


void set_val_cap_TYPE(vector_TYPE* vec, TYPE val)
{
	size_t i;
	for (i=0; i<vec->capacity; i++) {
		vec->a[i] = val;
	}
}


void clear_TYPE(vector_TYPE* vec) { vec->size = 0; }

void free_vec_TYPE_heap(void* vec)
{
	vector_TYPE* tmp = vec;
	free(tmp->a);
	free(tmp);
}

void free_vec_TYPE(void* vec)
{
	vector_TYPE* tmp = vec;
	free(tmp->a);
	tmp->size = 0;
	tmp->capacity = 0;
}
