#include "vector_i.h"

#include <assert.h>

#define STDERR(X) fprintf(stderr, X)



size_t VEC_I_START_SZ = 50;

#define VEC_I_ALLOCATOR(x) ((x) * 2)




/**
 * Creates a new vector_i on the heap.
 * Vector size set to (size > 0) ? size : 0;
 * Capacity to (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + VEC_I_START_SZ
 * in other words capacity has to be at least 1 and >= to vec->size of course. 
 */
vector_i* vec_i_heap(size_t size, size_t capacity)
{
	vector_i* vec;
	if (!(vec = (vector_i*)malloc(sizeof(vector_i)))) {
		assert(vec != NULL);
		return NULL;
	}

	vec->size = size;
	vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + VEC_I_START_SZ;

	if (!(vec->a = (int*)malloc(vec->capacity*sizeof(int)))) {
		assert(vec->a != NULL);
		free(vec);
		return NULL;
	}

	return vec;
}

/** Create (on the heap) and initialize vector_i with num elements of vals.
 *  Capacity is set to num + VEC_I_START_SZ.
 */
vector_i* init_vec_i_heap(int* vals, size_t num)
{
	vector_i* vec;
	
	if (!(vec = (vector_i*)malloc(sizeof(vector_i)))) {
		assert(vec != NULL);
		return NULL;
	}

	vec->capacity = num + VEC_I_START_SZ;
	vec->size = num;
	if (!(vec->a = (int*)malloc(vec->capacity*sizeof(int)))) {
		assert(vec->a != NULL);
		free(vec);
		return NULL;
	}

	memcpy(vec->a, vals, sizeof(int)*num);

	return vec;
}

/** Same as vec_i_heap() except the vector passed in was declared on the stack so
 *  it isn't allocated in this function.  Use the free_vec_i in this case.
 *  This and init_vec_i should be preferred over the heap versions.
 */
int vec_i(vector_i* vec, size_t size, size_t capacity)
{
	vec->size = size;
	vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + VEC_I_START_SZ;

	if (!(vec->a = (int*)malloc(vec->capacity*sizeof(int)))) {
		assert(vec->a != NULL);
		vec->size = vec->capacity = 0;
		return 0;
	}

	return 1;
}


/** Same as init_vec_i_heap() except the vector passed in was declared on the stack so
 *  it isn't allocated in this function.
 */
int init_vec_i(vector_i* vec, int* vals, size_t num)
{
	vec->capacity = num + VEC_I_START_SZ;
	vec->size = num;
	if (!(vec->a = (int*)malloc(vec->capacity*sizeof(int)))) {
		assert(vec->a != NULL);
		vec->size = vec->capacity = 0;
		return 0;
	}

	memcpy(vec->a, vals, sizeof(int)*num);

	return 1;
}


/** Makes dest an identical copy of src.  The parameters
 *  are void so it can be used as the constructor when making
 *  a vector of vector_i's.  Assumes dest (the structure)
 *  is already allocated (probably on the stack) and that
 *  capacity is 0 (ie the array doesn't need to be freed).
 */
void vec_i_copy(void* dest, void* src)
{
	vector_i* vec1 = (vector_i*)dest;
	vector_i* vec2 = (vector_i*)src;
	
	vec1->size = 0;
	vec1->capacity = 0;
	
	/*not much else we can do here*/
	if (!(vec1->a = (int*)malloc(vec2->capacity*sizeof(int)))) {
		assert(vec1->a != NULL);
		return;
	}
	
	memcpy(vec1->a, vec2->a, vec2->size*sizeof(int));
	vec1->size = vec2->size;
	vec1->capacity = vec2->capacity;
}





/**
 * Append a to end of vector (size increased 1).
 * Capacity is increased by doubling when necessary.
 */
