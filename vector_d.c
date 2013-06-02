#include "vector_d.h"

#include <assert.h>

#define STDERR(X) fprintf(stderr, X)



size_t VEC_D_START_SZ = 50;


#define VECD_ALLOCATOR(x) ((x) * 2)




/**
 * Creates a new vector_d on the heap.
 * Vector size set to (size > 0) ? size : 0;
 * Capacity to (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + VEC_D_START_SZ
 * in other words capacity has to be at least 1 and >= to vec->size of course.
 */
vector_d* vec_d_heap(size_t size, size_t capacity)
{
	vector_d* vec;
	
	if (!(vec = malloc(sizeof(vector_d)))) {
		assert(vec != NULL);
		return NULL;
	}

	vec->size = (size > 0) ? size : 0;
	vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + VEC_D_START_SZ;

	if (!(vec->a = malloc(vec->capacity*sizeof(double)))) {
		assert(vec->a != NULL);
		free(vec);
		return NULL;
	}

	return vec;
}


/** Create (on the heap) and initialize vector_d with num elements of vals.
 *  Capacity is set to num + VEC_D_START_SZ.
 */
vector_d* init_vec_d_heap(double* vals, size_t num)
{
	vector_d* vec;
	
	if (!(vec = malloc(sizeof(vector_d)))) {
		assert(vec != NULL);
		return NULL;
	}

	vec->capacity = num + VEC_D_START_SZ;
	vec->size = num;
	if (!(vec->a = malloc(vec->capacity*sizeof(double)))) {
		assert(vec->a != NULL);
		free(vec);
		return NULL;
	}

	memcpy(vec->a, vals, sizeof(double)*num);

	return vec;
}


/** Same as vec_d_heap() except the vector passed in was declared on the stack so
 *  it isn't allocated in this function.  Use the free_vec_d in this case.
 *  This and init_vec_d should be preferred over the heap versions.
 */
int vec_d(vector_d* vec, size_t size, size_t capacity)
{
	vec->size = (size > 0) ? size : 0;
	vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + VEC_D_START_SZ;

	if (!(vec->a = malloc(vec->capacity*sizeof(double)))) {
		assert(vec->a != NULL);
		vec->size = vec->capacity = 0;
		return 0;
	}

	return 1;
}

/** Same as init_vec_d_heap() except the vector passed in was declared on the stack so
 *  it isn't allocated in this function.  Use the free_vec_d in this case.
 */
int init_vec_d(vector_d* vec, double* vals, size_t num)
{
	vec->capacity = num + VEC_D_START_SZ;
	vec->size = num;
	if (!(vec->a = malloc(vec->capacity*sizeof(double)))) {
		assert(vec->a != NULL);
		vec->size = vec->capacity = 0;
		return 0;
	}

	memcpy(vec->a, vals, sizeof(double)*num);

	return 1;
}

/** Makes dest an identical copy of src.  The parameters
 *  are void so it can be used as the constructor when making
 *  a vector of vector_d's.  Assumes dest (the structure)
 *  is already allocated (probably on the stack) and that
 *  capacity is 0 (ie the array doesn't need to be freed).
 */
void vec_d_copy(void* dest, void* src)
{
	vector_d* vec1 = dest;
	vector_d* vec2 = src;
	
	vec1->size = 0;
	vec1->capacity = 0;
	
	/*not much else we can do here*/
	if (!(vec1->a = malloc(vec2->capacity*sizeof(double)))) {
		assert(vec1->a != NULL);
		return;
	}
	
	memcpy(vec1->a, vec2->a, vec2->size*sizeof(double));
	vec1->size = vec2->size;
	vec1->capacity = vec2->capacity;
}







/** Append a to end of vector (size increased 1).
 * Capacity is increased by doubling when necessary.
 */
