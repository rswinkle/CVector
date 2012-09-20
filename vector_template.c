#include "vector_TYPE.h"

#define STDERR(X) fprintf(stderr, X)


int VECTOR_TYPE_SZ = 50;



vector_TYPE* vec_TYPE(size_t size, size_t capacity)
{
	vector_TYPE* vec;
	if (!(vec = malloc(sizeof(vector_TYPE)))) {
		STDERR("Error allocating memory\n");
		return NULL;
	}

	vec->size = (size > 0) ? size : 0;
	vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + VECTOR_TYPE_SZ;

	if (!(vec->a = malloc(vec->capacity*sizeof(TYPE)))) {
		STDERR("Error allocating memory\n");
		return NULL;
	}

	return vec;
}


vector_TYPE* init_vec_TYPE(TYPE* vals, size_t num)
{
	vector_TYPE* vec;
	
	if (!vals || num < 1)
		return NULL;
	
	if (!(vec = malloc(sizeof(vector_TYPE)))) {
		STDERR("Error allocating memory\n");
		return NULL;
	}

	vec->capacity = num + VECTOR_TYPE_SZ;
	vec->size = num;
	if (!(vec->a = malloc(vec->capacity*sizeof(TYPE)))) {
		STDERR("Error allocating memory\n");
		return NULL;
	}

	memcpy(vec->a, vals, sizeof(TYPE)*num);

	return vec;
}


int vec_TYPE_stack(vector_TYPE* vec, size_t size, size_t capacity)
{
	vec->size = (size > 0) ? size : 0;
	vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + VECTOR_TYPE_SZ;

	if (!(vec->a = malloc(vec->capacity*sizeof(TYPE)))) {
		STDERR("Error allocating memory\n");
		return 0;
	}

	return 1;
}


int init_vec_TYPE_stack(vector_TYPE* vec, TYPE* vals, size_t num)
{
	if (!vals || num < 1)
		return 0;

	vec->capacity = num + VECTOR_TYPE_SZ;
	vec->size = num;
	if (!(vec->a = malloc(vec->capacity*sizeof(TYPE)))) {
		STDERR("Error allocating memory\n");
		return 0;
	}

	memcpy(vec->a, vals, sizeof(TYPE)*num);

	return 1;
}









int push_back_TYPE(vector_TYPE* vec, TYPE a)
{
	if (vec->capacity > vec->size) {
		vec->a[vec->size++] = a;
	} else {
		vec->capacity *= 2;
		if (!(vec->a = realloc(vec->a, sizeof(TYPE)*vec->capacity))) {
			STDERR("Error allocating memory\n");
			return 0;
		}
		vec->a[vec->size++] = a;
	}
	return 1;
}



TYPE pop_back_TYPE(vector_TYPE* vec)
{
	return vec->a[--vec->size];
}



int insert_TYPE(vector_TYPE* vec, size_t i, TYPE a)
{
	if (vec->capacity > vec->size) {
		memmove(&vec->a[i+1], &vec->a[i], (vec->size-i)*sizeof(TYPE));
		vec->a[i] = a;
	} else {
		vec->capacity *= 2;
		if (!(vec->a = realloc(vec->a, sizeof(TYPE)*vec->capacity))) {
			STDERR("Error allocating memory\n");
			return 0;
		}

		memmove(&vec->a[i+1], &vec->a[i], (vec->size-i)*sizeof(TYPE));
		vec->a[i] = a;
	}

	vec->size++;
	return 1;
}



void erase_TYPE(vector_TYPE* vec, size_t start, size_t end)
{
	int d = end-start+1;
	memmove(&vec->a[start], &vec->a[end+1], (vec->size-1-end)*sizeof(TYPE));
	vec->size -= d;
}


int reserve_TYPE(vector_TYPE* vec, size_t size)
{
	if (vec->capacity < size) {
		if (!(vec->a = realloc(vec->a, sizeof(TYPE)*(size+20)))) {
			STDERR("Error allocating memory\n");
			return 0;
		}
		vec->capacity = size+20;
	}
	return 1;
}




int set_capacity_TYPE(vector_TYPE* vec, size_t size)
{
	if (size < vec->size)
		vec->size = size;

	vec->capacity = size;

	if (!(vec->a = realloc(vec->a, sizeof(TYPE)*size))) {
		STDERR("Error allocating memory\n");
		return 0;
	}
	return 1;
}



void set_val_sz_TYPE(vector_TYPE* vec, TYPE val)
{
	int i;
	for (i=0; i<vec->size; i++)
		vec->a[i] = val;
}


void set_val_cap_TYPE(vector_TYPE* vec, TYPE val)
{
	int i;
	for (i=0; i<vec->capacity; i++)
		vec->a[i] = val;
}


int capacity_TYPE(vector_TYPE* vec) { return vec->capacity; }

int size_TYPE(vector_TYPE* vec) { return vec->size; }

void clear_TYPE(vector_TYPE* vec) { vec->size = 0; }

void free_vec_TYPE(vector_TYPE* vec)
{
	free(vec->a);
	free(vec);
}

void free_vec_TYPE_stack(vector_TYPE* vec)
{
	free(vec->a);
	vec->size = 0;
	vec->capacity = 0;
}