int push_i(vector_i* vec, int a)
{
	int* tmp;
	size_t tmp_sz;
	if (vec->capacity == vec->size) {
		tmp_sz = VEC_I_ALLOCATOR(vec->capacity);
		if (!(tmp = (int*)realloc(vec->a, sizeof(int)*tmp_sz))) {
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
int pop_i(vector_i* vec)
{
	return vec->a[--vec->size];
}

/** Return pointer to last element */
int* back_i(vector_i* vec)
{
	return &vec->a[vec->size-1];
}



/** Increase the size of the array num items.  Items
 *  are not initialized to anything */
int extend_i(vector_i* vec, size_t num)
{
	int* tmp;
	size_t tmp_sz;
	if (vec->capacity < vec->size + num) {
		tmp_sz = vec->capacity + num + VEC_I_START_SZ;
		if (!(tmp = (int*)realloc(vec->a, sizeof(int)*tmp_sz))) {
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
int insert_i(vector_i* vec, size_t i, int a)
{
	int* tmp;
	size_t tmp_sz;
	if (vec->capacity == vec->size) {
		tmp_sz = VEC_I_ALLOCATOR(vec->capacity);
		if (!(tmp = (int*)realloc(vec->a, sizeof(int)*tmp_sz))) {
			assert(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}

	memmove(&vec->a[i+1], &vec->a[i], (vec->size-i)*sizeof(int));
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
int insert_array_i(vector_i* vec, size_t i, int* a, size_t num)
{
	int* tmp;
	size_t tmp_sz;
	if (vec->capacity < vec->size + num) {
		tmp_sz = vec->capacity + num + VEC_I_START_SZ;
		if (!(tmp = (int*)realloc(vec->a, sizeof(int)*tmp_sz))) {
			assert(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}

	memmove(&vec->a[i+num], &vec->a[i], (vec->size-i)*sizeof(int));
	memcpy(&vec->a[i], a, num*sizeof(int));
	vec->size += num;
	return 1;
}





/**
 * Erases elements from start to end inclusive.
 * Example erase_i(myvec, 1, 3) would remove elements at 1, 2, and 3 and the element
 * that was at index 4 would now be at 1 etc.
 */
void erase_i(vector_i* vec, size_t start, size_t end)
{
	size_t d = end - start + 1;
	memmove(&vec->a[start], &vec->a[end+1], (vec->size-1-end)*sizeof(int));
	vec->size -= d;
}


/** Make sure capacity is at least size(parameter not member). */
int reserve_i(vector_i* vec, size_t size)
{
	int* tmp;
	if (vec->capacity < size) {
		if (!(tmp = (int*)realloc(vec->a, sizeof(int)*(size+VEC_I_START_SZ)))) {
			assert(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = size + VEC_I_START_SZ;
	}
	return 1;
}



/** Set capacity to size.
 * You will lose data if you shrink the capacity below the current size.
 * If you do, the size will be set to capacity of course.
*/
int set_capacity_i(vector_i* vec, size_t size)
{
	int* tmp;
	if (size < vec->size) {
		vec->size = size;
	}

	if (!(tmp = (int*)realloc(vec->a, sizeof(int)*size))) {
		assert(tmp != NULL);
		return 0;
	}
	vec->a = tmp;
	vec->capacity = size;
	return 1;
}



/** Set all size elements to val. */
void set_val_sz_i(vector_i* vec, int val)
{
	size_t i;
	for (i=0; i<vec->size; i++) {
		vec->a[i] = val;
	}
}


/** Fills entire allocated array (capacity) with val. */
void set_val_cap_i(vector_i* vec, int val)
{
	size_t i;
	for (i=0; i<vec->capacity; i++) {
		vec->a[i] = val;
	}
}


/** Sets size to 0 (does not clear contents).*/
void clear_i(vector_i* vec) { vec->size = 0; }

/** Frees everything so don't use vec after calling this. */
void free_vec_i_heap(void* vec)
{
	vector_i* tmp = (vector_i*)vec;
	free(tmp->a);
	free(tmp);
}

/** Frees the internal array and sets size and capacity to 0 */
void free_vec_i(void* vec)
{
	vector_i* tmp = (vector_i*)vec;
	free(tmp->a);
	tmp->size = 0;
	tmp->capacity = 0;
}

