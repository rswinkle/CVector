#include "vector_template.h"

#define STDERR(X) fprintf(stderr, X)


int VECTOR_START_SZ = 50;



VECTOR_STRUCT* vec_i(size_t size, size_t capacity)
{
	VECTOR_STRUCT* vec;
	if (!(vec = malloc(sizeof(VECTOR_STRUCT)))) {
		STDERR("Error allocating memory\n");
		return NULL;
	}

	vec->size = (size > 0) ? size : 0;
	vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + VEC_I_START_SZ;

	if (!(vec->a = malloc(vec->capacity*sizeof(TYPE)))) {
		STDERR("Error allocating memory\n");
		return NULL;
	}

	return vec;
}


VECTOR_STRUCT* init_vec_i(TYPE* vals, size_t num)
{
	VECTOR_STRUCT* vec;
	
	if (!vals || num < 1)
		return NULL;
	
	if (!(vec = malloc(sizeof(VECTOR_STRUCT)))) {
		STDERR("Error allocating memory\n");
		return NULL;
	}

	vec->capacity = num + VEC_I_START_SZ;
	vec->size = num;
	if (!(vec->a = malloc(vec->capacity*sizeof(TYPE)))) {
		STDERR("Error allocating memory\n");
		return NULL;
	}

	memcpy(vec->a, vals, sizeof(TYPE)*num);

	return vec;
}




int push_backi(VECTOR_STRUCT* vec, TYPE a)
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



TYPE pop_backi(VECTOR_STRUCT* vec)
{
	return vec->a[--vec->size];
}



int inserti(VECTOR_STRUCT* vec, size_t i, TYPE a)
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



void erasei(VECTOR_STRUCT* vec, size_t start, size_t end)
{
	int d = end-start+1;
	memmove(&vec->a[start], &vec->a[end+1], (vec->size-1-end)*sizeof(TYPE));
	vec->size -= d;
}


int reservei(VECTOR_STRUCT* vec, size_t size)
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




int set_capacityi(VECTOR_STRUCT* vec, size_t size)
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



void set_val_szi(VECTOR_STRUCT* vec, TYPE val)
{
	int i;
	for (i=0; i<vec->size; i++)
		vec->a[i] = val;
}


void set_val_capi(VECTOR_STRUCT* vec, TYPE val)
{
	int i;
	for (i=0; i<vec->capacity; i++)
		vec->a[i] = val;
}


int capacityi(VECTOR_STRUCT* vec) { return vec->capacity; }

int sizei(VECTOR_STRUCT* vec) { return vec->size; }

void cleari(VECTOR_STRUCT* vec) { vec->size = 0; }

void free_veci(VECTOR_STRUCT* vec)
{
	free(vec->a);
	free(vec);
}