int push_d(vector_d* vec, double a)
{
	void* tmp;
	size_t tmp_sz;
	if (vec->capacity == vec->size) {
		tmp_sz = VECD_ALLOCATOR(vec->capacity);
		if (!(tmp = realloc(vec->a, sizeof(double)*tmp_sz))) {
			assert(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}
	vec->a[vec->size++] = a;
	return 1;
}


/** Remove and return the last element (size decreased 1).*/
double pop_d(vector_d* vec)
{
	return vec->a[--vec->size];
}


/** Return pointer to last element */
double* back_d(vector_d* vec)
{
	return &vec->a[vec->size-1];
}




/** Increase the size of the array num items.  Items
 *  are not initialized to anything */
int extend_d(vector_d* vec, size_t num)
{
	void* tmp;
	size_t tmp_sz;
	if (vec->capacity < vec->size + num) {
		tmp_sz = vec->capacity + num + VEC_D_START_SZ;
		if (!(tmp = realloc(vec->a, sizeof(double)*tmp_sz))) {
			assert(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}

	vec->size += num;
	return 1;
}




/**
 * Insert a at index i (0 based).
 * Everything from that index and right is shifted one to the right.
 */
int insert_d(vector_d* vec, size_t i, double a)
{
	void* tmp;
	size_t tmp_sz;
	if (vec->capacity == vec->size) {
		tmp_sz = VECD_ALLOCATOR(vec->capacity);
		if (!(tmp = realloc(vec->a, sizeof(double)*tmp_sz))) {
			assert(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}
	
	memmove(&vec->a[i+1], &vec->a[i], (vec->size-i)*sizeof(double));
	vec->a[i] = a;
	vec->size++;
	return 1;
}

/**
 * Insert the first num elements of array a at index i.
 * Note that it is the user's responsibility to pass in valid
 * arguments.  Also memcpy is used so don't try to insert
 * part of the vector array into itself (that would require memmove)
 */
int insert_array_d(vector_d* vec, size_t i, double* a, size_t num)
{
	void* tmp;
	size_t tmp_sz;
	if (vec->capacity < vec->size + num) {
		tmp_sz = vec->capacity + num + VEC_D_START_SZ;
		if (!(tmp = realloc(vec->a, sizeof(double)*tmp_sz))) {
			assert(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}

	memmove(&vec->a[i+num], &vec->a[i], (vec->size-i)*sizeof(double));
	memcpy(&vec->a[i], a, num*sizeof(double));
	vec->size += num;
	return 1;
}



/**
 * Erases elements from start to end inclusive.
 * Example erase_d(myvec, 1, 3) would remove elements at 1, 2, and 3 and the element
 * that was at index 4 would now be at 1 etc.
 */
void erase_d(vector_d* vec, size_t start, size_t end)
{
	size_t d = end - start + 1;
	memmove(&vec->a[start], &vec->a[end+1], (vec->size-1-end)*sizeof(double));
	vec->size -= d;
}


/** Make sure capacity is at least size(parameter not member). */
int reserve_d(vector_d* vec, size_t size)
{
	void* tmp;
	if (vec->capacity < size) {
		if (!(tmp = realloc(vec->a, sizeof(double)*(size+VEC_D_START_SZ)))) {
			assert(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = size + VEC_D_START_SZ;
	}
	return 1;
}


/** Set capacity to size.
 * You will lose data if you shrink the capacity below the current size.
 * If you do, the size will be set to capacity of course.
*/
int set_capacity_d(vector_d* vec, size_t size)
{
	void* tmp;
	if (size < vec->size)
		vec->size = size;

	if (!(tmp = realloc(vec->a, sizeof(double)*size))) {
		assert(tmp != NULL);
		return 0;
	}
	vec->a = tmp;
	vec->capacity = size;
	return 1;
}


/** Set all size elements to val. */
void set_val_sz_d(vector_d* vec, double val)
{
	size_t i;
	for(i=0; i<vec->size; i++) {
		vec->a[i] = val;
	}
}


/** Fills entire allocated array (capacity) with val. */
void set_val_cap_d(vector_d* vec, double val)
{
	size_t i;
	for(i=0; i<vec->capacity; i++) {
		vec->a[i] = val;
	}
}



/** Sets size to 0 (does not clear contents).*/
void clear_d(vector_d* vec) { vec->size = 0; }


/** Frees everything so don't use vec after calling this. */
void free_vec_d_heap(void* vec)
{
	vector_d* tmp = vec;
	free(tmp->a);
	free(tmp);
}

/** Frees the internal array and sets size and capacity to 0 */
void free_vec_d(void* vec)
{
	vector_d* tmp = vec;
	free(tmp->a);
	tmp->size = 0;
	tmp->capacity = 0;
}
