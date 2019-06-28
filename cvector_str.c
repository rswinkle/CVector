#include "cvector_str.h"





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

size_t CVEC_STR_START_SZ = 20;

#define CVEC_STR_ALLOCATOR(x) ((x+1) * 2)

/** Useful utility function since strdup isn't in standard C.*/
char* mystrdup(const char* str)
{
	size_t len = strlen(str);
	char* temp = (char*)CVEC_MALLOC(len+1);
	if (!temp) {
		CVEC_ASSERT(temp != NULL);
		return NULL;
	}
	temp[len] = 0;
	
	return (char*)CVEC_MEMMOVE(temp, str, len);  /* CVEC_MEMMOVE returns to */
}

/**
 * Create a new cvector_str on the heap.
 * Vector size set to (size > 0) ? size : 0;
 * Capacity to (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + CVEC_STR_START_SZ
 * in other words capacity has to be at least 1 and >= to vec->size of course.
 * Note: cvector_str does not copy pointers passed in but duplicates the strings
 * they point to (using mystrdup()) so you don't have to worry about freeing
 * or changing the contents of variables that you've pushed or inserted; it
 * won't affect the values vector.
 */
cvector_str* cvec_str_heap(size_t size, size_t capacity)
{
	cvector_str* vec;
	if (!(vec = (cvector_str*)CVEC_MALLOC(sizeof(cvector_str)))) {
		CVEC_ASSERT(vec != NULL);
		return NULL;
	}

	vec->size = size;
	vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + CVEC_STR_START_SZ;

	if (!(vec->a = (char**)CVEC_MALLOC(vec->capacity * sizeof(char*)))) {
		CVEC_ASSERT(vec->a != NULL);
		CVEC_FREE(vec);
		return NULL;
	}
	/* clearing to 0 here because if the user gave a non-zero initial size, popping/
	 * erasing will crash unless they're NULL.  Really the user should never do that.
	 * They should use cvec_init or otherwise immediately assign to the size elements they
	 * started with.  */
	memset(vec->a, 0, vec->capacity*sizeof(char*));

	return vec;
}

/** Create (on the heap) and initialize cvector_str with num elements of vals.
 */
cvector_str* cvec_init_str_heap(char** vals, size_t num)
{
	cvector_str* vec;
	size_t i;
	
	if (!(vec = (cvector_str*)CVEC_MALLOC(sizeof(cvector_str)))) {
		CVEC_ASSERT(vec != NULL);
		return NULL;
	}

	vec->capacity = num + CVEC_STR_START_SZ;
	vec->size = num;
	if (!(vec->a = (char**)CVEC_MALLOC(vec->capacity*sizeof(char*)))) {
		CVEC_ASSERT(vec->a != NULL);
		CVEC_FREE(vec);
		return NULL;
	}

	for(i=0; i<num; i++) {
		vec->a[i] = mystrdup(vals[i]);
	}
	
	return vec;
}

/** Same as cvec_str_heap() except the vector passed in was declared on the stack so
 *  it isn't allocated in this function.  Use the cvec_free_str in this case
 *  This and cvec_init_str should be preferred over the heap versions.
 */
int cvec_str(cvector_str* vec, size_t size, size_t capacity)
{
	vec->size = size;
	vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + CVEC_STR_START_SZ;

	if (!(vec->a = (char**)CVEC_MALLOC(vec->capacity * sizeof(char*)))) {
		CVEC_ASSERT(vec->a != NULL);
		vec->size = vec->capacity = 0;
		return 0;
	}
	/* clearing to 0 here because if the user gave a non-zero initial size, popping/
	 * erasing will crash unless they're NULL */
	memset(vec->a, 0, vec->capacity*sizeof(char*));

	return 1;
}

/** Same as cvec_init_str() except the vector passed in was declared on the stack so
 *  it isn't allocated in this function.  Use the cvec_free_str in this case
 */
int cvec_init_str(cvector_str* vec, char** vals, size_t num)
{
	size_t i;
	
	vec->capacity = num + CVEC_STR_START_SZ;
	vec->size = num;
	if (!(vec->a = (char**)CVEC_MALLOC(vec->capacity*sizeof(char*)))) {
		CVEC_ASSERT(vec->a != NULL);
		vec->size = vec->capacity = 0;
		return 0;
	}

	for(i=0; i<num; i++) {
		vec->a[i] = mystrdup(vals[i]);
	}
	
	return 1;
}


