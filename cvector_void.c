#include "cvector_void.h"



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

cvec_sz CVEC_VOID_START_SZ = 20;

#define CVEC_VOID_ALLOCATOR(x) ((x+1) * 2)

/**
 * Creates a new vector on the heap.
 * Vector size set to (size > 0) ? size : 0;
 * Capacity to (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + CVEC_VOID_START_SZ
 * in other words capacity has to be at least 1 and >= to vec->size of course.
 * elem_sz is the size of the type you want to store ( ie sizeof(T) where T is your type ).
 * You can pass in an optional function, elem_free, to be called on every element before it is erased
 * from the vector to free any dynamically allocated memory.  Likewise you can pass in elem_init to be
 * as a sort of copy constructor for any insertions if you needed some kind of deep copy/allocations.
 *
 * For example if you passed in sizeof(char*) for elem_sz, and wrappers around the standard free(void*)
 * function for elem_free and CVEC_STRDUP for elem_init you could
 * make vector work *almost* exactly like cvector_str.  The main difference is cvector_str does not
 * check for failure of CVEC_STRDUP while cvector_void does check for failure of elem_init.  The other
 * minor differences are popm and replacem are macros in cvector_str (and the latter returns the result
 * rather than using a double pointer return parameter) and depending on how you defined elem_init
 * and whether you're using the 'move' functions, you have to pass in char**'s instead of char*'s
 * because cvector_void has to use memmove rather than straight assignment.
 *
 * Pass in NULL, to not use the function parameters.
 *
 * The function remove and the 'move' functions (with the m suffix) do not call elem_init or elem_free
 * even if they are set.  This gives you some flexibility and performance when you already have things
 * allocated or want to keep things after removing them from the vector but only some of the time (otherwise
 * you wouldn't have defined elem_free/elem_init in the first place).
 *
 * See the other functions and the tests for more behavioral/usage details.
 */
cvector_void* cvec_void_heap(cvec_sz size, cvec_sz capacity, cvec_sz elem_sz, void(*elem_free)(void*), int(*elem_init)(void*, void*))
{
	cvector_void* vec;
	if (!(vec = (cvector_void*)CVEC_MALLOC(sizeof(cvector_void)))) {
		CVEC_ASSERT(vec != NULL);
		return NULL;
	}

	vec->size = size;
	vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + CVEC_VOID_START_SZ;

	vec->elem_size = elem_sz;
	
	if (!(vec->a = (cvec_u8*)CVEC_MALLOC(vec->capacity*elem_sz))) {
		CVEC_ASSERT(vec->a != NULL);
		CVEC_FREE(vec);
		return NULL;
	}
	/* not clearing to 0 here as in cvector_str because elem_free cannot be calling CVEC_FREE directly
	 * since it takes the address of the element not the element itself */

	vec->elem_free = elem_free;
	vec->elem_init = elem_init;

	return vec;
}

/** Create (on the heap) and initialize vector with num elements of vals.
 *  elem_sz is the size of the type you want to store ( ie sizeof(T) where T is your type ).
 *  See cvec_void_heap() for more information about the elem_free and elem_init parameters.
 */
cvector_void* cvec_init_void_heap(void* vals, cvec_sz num, cvec_sz elem_sz, void(*elem_free)(void*), int(*elem_init)(void*, void*))
{
	cvector_void* vec;
	cvec_sz i;
	
	if (!(vec = (cvector_void*)CVEC_MALLOC(sizeof(cvector_void)))) {
		CVEC_ASSERT(vec != NULL);
		return NULL;
	}

	vec->elem_size = elem_sz;

	vec->capacity = num + CVEC_VOID_START_SZ;
	vec->size = num;
	if (!(vec->a = (cvec_u8*)CVEC_MALLOC(vec->capacity*elem_sz))) {
		CVEC_ASSERT(vec->a != NULL);
		CVEC_FREE(vec);
		return NULL;
	}

	if (elem_init) {
		for (i=0; i<num; ++i) {
			if (!elem_init(&vec->a[i*elem_sz], &((cvec_u8*)vals)[i*elem_sz])) {
				CVEC_ASSERT(0);
				CVEC_FREE(vec->a);
				CVEC_FREE(vec);
				return NULL;
			}
		}
	} else {
		CVEC_MEMMOVE(vec->a, vals, elem_sz*num);
	}
	
	vec->elem_free = elem_free;
	vec->elem_init = elem_init;

	return vec;
}

