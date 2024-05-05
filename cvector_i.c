#include "cvector_i.h"



#if defined(CVEC_MALLOC) && defined(CVEC_FREE) && defined(CVEC_REALLOC)
/* ok */
#elif !defined(CVEC_MALLOC) && !defined(CVEC_FREE) && !defined(CVEC_REALLOC)
/* ok */
#else
#error "Must define all or none of CVEC_MALLOC, CVEC_FREE, and CVEC_REALLOC."
#endif

#ifndef CVEC_MALLOC
#include <stdlib.h>
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

cvec_sz CVEC_I_START_SZ = 50;

#define CVEC_I_ALLOCATOR(x) ((x+1) * 2)

/**
 * Creates a new cvector_i on the heap.
 * Vector size set to (size > 0) ? size : 0;
 * Capacity to (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + CVEC_I_START_SZ
 * in other words capacity has to be at least 1 and >= to vec->size of course.
 */
cvector_i* cvec_i_heap(cvec_sz size, cvec_sz capacity)
{
	cvector_i* vec;
	if (!(vec = (cvector_i*)CVEC_MALLOC(sizeof(cvector_i)))) {
		CVEC_ASSERT(vec != NULL);
		return NULL;
	}

	vec->size = size;
	vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + CVEC_I_START_SZ;

	if (!(vec->a = (int*)CVEC_MALLOC(vec->capacity*sizeof(int)))) {
		CVEC_ASSERT(vec->a != NULL);
		CVEC_FREE(vec);
		return NULL;
	}

	return vec;
}

/** Create (on the heap) and initialize cvector_i with num elements of vals.
 *  Capacity is set to num + CVEC_I_START_SZ.
 */
cvector_i* cvec_init_i_heap(int* vals, cvec_sz num)
{
	cvector_i* vec;
	
	if (!(vec = (cvector_i*)CVEC_MALLOC(sizeof(cvector_i)))) {
		CVEC_ASSERT(vec != NULL);
		return NULL;
	}

	vec->capacity = num + CVEC_I_START_SZ;
	vec->size = num;
	if (!(vec->a = (int*)CVEC_MALLOC(vec->capacity*sizeof(int)))) {
		CVEC_ASSERT(vec->a != NULL);
		CVEC_FREE(vec);
		return NULL;
	}

	CVEC_MEMMOVE(vec->a, vals, sizeof(int)*num);

	return vec;
}

/** Same as cvec_i_heap() except the vector passed in was declared on the stack so
 *  it isn't allocated in this function.  Use the cvec_free_i in this case.
 *  This and cvec_init_i should be preferred over the heap versions.
 */
int cvec_i(cvector_i* vec, cvec_sz size, cvec_sz capacity)
{
	vec->size = size;
	vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + CVEC_I_START_SZ;

	if (!(vec->a = (int*)CVEC_MALLOC(vec->capacity*sizeof(int)))) {
		CVEC_ASSERT(vec->a != NULL);
		vec->size = vec->capacity = 0;
		return 0;
	}

	return 1;
}

/** Same as cvec_init_i_heap() except the vector passed in was declared on the stack so
 *  it isn't allocated in this function.
 */
int cvec_init_i(cvector_i* vec, int* vals, cvec_sz num)
{
	vec->capacity = num + CVEC_I_START_SZ;
	vec->size = num;
	if (!(vec->a = (int*)CVEC_MALLOC(vec->capacity*sizeof(int)))) {
		CVEC_ASSERT(vec->a != NULL);
		vec->size = vec->capacity = 0;
		return 0;
	}

	CVEC_MEMMOVE(vec->a, vals, sizeof(int)*num);

	return 1;
}

/** Makes dest a copy of src.  The parameters
 *  are void so it can be used as the constructor when making
 *  a vector of cvector_i's.  Assumes dest (the structure)
 *  is already allocated (probably on the stack) and that
 *  capacity is 0 (ie the array doesn't need to be freed).
 *
 *  Really just a wrapper around copy, that initializes dest/vec1's
 *  members to NULL/0.  If you pre-initialized dest to 0, you could
 *  just use copy.
 */
int cvec_copyc_i(void* dest, void* src)
{
	cvector_i* vec1 = (cvector_i*)dest;
	cvector_i* vec2 = (cvector_i*)src;

	vec1->a = NULL;
	vec1->size = 0;
	vec1->capacity = 0;

	return cvec_copy_i(vec1, vec2);
}

/** Makes dest a copy of src.  Assumes dest
 * (the structure) is already allocated (probably on the stack) and
 * is in a valid state (ie array is either NULL or allocated with
 * size and capacity set appropriately).
 *
 * TODO Should I copy capacity, so dest is truly identical or do
 * I only care about the actual contents, and let dest->cap = src->size
 * maybe plus CVEC_I_START_SZ
 */
int cvec_copy_i(cvector_i* dest, cvector_i* src)
{
	int* tmp = NULL;
	if (!(tmp = (int*)CVEC_REALLOC(dest->a, src->capacity*sizeof(int)))) {
		CVEC_ASSERT(tmp != NULL);
		return 0;
	}
	dest->a = tmp;
	
	CVEC_MEMMOVE(dest->a, src->a, src->size*sizeof(int));
	dest->size = src->size;
	dest->capacity = src->capacity;
	return 1;
}

/**
 * Append a to end of vector (size increased 1).
 * Capacity is increased by doubling when necessary.
 */