/** Makes dest an identical copy of src.  The parameters
 *  are void so it can be used as the constructor when making
 *  a vector of cvector_str's.  Assumes dest (the structure)
 *  is already allocated (probably on the stack) and that
 *  capacity is 0 (ie the array doesn't need to be freed).
 */
void cvec_str_copy(void* dest, void* src)
{
	size_t i;
	cvector_str* vec1 = (cvector_str*)dest;
	cvector_str* vec2 = (cvector_str*)src;
	
	vec1->size = 0;
	vec1->capacity = 0;
	
	/*not much else we can do here*/
	if (!(vec1->a = (char**)CVEC_MALLOC(vec2->capacity*sizeof(char*)))) {
		CVEC_ASSERT(vec1->a != NULL);
		return;
	}
	
	for (i=0; i<vec2->size; ++i) {
		vec1->a[i] = mystrdup(vec2->a[i]);
	}
	
	vec1->size = vec2->size;
	vec1->capacity = vec2->capacity;
}

/**
 * Append a to end of vector (size increased 1).
 * Capacity is increased by doubling when necessary.
 */
int cvec_push_str(cvector_str* vec, char* a)
{
	char** tmp;
	size_t tmp_sz;
	if (vec->capacity == vec->size) {
		tmp_sz = CVEC_STR_ALLOCATOR(vec->capacity);
		if (!(tmp = (char**)CVEC_REALLOC(vec->a, sizeof(char*)*tmp_sz))) {
			CVEC_ASSERT(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}
	
	vec->a[vec->size++] = mystrdup(a);
	return 1;
}

/** Remove the last element (size decreased 1).
 *  String is freed.  If ret != NULL strcpy the last element into ret.
 *  It is the user's responsibility to make sure ret can receive it without error
 *  (ie ret has adequate space.) */
void cvec_pop_str(cvector_str* vec, char* ret)
{
	vec->size--;
	if (ret)
		strcpy(ret, vec->a[vec->size]);
	CVEC_FREE(vec->a[vec->size]);
}

/** Return pointer to last element */
char** cvec_back_str(cvector_str* vec)
{
	return &vec->a[vec->size-1];
}

/** Increase the size of the array num items.  Items
 *  are memset to NULL since they will be freed when
    popped or the vector is freed.*/
int cvec_extend_str(cvector_str* vec, size_t num)
{
	char** tmp;
	size_t tmp_sz;
	if (vec->capacity < vec->size + num) {
		tmp_sz = vec->capacity + num + CVEC_STR_START_SZ;
		if (!(tmp = (char**)CVEC_REALLOC(vec->a, sizeof(char*)*tmp_sz))) {
			CVEC_ASSERT(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}

	memset(&vec->a[vec->size], 0, num*sizeof(char*));
	vec->size += num;
	return 1;
}

/**
 * Insert a at index i (0 based).
 * Everything from that index and right is shifted one to the right.
 */
int cvec_insert_str(cvector_str* vec, size_t i, char* a)
{
	char** tmp;
	size_t tmp_sz;
	if (vec->capacity == vec->size) {
		tmp_sz = CVEC_STR_ALLOCATOR(vec->capacity);
		if (!(tmp = (char**)CVEC_REALLOC(vec->a, sizeof(char*)*tmp_sz))) {
			CVEC_ASSERT(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}

	CVEC_MEMMOVE(&vec->a[i+1], &vec->a[i], (vec->size-i)*sizeof(char*));
	vec->a[i] = mystrdup(a);
	vec->size++;
	return 1;
}

/**
 * Insert the first num elements of array a at index i.
 * Note that it is the user's responsibility to pass in valid
 * arguments.
 */
int cvec_insert_array_str(cvector_str* vec, size_t i, char** a, size_t num)
{
	char** tmp;
	size_t tmp_sz, j;
	if (vec->capacity < vec->size + num) {
		tmp_sz = vec->capacity + num + CVEC_STR_START_SZ;
		if (!(tmp = (char**)CVEC_REALLOC(vec->a, sizeof(char*)*tmp_sz))) {
			CVEC_ASSERT(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}

	CVEC_MEMMOVE(&vec->a[i+num], &vec->a[i], (vec->size-i)*sizeof(char*));
	for (j=0; j<num; ++j) {
		vec->a[j+i] = mystrdup(a[j]);
	}
	
	vec->size += num;
	return 1;
}

/**
 * Replace string at i with a. If ret != NULL, strcpy the old str to it.
 * See cvec_pop_str warning
 * */
void cvec_replace_str(cvector_str* vec, size_t i, char* a, char* ret)
{
	if (ret)
		strcpy(ret, vec->a[i]);
	CVEC_FREE(vec->a[i]);
	vec->a[i] = mystrdup(a);
}

/**
 * Erases strings from start to end inclusive.
 * Example erases(myvec, 1, 3) would CVEC_FREE and remove strings at 1, 2, and 3 and the string
 * that was at index 4 would now be at 1 etc.
 */
void cvec_erase_str(cvector_str* vec, size_t start, size_t end)
{
	size_t i;
	size_t d = end - start + 1;
	for (i=start; i<=end; i++) {
		CVEC_FREE(vec->a[i]);
	}
	
	CVEC_MEMMOVE(&vec->a[start], &vec->a[end+1], (vec->size-1-end)*sizeof(char*));
	vec->size -= d;
}

/** Makes sure the vector capacity is >= size (parameter not member). */
int cvec_reserve_str(cvector_str* vec, size_t size)
{
	char** tmp;
	if (vec->capacity < size) {
		if (!(tmp = (char**)CVEC_REALLOC(vec->a, sizeof(char*)*(size+CVEC_STR_START_SZ)))) {
			CVEC_ASSERT(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = size + CVEC_STR_START_SZ;
	}
	return 1;
}

/** Set capacity to size.
 * You will lose data if you shrink the capacity below the current size.
 * If you do, the size will be set to capacity of course.
*/
int cvec_set_cap_str(cvector_str* vec, size_t size)
{
	size_t i;
	char** tmp;
	if (size < vec->size) {
		for(i=vec->size-1; i>size-1; i--) {
			CVEC_FREE(vec->a[i]);
		}

		vec->size = size;
	}

	if (!(tmp = (char**)CVEC_REALLOC(vec->a, sizeof(char*)*size))) {
		CVEC_ASSERT(tmp != NULL);
		return 0;
	}
	vec->a = tmp;
	vec->capacity = size;
	return 1;
}

/** Sets all size elements to val. */
void cvec_set_val_sz_str(cvector_str* vec, char* val)
{
	size_t i;
	for(i=0; i<vec->size; i++) {
		CVEC_FREE(vec->a[i]);

		/* not worth checking/(char**)reallocing to me */
		vec->a[i] = mystrdup(val);
	}
}

/** Fills entire allocated array (capacity) with val.  Size is set
 * to capacity in this case because strings are individually dynamically allocated.
 * This is different from cvector_i, cvector_d and cvector_void (without a CVEC_FREE function) where the size stays the same.
   TODO  Remove this function?  even more unnecessary than for cvector_i and cvector_d and different behavior*/
void cvec_set_val_cap_str(cvector_str* vec, char* val)
{
	size_t i;
	for (i=0; i<vec->capacity; i++) {
		if (i<vec->size) {
			CVEC_FREE(vec->a[i]);
		}
		
		vec->a[i] = mystrdup(val);
	}
	vec->size = vec->capacity;
}

/** Clears the contents of vector (frees all strings) and sets size to 0. */
void cvec_clear_str(cvector_str* vec)
{
	int i;
	for (i=0; i<vec->size; i++) {
		CVEC_FREE(vec->a[i]);
	}
	
	vec->size = 0;
}

/** Frees contents (individual strings and array) and frees vector so don't use after calling this. */
void cvec_free_str_heap(void* vec)
{
	size_t i;
	cvector_str* tmp = (cvector_str*)vec;
	for (i=0; i<tmp->size; i++) {
		CVEC_FREE(tmp->a[i]);
	}
	
	CVEC_FREE(tmp->a);
	CVEC_FREE(tmp);
}

/** Frees the internal array and sets size and capacity to 0 */
void cvec_free_str(void* vec)
{
	size_t i;
	cvector_str* tmp = (cvector_str*)vec;
	for (i=0; i<tmp->size; i++) {
		CVEC_FREE(tmp->a[i]);
	}
	
	CVEC_FREE(tmp->a);
	tmp->size = 0;
	tmp->capacity = 0;
}
