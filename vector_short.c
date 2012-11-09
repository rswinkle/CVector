#include "vector_short.h"

#define STDERR(X) fprintf(stderr, X)

#define VEC_short_ALLOCATOR(x) (x*2)


size_t VECTOR_short_SZ = 50;



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
		free(vec);
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
		free(vec);
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
		vec->size = vec->capacity = 0;
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
		vec->size = vec->capacity = 0;
		return 0;
	}

	memcpy(vec->a, vals, sizeof(short)*num);

	return 1;
}

void vec_short_copy(void* dest, void* src)
{
	size_t i;
	vector_short* vec1 = dest;
	vector_short* vec2 = src;
	
	vec1->size = 0;
	vec1->capacity = 0;
	
	/*not much else we can do here*/
	if (!(vec1->a = malloc(vec2->capacity*sizeof(int)))) {
		STDERR("Error allocating memory\n");
		return;
	}
	
	memcpy(vec1->a, vec2->a, vec2->size*sizeof(short));
	vec1->size = vec2->size;
	vec1->capacity = vec2->capacity;
}





int push_back_short(vector_short* vec, short a)
{
	void* tmp;
	size_t tmp_sz;
	if (vec->capacity > vec->size) {
		vec->a[vec->size++] = a;
	} else {
		tmp_sz = VEC_short_ALLOCATOR(vec->capacity);
		if (!(tmp = realloc(vec->a, sizeof(short)*tmp_sz))) {
			STDERR("Error allocating memory\n");
			return 0;
		}
		vec->a = tmp;
		vec->a[vec->size++] = a;
		vec->capacity = tmp_sz;
	}
	return 1;
}



short pop_back_short(vector_short* vec)
{
	return vec->a[--vec->size];
}

short* back_short(vector_short* vec)
{
	return &vec->a[vec->size-1];
}


int extend_short(vector_short* vec, size_t num)
{
	void* tmp;
	size_t tmp_sz;
	if (vec->capacity < vec->size + num) {
		tmp_sz = vec->capacity + num + VECTOR_short_SZ;
		if (!(tmp = realloc(vec->a, sizeof(short)*tmp_sz))) {
			STDERR("Error allocating memory\n");
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
	void* tmp;
	size_t tmp_sz;
	if (vec->capacity > vec->size) {
		memmove(&vec->a[i+1], &vec->a[i], (vec->size-i)*sizeof(short));
		vec->a[i] = a;
	} else {
		tmp_sz = VEC_short_ALLOCATOR(vec->capacity);
		if (!(tmp = realloc(vec->a, sizeof(short)*tmp_sz))) {
			STDERR("Error allocating memory\n");
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
	void* tmp;
	size_t tmp_sz;
	if (vec->capacity < vec->size + num) {
		tmp_sz = vec->capacity + num + VECTOR_short_SZ;
		if (!(tmp = realloc(vec->a, sizeof(short)*tmp_sz))) {
			STDERR("Error allocating memory\n");
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
	void* tmp;
	if (vec->capacity < size) {
		if (!(tmp = realloc(vec->a, sizeof(short)*(size+VECTOR_short_SZ)))) {
			STDERR("Error allocating memory\n");
			return 0;
		}
		vec->a = tmp;
		vec->capacity = size + VECTOR_short_SZ;
	}
	return 1;
}




int set_capacity_short(vector_short* vec, size_t size)
{
	void* tmp;
	if (size < vec->size)
		vec->size = size;

	if (!(tmp = realloc(vec->a, sizeof(short)*size))) {
		STDERR("Error allocating memory\n");
		return 0;
	}
	vec->a = tmp;
	vec->capacity = size;
	return 1;
}



void set_val_sz_short(vector_short* vec, short val)
{
	size_t i;
	for (i=0; i<vec->size; i++)
		vec->a[i] = val;
}


void set_val_cap_short(vector_short* vec, short val)
{
	size_t i;
	for (i=0; i<vec->capacity; i++)
		vec->a[i] = val;
}


int capacity_short(vector_short* vec) { return vec->capacity; }

int size_short(vector_short* vec) { return vec->size; }

void clear_short(vector_short* vec) { vec->size = 0; }

void free_vec_short(void* vec)
{
	vector_short* tmp = vec;
	free(tmp->a);
	free(tmp);
}

void free_vec_short_stack(void* vec)
{
	vector_short* tmp = vec;
	free(tmp->a);
	tmp->size = 0;
	tmp->capacity = 0;
}