int cvec_push_i(cvector_i* vec, int a)
{
	int* tmp;
	cvec_sz tmp_sz;
	if (vec->capacity == vec->size) {
		tmp_sz = CVEC_I_ALLOCATOR(vec->capacity);
		if (!(tmp = (int*)CVEC_REALLOC(vec->a, sizeof(int)*tmp_sz))) {
			CVEC_ASSERT(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}
	
	vec->a[vec->size++] = a;
	return 1;
}

/** Remove and return the last element (size decreased 1).*/
int cvec_pop_i(cvector_i* vec)
{
	return vec->a[--vec->size];
}

/** Return pointer to last element */
int* cvec_back_i(cvector_i* vec)
{
	return &vec->a[vec->size-1];
}

/** Increase the size of the array num items.  Items
 *  are not initialized to anything */
int cvec_extend_i(cvector_i* vec, cvec_sz num)
{
	int* tmp;
	cvec_sz tmp_sz;
	if (vec->capacity < vec->size + num) {
		tmp_sz = vec->capacity + num + CVEC_I_START_SZ;
		if (!(tmp = (int*)CVEC_REALLOC(vec->a, sizeof(int)*tmp_sz))) {
			CVEC_ASSERT(tmp != NULL);
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
int cvec_insert_i(cvector_i* vec, cvec_sz i, int a)
{
	int* tmp;
	cvec_sz tmp_sz;
	if (vec->capacity == vec->size) {
		tmp_sz = CVEC_I_ALLOCATOR(vec->capacity);
		if (!(tmp = (int*)CVEC_REALLOC(vec->a, sizeof(int)*tmp_sz))) {
			CVEC_ASSERT(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}

	CVEC_MEMMOVE(&vec->a[i+1], &vec->a[i], (vec->size-i)*sizeof(int));
	vec->a[i] = a;
	vec->size++;
	return 1;
}

/**
 * Insert the first num elements of array a at index i.
 * Note that it is the user's responsibility to pass in valid
 * arguments.  Also CVEC_MEMMOVE is used so don't try to insert
 * part of the vector array into itself (that would require CVEC_MEMMOVE)
 */
int cvec_insert_array_i(cvector_i* vec, cvec_sz i, int* a, cvec_sz num)
{
	int* tmp;
	cvec_sz tmp_sz;
	if (vec->capacity < vec->size + num) {
		tmp_sz = vec->capacity + num + CVEC_I_START_SZ;
		if (!(tmp = (int*)CVEC_REALLOC(vec->a, sizeof(int)*tmp_sz))) {
			CVEC_ASSERT(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}

	CVEC_MEMMOVE(&vec->a[i+num], &vec->a[i], (vec->size-i)*sizeof(int));
	CVEC_MEMMOVE(&vec->a[i], a, num*sizeof(int));
	vec->size += num;
	return 1;
}

/** Replace value at index i with a, return original value. */
int cvec_replace_i(cvector_i* vec, cvec_sz i, int a)
{
	int tmp = vec->a[i];
	vec->a[i] = a;
	return tmp;
}

/**
 * Erases elements from start to end inclusive.
 * Example cvec_erase_i(myvec, 1, 3) would remove elements at 1, 2, and 3 and the element
 * that was at index 4 would now be at 1 etc.
 */
void cvec_erase_i(cvector_i* vec, cvec_sz start, cvec_sz end)
{
	cvec_sz d = end - start + 1;
	CVEC_MEMMOVE(&vec->a[start], &vec->a[end+1], (vec->size-1-end)*sizeof(int));
	vec->size -= d;
}

/** Make sure capacity is at least size(parameter not member). */
int cvec_reserve_i(cvector_i* vec, cvec_sz size)
{
	int* tmp;
	if (vec->capacity < size) {
		if (!(tmp = (int*)CVEC_REALLOC(vec->a, sizeof(int)*(size+CVEC_I_START_SZ)))) {
			CVEC_ASSERT(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = size + CVEC_I_START_SZ;
	}
	return 1;
}

/** Set capacity to size.
 * You will lose data if you shrink the capacity below the current size.
 * If you do, the size will be set to capacity of course.
*/
int cvec_set_cap_i(cvector_i* vec, cvec_sz size)
{
	int* tmp;
	if (size < vec->size) {
		vec->size = size;
	}

	if (!(tmp = (int*)CVEC_REALLOC(vec->a, sizeof(int)*size))) {
		CVEC_ASSERT(tmp != NULL);
		return 0;
	}
	vec->a = tmp;
	vec->capacity = size;
	return 1;
}

/** Set all size elements to val. */
void cvec_set_val_sz_i(cvector_i* vec, int val)
{
	cvec_sz i;
	for (i=0; i<vec->size; i++) {
		vec->a[i] = val;
	}
}

/** Fills entire allocated array (capacity) with val. */
void cvec_set_val_cap_i(cvector_i* vec, int val)
{
	cvec_sz i;
	for (i=0; i<vec->capacity; i++) {
		vec->a[i] = val;
	}
}

/** Sets size to 0 (does not clear contents).*/
void cvec_clear_i(cvector_i* vec) { vec->size = 0; }

/** Frees everything so don't use vec after calling this.
 *  Passing NULL is a NO-OP, matching the behavior of free(). */
void cvec_free_i_heap(void* vec)
{
	cvector_i* tmp = (cvector_i*)vec;
	if (!tmp) return;
	CVEC_FREE(tmp->a);
	CVEC_FREE(tmp);
}

/** Frees the internal array and sets size and capacity to 0 */
void cvec_free_i(void* vec)
{
	cvector_i* tmp = (cvector_i*)vec;
	CVEC_FREE(tmp->a);
	tmp->size = 0;
	tmp->capacity = 0;
}

