#include "vector_str.h"

#include <assert.h>

#define STDERR(X) fprintf(stderr, X)



size_t VEC_STR_START_SZ = 20;

#define VEC_STR_ALLOCATOR(x) ((x) * 2)

/** Useful utility function since strdup isn't in standard C.*/
char* mystrdup(const char* str)
{
	/* if (!str)
	 * 	return NULL;
	 */
	size_t len = strlen(str);
	char* temp = (char*)calloc(len+1, sizeof(char));
	if (!temp) {
		assert(temp != NULL);
		return NULL;
	}
	
	return (char*)memcpy(temp, str, len);  /* memcpy returns to, and (char**)calloc already nulled last char */
}



/**
 * Create a new vector_str on the heap.
 * Vector size set to (size > 0) ? size : 0;
 * Capacity to (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + VEC_STR_START_SZ
 * in other words capacity has to be at least 1 and >= to vec->size of course.
 * Note: vector_str does not copy pointers passed in but duplicates the strings
 * they point to (using mystrdup()) so you don't have to worry about freeing
 * or changing the contents of variables that you've pushed or inserted; it
 * won't affect the values vector.
 */
vector_str* vec_str_heap(size_t size, size_t capacity)
{
	vector_str* vec;
	if (!(vec = (vector_str*)malloc(sizeof(vector_str)))) {
		assert(vec != NULL);
		return NULL;
	}

	vec->size = size;
	vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + VEC_STR_START_SZ;

	/* calloc here because if we free before poppirg/erasing and since nothing is
	 * allocated these need to be NULL to not cause problems */
	if (!(vec->a = (char**)calloc(vec->capacity, sizeof(char*)))) {
		assert(vec->a != NULL);
		free(vec);
		return NULL;
	}

	return vec;
}


/** Create (on the heap) and initialize vector_str with num elements of vals.
 */
vector_str* init_vec_str_heap(char** vals, size_t num)
{
	vector_str* vec;
	size_t i;
	
	if (!(vec = (vector_str*)malloc(sizeof(vector_str)))) {
		assert(vec != NULL);
		return NULL;
	}

	vec->capacity = num + VEC_STR_START_SZ;
	vec->size = num;
	if (!(vec->a = (char**)malloc(vec->capacity*sizeof(char*)))) {
		assert(vec->a != NULL);
		free(vec);
		return NULL;
	}

	for(i=0; i<num; i++) {
		vec->a[i] = mystrdup(vals[i]);
	}
	
	return vec;
}


/** Same as vec_str_heap() except the vector passed in was declared on the stack so
 *  it isn't allocated in this function.  Use the free_vec_str in this case
 *  This and init_vec_str should be preferred over the heap versions.
 */
int vec_str(vector_str* vec, size_t size, size_t capacity)
{
	vec->size = size;
	vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + VEC_STR_START_SZ;

	/* (char**)calloc here because it we free before poppirg/erasing and since nothing is
	 * allocated these need to be NULL to not cause problems */
	if (!(vec->a = (char**)calloc(vec->capacity, sizeof(char*)))) {
		assert(vec->a != NULL);
		vec->size = vec->capacity = 0;
		return 0;
	}

	return 1;
}

/** Same as init_vec_str() except the vector passed in was declared on the stack so
 *  it isn't allocated in this function.  Use the free_vec_str in this case
 */