/** Same as cvec_void_heap() except the vector passed in was declared on the stack so
 *  it isn't allocated in this function.  Use the cvec_free_void in that case
 */
int cvec_void(cvector_void* vec, cvec_sz size, cvec_sz capacity, cvec_sz elem_sz, void(*elem_free)(void*), int(*elem_init)(void*, void*))
{
	vec->size = size;
	vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + CVEC_VOID_START_SZ;

	vec->elem_size = elem_sz;
	
	if (!(vec->a = (cvec_u8*)CVEC_MALLOC(vec->capacity*elem_sz))) {
		CVEC_ASSERT(vec->a != NULL);
		vec->size = vec->capacity = 0;
		return 0;
	}

	vec->elem_free = elem_free;
	vec->elem_init = elem_init;

	return 1;
}

/** Same as init_vec_heap() except the vector passed in was declared on the stack so
 *  it isn't allocated in this function.  Use the cvec_free_void in this case
 */
int cvec_init_void(cvector_void* vec, void* vals, cvec_sz num, cvec_sz elem_sz, void(*elem_free)(void*), int(*elem_init)(void*, void*))
{
	cvec_sz i;
	
	vec->elem_size = elem_sz;

	vec->capacity = num + CVEC_VOID_START_SZ;
	vec->size = num;
	if (!(vec->a = (cvec_u8*)CVEC_MALLOC(vec->capacity*elem_sz))) {
		CVEC_ASSERT(vec->a != NULL);
		vec->size = vec->capacity = 0;
		return 0;
	}

	if (elem_init) {
		for (i=0; i<num; ++i) {
			if (!elem_init(&vec->a[i*elem_sz], &((cvec_u8*)vals)[i*elem_sz])) {
				CVEC_ASSERT(0);
				return 0;
			}
		}
	} else {
		CVEC_MEMMOVE(vec->a, vals, elem_sz*num);
	}

	vec->elem_free = elem_free;
	vec->elem_init = elem_init;

	return 1;
}

/** Makes dest a copy of src.  The parameters
 *  are void so it can be used as the constructor when making
 *  a vector of cvector_void's.  Assumes dest (the structure)
 *  is already allocated (probably on the stack) and that
 *  capacity is 0 (ie the array doesn't need to be freed).
 *
 *  Really just a wrapper around copy, that initializes dest/vec1's
 *  members to NULL/0.  If you pre-initialized dest to 0, you could
 *  just use copy.
 */
int cvec_copyc_void(void* dest, void* src)
{
	cvector_void* vec1 = (cvector_void*)dest;
	cvector_void* vec2 = (cvector_void*)src;

	vec1->a = NULL;
	vec1->size = 0;
	vec1->capacity = 0;

	return cvec_copy_void(vec1, vec2);
}

/** Makes dest a copy of src.  Assumes dest
 * (the structure) is already allocated (probably on the stack) and
 * is in a valid state (ie array is either NULL or allocated with
 * size and capacity set appropriately).
 *
 * TODO Should I copy capacity, so dest is truly identical or do
 * I only care about the actual contents, and let dest->cap = src->size
 * maybe plus CVEC_VOID_START_SZ
 */
int cvec_copy_void(cvector_void* dest, cvector_void* src)
{
	int i;
	cvec_u8* tmp = NULL;
	if (!(tmp = (cvec_u8*)CVEC_REALLOC(dest->a, src->capacity*src->elem_size))) {
		CVEC_ASSERT(tmp != NULL);
		return 0;
	}
	dest->a = tmp;

	if (src->elem_init) {
		for (i=0; i<src->size; ++i) {
			if (!src->elem_init(&dest->a[i*src->elem_size], &src->a[i*src->elem_size])) {
				CVEC_ASSERT(0);
				return 0;
			}
		}
	} else {
		/* could use memcpy here since we know we just allocated dest->a */
		CVEC_MEMMOVE(dest->a, src->a, src->size*src->elem_size);
	}

	dest->size = src->size;
	dest->capacity = src->capacity;
	dest->elem_size = src->elem_size;
	dest->elem_free = src->elem_free;
	dest->elem_init = src->elem_init;
	return 1;
}

/** Append a to end of vector (size increased 1).
 * Capacity is increased by doubling when necessary.
 *
 * TODO For all of cvector_void, now that elem_init returns int, is it worth
 * the extra code and overhead of checking it and asserting/returning 0?
 */
