#include "vector_short.h"

#define STDERR(X) fprintf(stderr, X)


int VECTOR_short_SZ = 50;



vector_short* vec_short(size_t size, size_t capacity)
{
	vector_short* vec;
	if (!(vec = malloc(sizeof(vector_short)))) {
		STDERR("Error allocating memory\n");
		return NULL;
	}

	vec->size = (size > 0) ? size : 0;
	vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + VECTOR_short_SZ;

	if (!(vec->a = malloc(vec->capacity*sizeof(short)))) {
		STDERR("Error allocating memory\n");
		return NULL;
	}

	return vec;
}


vector_short* init_vec_short(short* vals, size_t num)
{
	vector_short* vec;
	
	if (!vals || num < 1)
		return NULL;
	
	if (!(vec = malloc(sizeof(vector_short)))) {
		STDERR("Error allocating memory\n");
		return NULL;
	}

	vec->capacity = num + VECTOR_short_SZ;
	vec->size = num;
	if (!(vec->a = malloc(vec->capacity*sizeof(short)))) {
		STDERR("Error allocating memory\n");
		return NULL;
	}

	memcpy(vec->a, vals, sizeof(short)*num);

	return vec;
}


int vec_short_stack(vector_short* vec, size_t size, size_t capacity)
{
	vec->size = (size > 0) ? size : 0;
	vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + VECTOR_short_SZ;

	if (!(vec->a = malloc(vec->capacity*sizeof(short)))) {
		STDERR("Error allocating memory\n");
		return 0;
	}

	return 1;
}


int init_vec_short_stack(vector_short* vec, short* vals, size_t num)
{
	if (!vals || num < 1)
		return 0;

	vec->capacity = num + VECTOR_short_SZ;
	vec->size = num;
	if (!(vec->a = malloc(vec->capacity*sizeof(short)))) {
		STDERR("Error allocating memory\n");
		return 0;
	}

	memcpy(vec->a, vals, sizeof(short)*num);

	return 1;
}









int push_back_short(vector_short* vec, short a)
{
	if (vec->capacity > vec->size) {
		vec->a[vec->size++] = a;
	} else {
		vec->capacity *= 2;
		if (!(vec->a = realloc(vec->a, sizeof(short)*vec->capacity))) {
			STDERR("Error allocating memory\n");
			return 0;
		}
		vec->a[vec->size++] = a;
	}
	return 1;
}



short pop_back_short(vector_short* vec)
{
	return vec->a[--vec->size];
}



int insert_short(vector_short* vec, size_t i, short a)
{
	if (vec->capacity > vec->size) {
		memmove(&vec->a[i+1], &vec->a[i], (vec->size-i)*sizeof(short));
		vec->a[i] = a;
	} else {
		vec->capacity *= 2;
		if (!(vec->a = realloc(vec->a, sizeof(short)*vec->capacity))) {
			STDERR("Error allocating memory\n");
			return 0;
		}

		memmove(&vec->a[i+1], &vec->a[i], (vec->size-i)*sizeof(short));
		vec->a[i] = a;
	}

	vec->size++;
	return 1;
}



void erase_short(vector_short* vec, size_t start, size_t end)
{
	int d = end-start+1;
	memmove(&vec->a[start], &vec->a[end+1], (vec->size-1-end)*sizeof(short));
	vec->size -= d;
}


int reserve_short(vector_short* vec, size_t size)
{
	if (vec->capacity < size) {
		if (!(vec->a = realloc(vec->a, sizeof(short)*(size+20)))) {
			STDERR("Error allocating memory\n");
			return 0;
		}
		vec->capacity = size+20;
	}
	return 1;
}




int set_capacity_short(vector_short* vec, size_t size)
{
	if (size < vec->size)
		vec->size = size;

	vec->capacity = size;

	if (!(vec->a = realloc(vec->a, sizeof(short)*size))) {
		STDERR("Error allocating memory\n");
		return 0;
	}
	return 1;
}



void set_val_sz_short(vector_short* vec, short val)
{
	int i;
	for (i=0; i<vec->size; i++)
		vec->a[i] = val;
}


void set_val_cap_short(vector_short* vec, short val)
{
	int i;
	for (i=0; i<vec->capacity; i++)
		vec->a[i] = val;
}


int capacity_short(vector_short* vec) { return vec->capacity; }

int size_short(vector_short* vec) { return vec->size; }

void clear_short(vector_short* vec) { vec->size = 0; }

void free_vec_short(vector_short* vec)
{
	free(vec->a);
	free(vec);
}

void free_vec_short_stack(vector_short* vec)
{
	free(vec->a);
	vec->size = 0;
	vec->capacity = 0;
}