int init_vec_str(vector_str* vec, char** vals, size_t num)
{
	size_t i;
	
	vec->capacity = num + VEC_STR_START_SZ;
	vec->size = num;
	if (!(vec->a = (char**)malloc(vec->capacity*sizeof(char*)))) {
		assert(vec->a != NULL);
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
 *  a vector of vector_str's.  Assumes dest (the structure)
 *  is already allocated (probably on the stack) and that
 *  capacity is 0 (ie the array doesn't need to be freed).
 */
void vec_str_copy(void* dest, void* src)
{
	size_t i;
	vector_str* vec1 = (vector_str*)dest;
	vector_str* vec2 = (vector_str*)src;
	
	vec1->size = 0;
	vec1->capacity = 0;
	
	/*not much else we can do here*/
	if (!(vec1->a = (char**)malloc(vec2->capacity*sizeof(char*)))) {
		assert(vec1->a != NULL);
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
int push_str(vector_str* vec, char* a)
{
	char** tmp;
	size_t tmp_sz;
	if (vec->capacity == vec->size) {
		tmp_sz = VEC_STR_ALLOCATOR(vec->capacity);
		if (!(tmp = (char**)realloc(vec->a, sizeof(char*)*tmp_sz))) {
			assert(tmp != NULL);
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
void pop_str(vector_str* vec, char* ret)
{
	if (ret)
		strcpy(ret, vec->a[--vec->size]);
	free(vec->a[vec->size]);
}

/** Return pointer to last element */
char** back_str(vector_str* vec)
{
	return &vec->a[vec->size-1];
}






/** Increase the size of the array num items.  Items
 *  are memset to NULL since they will be freed when
    popped or the vector is freed.*/
int extend_str(vector_str* vec, size_t num)
{
	char** tmp;
	size_t tmp_sz;
	if (vec->capacity < vec->size + num) {
		tmp_sz = vec->capacity + num + VEC_STR_START_SZ;
		if (!(tmp = (char**)realloc(vec->a, sizeof(char*)*tmp_sz))) {
			assert(tmp != NULL);
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
int insert_str(vector_str* vec, size_t i, char* a)
{
	char** tmp;
	size_t tmp_sz;
	if (vec->capacity == vec->size) {
		tmp_sz = VEC_STR_ALLOCATOR(vec->capacity);
		if (!(tmp = (char**)realloc(vec->a, sizeof(char*)*tmp_sz))) {
			assert(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}

	memmove(&vec->a[i+1], &vec->a[i], (vec->size-i)*sizeof(char*));
	vec->a[i] = mystrdup(a);
	vec->size++;
	return 1;
}


/**
 * Insert the first num elements of array a at index i.
 * Note that it is the user's responsibility to pass in valid
 * arguments.
 */
int insert_array_str(vector_str* vec, size_t i, char** a, size_t num)
{
	char** tmp;
	size_t tmp_sz, j;
	if (vec->capacity < vec->size + num) {
		tmp_sz = vec->capacity + num + VEC_STR_START_SZ;
		if (!(tmp = (char**)realloc(vec->a, sizeof(char*)*tmp_sz))) {
			assert(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}

	memmove(&vec->a[i+num], &vec->a[i], (vec->size-i)*sizeof(char*));
	for (j=0; j<num; ++j) {
		vec->a[j+i] = mystrdup(a[j]);
	}
	
	vec->size += num;
	return 1;
}

/**
 * Replace string at i with a. If ret != NULL, strcpy the old str to it.
 * See pop_str warning
 * */
void replace_str(vector_str* vec, size_t i, char* a, char* ret)
{
	if (ret)
		strcpy(ret, vec->a[i]);
	free(vec->a[i]);
	vec->a[i] = mystrdup(a);
}


/**
 * Erases strings from start to end inclusive.
 * Example erases(myvec, 1, 3) would free and remove strings at 1, 2, and 3 and the string
 * that was at index 4 would now be at 1 etc.
 */
void erase_str(vector_str* vec, size_t start, size_t end)
{
	size_t i;
	size_t d = end - start + 1;
	for (i=start; i<=end; i++) {
		free(vec->a[i]);
	}
	
	memmove(&vec->a[start], &vec->a[end+1], (vec->size-1-end)*sizeof(char*));
	vec->size -= d;
}





/** Makes sure the vector capacity is >= size (parameter not member). */
int reserve_str(vector_str* vec, size_t size)
{
	char** tmp;
	if (vec->capacity < size) {
		if (!(tmp = (char**)realloc(vec->a, sizeof(char*)*(size+VEC_STR_START_SZ)))) {
			assert(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = size + VEC_STR_START_SZ;
	}
	return 1;
}

/** Set capacity to size.
 * You will lose data if you shrink the capacity below the current size.
 * If you do, the size will be set to capacity of course.
*/
int set_capacity_str(vector_str* vec, size_t size)
{
	size_t i;
	char** tmp;
	if (size < vec->size) {
		for(i=vec->size-1; i>size-1; i--) {
			free(vec->a[i]);
		}

		vec->size = size;
	}

	if (!(tmp = (char**)realloc(vec->a, sizeof(char*)*size))) {
		assert(tmp != NULL);
		return 0;
	}
	vec->a = tmp;
	vec->capacity = size;
	return 1;
}



/** Sets all size elements to val. */
void set_val_sz_str(vector_str* vec, char* val)
{
	size_t i;
	for(i=0; i<vec->size; i++) {
		free(vec->a[i]);

		/* not worth checking/(char**)reallocing to me */
		vec->a[i] = mystrdup(val);
	}
}


/** Fills entire allocated array (capacity) with val.  Size is set
 * to capacity in this case because strings are individually dynamically allocated.
 * This is different from vector_i, vector_d and vector (without a free function) where the size stays the same.
   TODO  Remove this function?  even more unnecessary than for vector_i and vector_d and different behavior*/
void set_val_cap_str(vector_str* vec, char* val)
{
	size_t i;
	for (i=0; i<vec->capacity; i++) {
		if (i<vec->size) {
			free(vec->a[i]);
		}
		
		vec->a[i] = mystrdup(val);
	}
	vec->size = vec->capacity;
}


/** Clears the contents of vector (frees all strings) and sets size to 0. */
void clear_str(vector_str* vec)
{
	int i;
	for (i=0; i<vec->size; i++) {
		free(vec->a[i]);
	}
	
	vec->size = 0;
}


/** Frees contents (individual strings and array) and frees vector so don't use after calling this. */
void free_vec_str_heap(void* vec)
{
	size_t i;
	vector_str* tmp = (vector_str*)vec;
	for (i=0; i<tmp->size; i++) {
		free(tmp->a[i]);
	}
	
	free(tmp->a);
	free(tmp);
}


/** Frees the internal array and sets size and capacity to 0 */
void free_vec_str(void* vec)
{
	size_t i;
	vector_str* tmp = (vector_str*)vec;
	for (i=0; i<tmp->size; i++) {
		free(tmp->a[i]);
	}
	
	free(tmp->a);
	tmp->size = 0;
	tmp->capacity = 0;
}