int cvec_push_void(cvector_void* vec, void* a)
{
	cvec_u8* tmp;
	cvec_sz tmp_sz;
	if (vec->capacity == vec->size) {
		tmp_sz = CVEC_VOID_ALLOCATOR(vec->capacity);
		if (!(tmp = (cvec_u8*)CVEC_REALLOC(vec->a, vec->elem_size*tmp_sz))) {
			CVEC_ASSERT(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}
	if (vec->elem_init) {
		if (!vec->elem_init(&vec->a[vec->size*vec->elem_size], a)) {
			CVEC_ASSERT(0);
			return 0;
		}
	} else {
		CVEC_MEMMOVE(&vec->a[vec->size*vec->elem_size], a, vec->elem_size);
	}
	
	vec->size++;
	return 1;
}

/** Same as push except no elem_init even if it's set */
int cvec_pushm_void(cvector_void* vec, void* a)
{
	cvec_u8* tmp;
	cvec_sz tmp_sz;
	if (vec->capacity == vec->size) {
		tmp_sz = CVEC_VOID_ALLOCATOR(vec->capacity);
		if (!(tmp = (cvec_u8*)CVEC_REALLOC(vec->a, vec->elem_size*tmp_sz))) {
			CVEC_ASSERT(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}
	CVEC_MEMMOVE(&vec->a[vec->size*vec->elem_size], a, vec->elem_size);
	
	vec->size++;
	return 1;
}


/** Remove the last element (size decreased 1).
 * Copy the element into ret if ret is not NULL.  This function assumes
 * that ret is large accept the element and just CVEC_MEMMOVE's it in.
 * Similar to pop_backs it is users responsibility.
 */
void cvec_pop_void(cvector_void* vec, void* ret)
{
	vec->size--;
	if (ret) {
		CVEC_MEMMOVE(ret, &vec->a[vec->size*vec->elem_size], vec->elem_size);
	}
	if (vec->elem_free) {
		vec->elem_free(&vec->a[vec->size*vec->elem_size]);
	}
}

/** Same as pop except no elem_free even if it's set. */
void cvec_popm_void(cvector_void* vec, void* ret)
{
	vec->size--;
	if (ret) {
		CVEC_MEMMOVE(ret, &vec->a[vec->size*vec->elem_size], vec->elem_size);
	}
}

/** Return pointer to last element */
void* cvec_back_void(cvector_void* vec)
{
	return &vec->a[(vec->size-1)*vec->elem_size];
}

/** Increase the size of the array num items.  Items
 *  are not initialized to anything! */
int cvec_extend_void(cvector_void* vec, cvec_sz num)
{
	cvec_u8* tmp;
	cvec_sz tmp_sz;
	if (vec->capacity < vec->size + num) {
		tmp_sz = vec->capacity + num + CVEC_VOID_START_SZ;
		if (!(tmp = (cvec_u8*)CVEC_REALLOC(vec->a, vec->elem_size*tmp_sz))) {
			CVEC_ASSERT(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}

	vec->size += num;
	return 1;
}

/** Return a void pointer to the ith element.
  * Another way to get elements from vector that is used in vector_tests.c
  * is a macro like this one
  * #define GET_ELEMENT(VEC,I,TYPE) ((TYPE*)&VEC.a[(I)*VEC.elem_size])
*/
void* cvec_get_void(cvector_void* vec, cvec_sz i)
{
	return &vec->a[i*vec->elem_size];
}

/**
 * Insert a at index i (0 based).
 * Everything from that index and right is shifted one to the right.
 */
int cvec_insert_void(cvector_void* vec, cvec_sz i, void* a)
{
	cvec_u8* tmp;
	cvec_sz tmp_sz;
	if (vec->capacity == vec->size) {
		tmp_sz = CVEC_VOID_ALLOCATOR(vec->capacity);
		if (!(tmp = (cvec_u8*)CVEC_REALLOC(vec->a, vec->elem_size*tmp_sz))) {
			CVEC_ASSERT(tmp != NULL);
			return 0;
		}
		
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}
	CVEC_MEMMOVE(&vec->a[(i+1)*vec->elem_size], &vec->a[i*vec->elem_size], (vec->size-i)*vec->elem_size);

	if (vec->elem_init) {
		if (!vec->elem_init(&vec->a[i*vec->elem_size], a)) {
			CVEC_ASSERT(0);
			return 0;
		}
	} else {
		CVEC_MEMMOVE(&vec->a[i*vec->elem_size], a, vec->elem_size);
	}

	vec->size++;
	return 1;
}

/** Same as insert but no elem_init even if defined. */
int cvec_insertm_void(cvector_void* vec, cvec_sz i, void* a)
{
	cvec_u8* tmp;
	cvec_sz tmp_sz;
	if (vec->capacity == vec->size) {
		tmp_sz = CVEC_VOID_ALLOCATOR(vec->capacity);
		if (!(tmp = (cvec_u8*)CVEC_REALLOC(vec->a, vec->elem_size*tmp_sz))) {
			CVEC_ASSERT(tmp != NULL);
			return 0;
		}
		
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}
	CVEC_MEMMOVE(&vec->a[(i+1)*vec->elem_size], &vec->a[i*vec->elem_size], (vec->size-i)*vec->elem_size);

	CVEC_MEMMOVE(&vec->a[i*vec->elem_size], a, vec->elem_size);

	vec->size++;
	return 1;
}

/**
 * Insert the first num elements of array a at index i.
 * Note that it is the user's responsibility to pass in val_id
 * arguments.  Also CVEC_MEMMOVE is used (when there is no elem_init function)
 * so don't try to insert part of the vector array into itself
 * (that would require CVEC_MEMMOVE)
 */
int cvec_insert_array_void(cvector_void* vec, cvec_sz i, void* a, cvec_sz num)
{
	cvec_u8* tmp;
	cvec_sz tmp_sz, j;
	if (vec->capacity < vec->size + num) {
		tmp_sz = vec->capacity + num + CVEC_VOID_START_SZ;
		if (!(tmp = (cvec_u8*)CVEC_REALLOC(vec->a, vec->elem_size*tmp_sz))) {
			CVEC_ASSERT(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}

	CVEC_MEMMOVE(&vec->a[(i+num)*vec->elem_size], &vec->a[i*vec->elem_size], (vec->size-i)*vec->elem_size);
	if (vec->elem_init) {
		for (j=0; j<num; ++j) {
			if (!vec->elem_init(&vec->a[(j+i)*vec->elem_size], &((cvec_u8*)a)[j*vec->elem_size])) {
				CVEC_ASSERT(0);
				return 0;
			}
		}
	} else {
		CVEC_MEMMOVE(&vec->a[i*vec->elem_size], a, num*vec->elem_size);
	}
	vec->size += num;
	return 1;
}

/** Same as insert_array but no elem_init even if defined. */
int cvec_insert_arraym_void(cvector_void* vec, cvec_sz i, void* a, cvec_sz num)
{
	cvec_u8* tmp;
	cvec_sz tmp_sz;
	if (vec->capacity < vec->size + num) {
		tmp_sz = vec->capacity + num + CVEC_VOID_START_SZ;
		if (!(tmp = (cvec_u8*)CVEC_REALLOC(vec->a, vec->elem_size*tmp_sz))) {
			CVEC_ASSERT(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}

	CVEC_MEMMOVE(&vec->a[(i+num)*vec->elem_size], &vec->a[i*vec->elem_size], (vec->size-i)*vec->elem_size);

	CVEC_MEMMOVE(&vec->a[i*vec->elem_size], a, num*vec->elem_size);
	vec->size += num;
	return 1;
}

/**
 * Replace value at i with a, return old value in ret if non-NULL.
 */
int cvec_replace_void(cvector_void* vec, cvec_sz i, void* a, void* ret)
{
	if (ret) {
		CVEC_MEMMOVE(ret, &vec->a[i*vec->elem_size], vec->elem_size);
	} else if (vec->elem_free) {
		vec->elem_free(&vec->a[i*vec->elem_size]);
	}

	if (vec->elem_init) {
		if (!vec->elem_init(&vec->a[i*vec->elem_size], a)) {
			CVEC_ASSERT(0);
			return 0;
		}
	} else {
		CVEC_MEMMOVE(&vec->a[i*vec->elem_size], a, vec->elem_size);
	}
	return 1;
}

/**
 * Same as replace but no elem_free or elem_init even if they're defined.
 * Because it doesn't call elem_init, there's no chance of failure so there's
 * no return value.
 */
void cvec_replacem_void(cvector_void* vec, cvec_sz i, void* a, void* ret)
{
	if (ret) {
		CVEC_MEMMOVE(ret, &vec->a[i*vec->elem_size], vec->elem_size);
	}

	CVEC_MEMMOVE(&vec->a[i*vec->elem_size], a, vec->elem_size);
}

/**
 * Erases elements from start to end inclusive.
 * Example cvec_erase_void(myvec, 1, 3) would call elem_free (if an elem_free function was provided)
 * and remove elements at 1, 2, and 3 and the element that was at index 4 would now be at 1 etc.
 */
void cvec_erase_void(cvector_void* vec, cvec_sz start, cvec_sz end)
{
	cvec_sz i;
	cvec_sz d = end - start + 1;
	if (vec->elem_free) {
		for (i=start; i<=end; i++) {
			vec->elem_free(&vec->a[i*vec->elem_size]);
		}
	}
	CVEC_MEMMOVE(&vec->a[start*vec->elem_size], &vec->a[(end+1)*vec->elem_size], (vec->size-1-end)*vec->elem_size);
	vec->size -= d;
}

/** Same as erase except it *does not* call elem_free */
void cvec_remove_void(cvector_void* vec, cvec_sz start, cvec_sz end)
{
	cvec_sz d = end - start + 1;
	CVEC_MEMMOVE(&vec->a[start*vec->elem_size], &vec->a[(end+1)*vec->elem_size], (vec->size-1-end)*vec->elem_size);
	vec->size -= d;
}

/** Makes sure capacity >= size (the parameter not the member). */
int cvec_reserve_void(cvector_void* vec, cvec_sz size)
{
	cvec_u8* tmp;
	if (vec->capacity < size) {
		if (!(tmp = (cvec_u8*)CVEC_REALLOC(vec->a, vec->elem_size*(size+CVEC_VOID_START_SZ)))) {
			CVEC_ASSERT(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = size + CVEC_VOID_START_SZ;
	}
	return 1;
}

/** Set capacity to size.
 * You will lose data if you shrink the capacity below the current size.
 * If you do, the size will be set to capacity of course.
*/
int cvec_set_cap_void(cvector_void* vec, cvec_sz size)
{
	cvec_sz i;
	cvec_u8* tmp;
	if (size < vec->size) {
		if (vec->elem_free) {
			for (i=vec->size-1; i>=size; i--) {
				vec->elem_free(&vec->a[i*vec->elem_size]);
			}
		}
		vec->size = size;
	}

	vec->capacity = size;

	if (!(tmp = (cvec_u8*)CVEC_REALLOC(vec->a, vec->elem_size*size))) {
		CVEC_ASSERT(tmp != NULL);
		return 0;
	}
	vec-> a = tmp;
	return 1;
}

/** Set all size elements to val. */
int cvec_set_val_sz_void(cvector_void* vec, void* val)
{
	cvec_sz i;

	if (vec->elem_free) {
		for(i=0; i<vec->size; i++) {
			vec->elem_free(&vec->a[i*vec->elem_size]);
		}
	}
	
	if (vec->elem_init) {
		for (i=0; i<vec->size; i++) {
			if (!vec->elem_init(&vec->a[i*vec->elem_size], val)) {
				CVEC_ASSERT(0);
				return 0;
			}
		}
	} else {
		for (i=0; i<vec->size; i++) {
			CVEC_MEMMOVE(&vec->a[i*vec->elem_size], val, vec->elem_size);
		}
	}
	return 1;
}

/** Fills entire allocated array (capacity) with val.  If you set an elem_free function
 * then size is set to capacity like cvector_str for the same reason, ie I need to know
 * that the elem_free function needs to be called on those elements.
 * TODO Remove this function?  Same reason as set_val_cap_str.
 */
int cvec_set_val_cap_void(cvector_void* vec, void* val)
{
	cvec_sz i;
	if (vec->elem_free) {
		for (i=0; i<vec->size; i++) {
			vec->elem_free(&vec->a[i*vec->elem_size]);
		}
		vec->size = vec->capacity;
	}

	if (vec->elem_init) {
		for (i=0; i<vec->capacity; i++) {
			if (!vec->elem_init(&vec->a[i*vec->elem_size], val)) {
				CVEC_ASSERT(0);
				return 0;
			}
		}
	} else {
		for (i=0; i<vec->capacity; i++) {
			CVEC_MEMMOVE(&vec->a[i*vec->elem_size], val, vec->elem_size);
		}
	}
	return 1;
}

/** Sets size to 0 (does not change contents unless elem_free is set
 *  then it will elem_free all size elements as in cvector_str). */
void cvec_clear_void(cvector_void* vec)
{
	cvec_sz i;
	if (vec->elem_free) {
		for (i=0; i<vec->size; ++i) {
			vec->elem_free(&vec->a[i*vec->elem_size]);
		}
	}
	vec->size = 0;
}

/** Frees everything so don't use vec after calling this. If you set an elem_free function
 * it will be called on all size elements of course. Passing NULL is a NO-OP, matching the behavior
 * of free(). */
void cvec_free_void_heap(void* vec)
{
	cvec_sz i;
	cvector_void* tmp = (cvector_void*)vec;
	if (!tmp) return;
	if (tmp->elem_free) {
		for (i=0; i<tmp->size; i++) {
			tmp->elem_free(&tmp->a[i*tmp->elem_size]);
		}
	}
	CVEC_FREE(tmp->a);
	CVEC_FREE(tmp);
}

/** Frees the internal array and sets size and capacity to 0 */
void cvec_free_void(void* vec)
{
	cvec_sz i;
	cvector_void* tmp = (cvector_void*)vec;
	if (tmp->elem_free) {
		for (i=0; i<tmp->size; i++) {
			tmp->elem_free(&tmp->a[i*tmp->elem_size]);
		}
	}

	CVEC_FREE(tmp->a);

	tmp->size = 0;
	tmp->capacity = 0;
}

/*! \mainpage CVector notes
 *

\section Intro
This is a relatively simple ANSI compliant C vector library with specific structures and
functions for int's, double's and string's and support for all other types
using either a generic structure where the type is passed in as void* and stored in a cvec_u8 array
(to avoid dereferencing void* warnings and frequent casting) or generated type-specific
vectors using a macro or template system (see below).

The generic vector is very flexible and allows you to provide free and init functions
if you like that it will call at appropriate times similar to the way C++ containers
will call destructors/constructors.

Other modifiable parameters are at the top of the respective cvector.c's
<pre>
cvec_sz CVEC_I_START_SZ = 50;
cvec_sz CVEC_D_START_SZ = 50;
cvec_sz CVEC_STR_START_SZ = 20;
cvec_sz CVEC_VOID_START_SZ = 20;

#define CVEC_I_ALLOCATOR(x) ((x+1) * 2)
#define CVEC_D_ALLOCATOR(x) ((x+1) * 2)
#define CVEC_STR_ALLOCATOR(x) ((x+1) * 2)
#define CVEC_VOID_ALLOCATOR(x) ((x+1) * 2)
</pre>
The allocator macros are used in all functions that increase the size by 1.
In others (constructors, insert_array, reserve) CVEC_X_START_SZ is the amount
extra allocated.

Note that the (x+1) portion allows you to use the non-void vectors
without calling any of the init functions first *if* you zero them out.  This
means size, capacity, and a are 0/NULL which is valid because realloc acts like
malloc when given a NULL pointer.  With cvector_void you still have to set
elem_size, and optionally elem_free/elem_init. See the zero_init_x_test()'s
in cvector_tests.c for example of that use.

The `cvec_sz` type defaults to size_t but if you define CVEC_SIZE_T before including
the header which is then `typedef`'d to `cvec_sz`.  It has to be defined before
every header inclusion since it is used in both the header (struct definiton)
and the implementation.  Note, if you use a signed type, passing a negative value
is undefined behavior (ie it'll likely crash immediately).  Of course if you
passed a negative while using the default `size_t` you'd probably crash anyway
as it would wrap around to 2^64.

There are also 2 templates, one for basic types and one for types that contain
dynamically allocated memory and you might want a free and/or init function.
In other words the first template is based off cvector_i and the second is based
off of cvector_void, so look at the corresponding documentation for behavior.

There are 2 ways to use/create your own cvector types.  The easiest way is to use
the macros defined in cvector_macro.h which are also included in the all-in-one header
cvector.h.  You can see how to use them in cvector_tests.c:

	#define RESIZE(a) (((a)+1)*2)

	CVEC_NEW_DECLS(short)
	CVEC_NEW_DECLS2(f_struct)

	CVEC_NEW_DEFS(short, RESIZE)
	CVEC_NEW_DEFS2(f_struct, RESIZE)

The RESIZE macro has to be defined before using the macros for now, serving the
same purpose as the regular allocator macros above.  Obviously the DECL macros
declare type and prototypes while the DEFS define them.  Using the macros for
user made types is much easier than the files because you can call the macro
right in the header where you define the type instead of having to include the
type in the generated file.  Basically 1 step rather than 2-3 and no extra files
needed.

The other way, and the only way in previous versions of CVector, is to generate
your own files from the template files which are located in cvector_template.h
and cvector_template2.h.

To generate your own cvector files for a type just run:

	python3 generate_code.py yourtype

which will generate the results for both templates so just delete the one
you don't want.

cvector_short.h and cvector_f_struct.h are examples of the generated files.

\section des_notes Design Notes
With the exception of CVEC_STRDUP calls in cvector_str, memory allocations are checked and asserted.
I decided that the likelihood of individual string allocations failing is low enough that it wasn't
worth the slowdown or extra code.  However, the equivalent calls to elem_init in vector_void and
generated vector_TYPEs are checked/asserted (since they're more likely to be large enough to possibly
fail).  If not in debug mode (ie NDEBUG is defined) 0 is returned on allocation failure.

For functions that take a ret parameter (ie pop and replace functions for str, void, and type 2
template/macro generated vectors), NULL is a valid option if you don't care to get the value.
I didn't want to force users to make a temporary variable just to catch something they weren't
going to use.

No other error checking is performed.  If you pass bad parameters (ie NULL for the vector
pointer, or end < start for the range functions), bad things will happen.
This is consistent with my belief that it is the caller's responsibility to pass valid arguments
and library code shouldn't be larger/uglier/slower for everyone just to pretty print errors.  This
is also consistent with the C standard library where, for the most part, passing invalid parameters
results in undefined behavior (see section 4.1.6 in C89, 7.1.4 in C99 and C11).

The back functions simply return the address of size - 1.  This is fine even if your size is zero
for the use of <= back_i(myvec) since the beginning of the array will already be > back.  If I were
to return NULL in the case of size 0, you'd just exchange a possible size check before the call for
a possible NULL check after the call.  I choose this way because it doesn't add an if check
to the function so it's smaller/faster, I think the <= use case is more likely, and it's easier
and more normal to know when your vector is empty than to remember to check for NULL after the fact.

The insert functions (insert_i and insert_array_i for example) do allow you to insert at the end.
The CVEC_MEMMOVE inside the functions will simply move 0 bytes if you pass the current size as the index.
C99 and C11 guarrantee this behavior in the standard (and thus C++ does as well).  Though I wrote
this library to be compliant with C89, which does not guarrantee this behavior, I think
it's safe to assume they'd use the same implementation since it doesn't contradict C89 and it
just makes sense.

\section Building
I use premake generated makefiles which are
included in the build subdirectory.  However, if you modified premake4.lua
the command to regenerate them is `premake4 gmake`.  cd into the build
directory and run `make` or `make config=release`. I have not tried it on
windows though it should work (well I'm not sure about CUnit ...).

There is no output of any kind, no errors or warnings.

It has been relatively well tested using CUnit tests which all pass.
I've also run it under valgrind and there are no memory leaks.

<pre>
$ valgrind --leak-check=full -v ./cvector
==116175==
==116175== HEAP SUMMARY:
==116175==     in use at exit: 0 bytes in 0 blocks
==116175==   total heap usage: 10,612 allocs, 10,612 frees, 1,151,748 bytes allocated
==116175==
==116175== All heap blocks were freed -- no leaks are possible
==116175==
==116175== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
$ grep FAIL CUnitAutomated-Results.xml
<FAILED> 0 </FAILED> 
<FAILED> 0 </FAILED> 
<FAILED> 0 </FAILED> 
</pre>

You can probably get Cunit from your package manager but
if you want to get the most up to date version of CUnit go here:

http://cunit.sourceforge.net/index.html
http://sourceforge.net/projects/cunit/

I'm using version 2.1-3.

\section Usage
To actually use the library just copy the appropriate c/h file pair(s) to your project
or just use cvector.h.  To get a good idea of how to use the library and see it in
action and how it should behave, look at cvector_tests.c

\section LICENSE
CVector is licensed under the MIT License.

Copyright (c) 2011-2023 Robert Winkler

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
documentation files (the "Software"), to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and
to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.
 *
 *
 */


