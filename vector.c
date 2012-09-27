#include "vector.h"

#define STDERR(X) fprintf(stderr, X)


size_t VEC_I_START_SZ = 50;
size_t VEC_D_START_SZ = 50;
size_t VEC_START_SZ = 20;
size_t VEC_S_START_SZ = 20;

#define VECI_ALLOCATOR(x) (x*2)
#define VECD_ALLOCATOR(x) (x*2)
#define VECS_ALLOCATOR(x) (x*2)
#define VEC_ALLOCATOR(x) (x*2)



/**
 * Creates a new vector_i.
 * Vector size set to (size > 0) ? size : 0;
 * Capacity to (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + VEC_I_START_SZ
 * in other words capacity has to be at least 1 and >= to vec->size of course. 
 */
vector_i* vec_i(size_t size, size_t capacity)
{
	vector_i* vec;
	if (!(vec = malloc(sizeof(vector_i)))) {
		STDERR("Error allocating memory\n");
		return NULL;
	}

	vec->size = (size > 0) ? size : 0;
	vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + VEC_I_START_SZ;

	if (!(vec->a = malloc(vec->capacity*sizeof(int)))) {
		STDERR("Error allocating memory\n");
		free(vec);
		return NULL;
	}

	return vec;
}

/** Create and initialize vector_i with num elements of vals.
 *  Capacity is set to num + VEC_I_START_SZ.
 *  If vals == NULL or num < 1 return NULL (you should just be using vec_i(size_t, size_t))
 */
vector_i* init_vec_i(int* vals, size_t num)
{
	vector_i* vec;
	
	if (!vals || num < 1)
		return NULL;
	
	if (!(vec = malloc(sizeof(vector_i)))) {
		STDERR("Error allocating memory\n");
		return NULL;
	}

	vec->capacity = num + VEC_I_START_SZ;
	vec->size = num;
	if (!(vec->a = malloc(vec->capacity*sizeof(int)))) {
		STDERR("Error allocating memory\n");
		free(vec);
		return NULL;
	}

	memcpy(vec->a, vals, sizeof(int)*num);

	return vec;
}

/** Same as vec_i() except the vector passed in was declared on the stack so
 *  it isn't allocated in this function.  Use the free_veci_stack in that case.
 */
int vec_i_stack(vector_i* vec, size_t size, size_t capacity)
{
	vec->size = (size > 0) ? size : 0;
	vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + VEC_I_START_SZ;

	if (!(vec->a = malloc(vec->capacity*sizeof(int)))) {
		STDERR("Error allocating memory\n");
		vec->size = vec->capacity = 0;
		return 0;
	}

	return 1;
}


/** Same as init_vec_i() except the vector passed in was declared on the stack so
 *  it isn't allocated in this function.  Use the free_veci_stack in that case.
 */
int init_vec_i_stack(vector_i* vec, int* vals, size_t num)
{
	if (!vals || num < 1)
		return 0;

	vec->capacity = num + VEC_I_START_SZ;
	vec->size = num;
	if (!(vec->a = malloc(vec->capacity*sizeof(int)))) {
		STDERR("Error allocating memory\n");
		vec->size = vec->capacity = 0;
		return 0;
	}

	memcpy(vec->a, vals, sizeof(int)*num);

	return 1;
}


/** Makes dest an identical copy of src.  The parameters
 *  are void so it can be used as the constructor when making
 *  a vector of vector_i's.  Assumes dest (the structure)
 *  is already allocated (probably on the stack).
 */
void veci_copy(void* dest, void* src)
{
	vector_i* vec1 = dest;
	vector_i* vec2 = src;
	
	vec1->size = 0;
	vec1->capacity = 0;
	
	/*not much else we can do here*/
	if (!(vec1->a = malloc(vec2->capacity*sizeof(int)))) {
		STDERR("Error allocating memory\n");
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
int push_backi(vector_i* vec, int a)
{
	void* tmp;
	size_t tmp_sz;
	if (vec->capacity == vec->size) {
		tmp_sz = VECI_ALLOCATOR(vec->capacity);
		if (!(tmp = realloc(vec->a, sizeof(int)*tmp_sz))) {
			STDERR("Error allocating memory\n");
			return 0;
		}
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}
	
	vec->a[vec->size++] = a;
	return 1;
}



/** Remove and return the last element (size decreased 1).*/
int pop_backi(vector_i* vec)
{
	return vec->a[--vec->size];
}


/**
 * Insert a at index i (0 based).
 * Everything from that index and right is shifted one to the right.
 *\todo check for i < 0 or > size ?
 */
int inserti(vector_i* vec, size_t i, int a)
{
	void* tmp;
	size_t tmp_sz;
	if (vec->capacity == vec->size) {
		tmp_sz = VECI_ALLOCATOR(vec->capacity);
		if (!(tmp = realloc(vec->a, sizeof(int)*tmp_sz))) {
			STDERR("Error allocating memory\n");
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
int insert_arrayi(vector_i* vec, size_t i, int* a, size_t num)
{
	void* tmp;
	size_t tmp_sz;
	if (vec->capacity < vec->size + num) {
		tmp_sz = vec->capacity + num + VEC_I_START_SZ;
		if (!(tmp = realloc(vec->a, sizeof(int)*tmp_sz))) {
			STDERR("Error allocating memory\n");
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
 * Example erases(myvec, 1, 3) would free and remove elements at 1, 2, and 3 and the element
 * that was at index 4 would now be at 1 etc. \todo check start and end in range?
 */
void erasei(vector_i* vec, size_t start, size_t end)
{
	size_t d = end - start + 1;
	memmove(&vec->a[start], &vec->a[end+1], (vec->size-1-end)*sizeof(int));
	vec->size -= d;
}


/** Make sure capacity is at least size(parameter not member). */
int reservei(vector_i* vec, size_t size)
{
	void* tmp;
	if (vec->capacity < size) {
		if (!(tmp = realloc(vec->a, sizeof(int)*(size+VEC_I_START_SZ)))) {
			STDERR("Error allocating memory\n");
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
int set_capacityi(vector_i* vec, size_t size)
{
	void* tmp;
	if (size < vec->size)
		vec->size = size;

	if (!(tmp = realloc(vec->a, sizeof(int)*size))) {
		STDERR("Error allocating memory\n");
		return 0;
	}
	vec->a = tmp;
	vec->capacity = size;
	return 1;
}



/** Set all size elements to val. */
void set_val_szi(vector_i* vec, int val)
{
	size_t i;
	for (i=0; i<vec->size; i++)
		vec->a[i] = val;
}


/** Fills entire allocated array (capacity) with val. */
void set_val_capi(vector_i* vec, int val)
{
	size_t i;
	for (i=0; i<vec->capacity; i++)
		vec->a[i] = val;
}


/**If you don't want to access capacity directly for some reason.*/
int capacityi(vector_i* vec) { return vec->capacity; }

/**If you don't want to access size directly for some reason.*/
int sizei(vector_i* vec) { return vec->size; }

/** Sets size to 0 (does not clear contents).*/
void cleari(vector_i* vec) { vec->size = 0; }

/** Frees everything so don't use vec after calling this. */
void free_veci(void* vec)
{
	vector_i* tmp = vec;
	free(tmp->a);
	free(tmp);
}

/** Frees the internal array and sets size and capacity to 0 */
void free_veci_stack(void* vec)
{
	vector_i* tmp = vec;
	free(tmp->a);
	tmp->size = 0;
	tmp->capacity = 0;
}






/**
 * Creates a new vector_d.
 * Vector size set to (size > 0) ? size : 0;
 * Capacity to (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + VEC_D_START_SZ
 * in other words capacity has to be at least 1 and >= to vec->size of course.
 */
vector_d* vec_d(size_t size, size_t capacity)
{
	vector_d* vec;
	void* tmp;
	
	if (!(vec = malloc(sizeof(vector_d)))) {
		STDERR("Error allocating memory\n");
		return NULL;
	}

	vec->size = (size > 0) ? size : 0;
	vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + VEC_D_START_SZ;

	if (!(tmp = malloc(vec->capacity*sizeof(double)))) {
		STDERR("Error allocating memory\n");
		free(vec);
		return NULL;
	}
	vec->a = tmp;

	return vec;
}


/** Create and initialize vector_d with num elements of vals.
 *  Capacity is set to num + VEC_D_START_SZ.
 *  If vals == NULL or num < 1 return NULL (you should just be using vec_d(size_t, size_t))
 */
vector_d* init_vec_d(double* vals, size_t num)
{
	vector_d* vec;
	
	if (!vals || num < 1)
		return NULL;
	
	if (!(vec = malloc(sizeof(vector_d)))) {
		STDERR("Error allocating memory\n");
		return NULL;
	}

	vec->capacity = num + VEC_D_START_SZ;
	vec->size = num;
	if (!(vec->a = malloc(vec->capacity*sizeof(double)))) {
		STDERR("Error allocating memory\n");
		free(vec);
		return NULL;
	}

	memcpy(vec->a, vals, sizeof(double)*num);

	return vec;
}


/** Same as vec_d() except the vector passed in was declared on the stack so
 *  it isn't allocated in this function.  Use the free_vecd_stack in that case.
 */
int vec_d_stack(vector_d* vec, size_t size, size_t capacity)
{
	vec->size = (size > 0) ? size : 0;
	vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + VEC_D_START_SZ;

	if (!(vec->a = malloc(vec->capacity*sizeof(double)))) {
		STDERR("Error allocating memory\n");
		vec->size = vec->capacity = 0;
		return 0;
	}

	return 1;
}

/** Same as init_vec_d() except the vector passed in was declared on the stack so
 *  it isn't allocated in this function.  Use the free_vecd_stack in that case.
 */
int init_vec_d_stack(vector_d* vec, double* vals, size_t num)
{
	if (!vals || num < 1)
		return 0;

	vec->capacity = num + VEC_D_START_SZ;
	vec->size = num;
	if (!(vec->a = malloc(vec->capacity*sizeof(double)))) {
		STDERR("Error allocating memory\n");
		vec->size = vec->capacity = 0;
		return 0;
	}

	memcpy(vec->a, vals, sizeof(double)*num);

	return 1;
}

/** Makes dest an identical copy of src.  The parameters
 *  are void so it can be used as the constructor when making
 *  a vector of vector_d's.  Assumes dest (the structure)
 *  is already allocated (probably on the stack).
 */
void vecd_copy(void* dest, void* src)
{
	vector_d* vec1 = dest;
	vector_d* vec2 = src;
	
	vec1->size = 0;
	vec1->capacity = 0;
	
	/*not much else we can do here*/
	if (!(vec1->a = malloc(vec2->capacity*sizeof(double)))) {
		STDERR("Error allocating memory\n");
		return;
	}
	
	memcpy(vec1->a, vec2->a, vec2->size*sizeof(double));
	vec1->size = vec2->size;
	vec1->capacity = vec2->capacity;
}







/** Append a to end of vector (size increased 1).
 * Capacity is increased by doubling when necessary.
 */
int push_backd(vector_d* vec, double a)
{
	void* tmp;
	size_t tmp_sz;
	if (vec->capacity == vec->size) {
		tmp_sz = VECD_ALLOCATOR(vec->capacity);
		if (!(tmp = realloc(vec->a, sizeof(double)*tmp_sz))) {
			STDERR("Error allocating memory\n");
			return 0;
		}
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}
	vec->a[vec->size++] = a;
	return 1;
}


/** Remove and return the last element (size decreased 1).*/
double pop_backd(vector_d* vec)
{
	return vec->a[--vec->size];
}


/**
 * Insert a at index i (0 based).
 * Everything from that index and right is shifted one to the right.
 *\todo check for i < 0 or > size ?
 */
int insertd(vector_d* vec, size_t i, double a)
{
	void* tmp;
	size_t tmp_sz;
	if (vec->capacity == vec->size) {
		tmp_sz = VECD_ALLOCATOR(vec->capacity);
		if (!(tmp = realloc(vec->a, sizeof(double)*tmp_sz))) {
			STDERR("Error allocating memory\n");
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
int insert_arrayd(vector_d* vec, size_t i, double* a, size_t num)
{
	void* tmp;
	size_t tmp_sz;
	if (vec->capacity < vec->size + num) {
		tmp_sz = vec->capacity + num + VEC_I_START_SZ;
		if (!(tmp = realloc(vec->a, sizeof(double)*tmp_sz))) {
			STDERR("Error allocating memory\n");
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
 * Example erases(myvec, 1, 3) would free and remove elements at 1, 2, and 3 and the element
 * that was at index 4 would now be at 1 etc. \todo check start and end in range?
 */
void erased(vector_d* vec, size_t start, size_t end)
{
	size_t d = end - start + 1;
	memmove(&vec->a[start], &vec->a[end+1], (vec->size-1-end)*sizeof(double));
	vec->size -= d;
}


/** Make sure capacity is at least size(parameter not member). */
int reserved(vector_d* vec, size_t size)
{
	void* tmp;
	if (vec->capacity < size) {
		if (!(tmp = realloc(vec->a, sizeof(double)*(size+VEC_D_START_SZ)))) {
			STDERR("Error allocating memory\n");
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
int set_capacityd(vector_d* vec, size_t size)
{
	void* tmp;
	if (size < vec->size)
		vec->size = size;

	if (!(tmp = realloc(vec->a, sizeof(double)*size))) {
		STDERR("Error allocating memory\n");
		return 0;
	}
	vec->a = tmp;
	vec->capacity = size;
	return 1;
}


/** Set all size elements to val. */
void set_val_szd(vector_d* vec, double val)
{
	size_t i;
	for(i=0; i<vec->size; i++)
		vec->a[i] = val;
}


/** Fills entire allocated array (capacity) with val. */
void set_val_capd(vector_d* vec, double val)
{
	size_t i;
	for(i=0; i<vec->capacity; i++)
		vec->a[i] = val;
}



/**If you don't want to access capacity directly for some reason.*/
int capacityd(vector_d* vec) { return vec->capacity; }

/**If you don't want to access size directly for some reason.*/
int sized(vector_d* vec) { return vec->size; }

/** Sets size to 0 (does not clear contents).*/
void cleard(vector_d* vec) { vec->size = 0; }


/** Frees everything so don't use vec after calling this. */
void free_vecd(void* vec)
{
	vector_d* tmp = vec;
	free(tmp->a);
	free(tmp);
}

/** Frees the internal array and sets size and capacity to 0 */
void free_vecd_stack(void* vec)
{
	vector_d* tmp;
	free(tmp->a);
	tmp->size = 0;
	tmp->capacity = 0;
}





/*  String vector */

/** Useful utility function since strdup isn't in standard C.*/
char* mystrdup(const char* str)
{
	/* if (!str)
	 * 	return NULL;
	 */
	size_t len = strlen(str);
	char* temp = calloc(len+1, sizeof(char));
	if (!temp)
		return NULL;
	
	return memcpy(temp, str, len);  /* memcpy returns to, and calloc already nulled last char */
}



/**
 * Create a new vector_s.
 * Vector size set to (size > 0) ? size : 0;
 * Capacity to (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + VEC_S_START_SZ
 * in other words capacity has to be at least 1 and >= to vec->size of course.
 * Note: vector_s does not copy pointers passed in but duplicates the strings
 * they point to (using mystrdup()) so you don't have to worry about freeing
 * or changing the contents of variables that you've pushed or inserted; it
 * won't affect the values vector.
 */
vector_s* vec_s(size_t size, size_t capacity)
{
	vector_s* vec;
	if (!(vec = malloc(sizeof(vector_s)))) {
		STDERR("Error allocating memory\n");
		return NULL;
	}

	vec->size = (size > 0) ? size : 0;
	vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + VEC_S_START_SZ;

	/* calloc here because it we free before poppirg/erasing and since nothing is
	 * allocated these need to be NULL to not cause problems */
	if (!(vec->a = calloc(vec->capacity, sizeof(char*)))) {
		STDERR("Error allocating memory\n");
		free(vec);
		return NULL;
	}

	return vec;
}


/** Create and initialize vector_s with num elements of vals.
 *  If vals is NULL, or num < 1, it returns NULL.  You should
 *  use vec_s(size_t) instead in those cases.
 */
vector_s* init_vec_s(char** vals, size_t num)
{
	vector_s* vec;
	size_t i;
	
	if (!vals || num < 1)
		return NULL;
	
	if (!(vec = malloc(sizeof(vector_s)))) {
		STDERR("Error allocating memory\n");
		return NULL;
	}

	vec->capacity = num + VEC_S_START_SZ;
	vec->size = num;
	if (!(vec->a = malloc(vec->capacity*sizeof(char*)))) {
		STDERR("Error allocating memory\n");
		free(vec);
		return NULL;
	}

	for(i=0; i<num; i++)
		vec->a[i] = mystrdup(vals[i]);

	return vec;
}


/** Same as vec_s() except the vector passed in was declared on the stack so
 *  it isn't allocated in this function.  Use the free_vecs_stack in that case
 */
int vec_s_stack(vector_s* vec, size_t size, size_t capacity)
{
	vec->size = (size > 0) ? size : 0;
	vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + VEC_S_START_SZ;

	/* calloc here because it we free before poppirg/erasing and since nothing is
	 * allocated these need to be NULL to not cause problems */
	if (!(vec->a = calloc(vec->capacity, sizeof(char*)))) {
		STDERR("Error allocating memory\n");
		vec->size = vec->capacity = 0;
		return 0;
	}

	return 1;
}

/** Same as init_vec_s() except the vector passed in was declared on the stack so
 *  it isn't allocated in this function.  Use the free_vecs_stack in that case
 */
int init_vec_s_stack(vector_s* vec, char** vals, size_t num)
{
	size_t i;
	
	if (!vals || num < 1)
		return 0;
	
	vec->capacity = num + VEC_S_START_SZ;
	vec->size = num;
	if (!(vec->a = malloc(vec->capacity*sizeof(char*)))) {
		STDERR("Error allocating memory\n");
		vec->size = vec->capacity = 0;
		return 0;
	}

	for(i=0; i<num; i++)
		vec->a[i] = mystrdup(vals[i]);

	return 1;
}


/** Makes dest an identical copy of src.  The parameters
 *  are void so it can be used as the constructor when making
 *  a vector of vector_s's.  Assumes dest (the structure)
 *  is already allocated (probably on the stack).
 */
void vecs_copy(void* dest, void* src)
{
	size_t i;
	vector_s* vec1 = dest;
	vector_s* vec2 = src;
	
	vec1->size = 0;
	vec1->capacity = 0;
	
	/*not much else we can do here*/
	if (!(vec1->a = malloc(vec2->capacity*sizeof(char*)))) {
		STDERR("Error allocating memory\n");
		return;
	}
	
	for (i=0; i<vec2->size; ++i)
		vec1->a[i] = mystrdup(vec2->a[i]);
	
	vec1->size = vec2->size;
	vec1->capacity = vec2->capacity;
}



/**
 * Append a to end of vector (size increased 1).
 * Capacity is increased by doubling when necessary.
 */
int push_backs(vector_s* vec, char* a)
{
	void* tmp;
	size_t tmp_sz;
	if (vec->capacity == vec->size) {
		tmp_sz = VECS_ALLOCATOR(vec->capacity);
		if (!(tmp = realloc(vec->a, sizeof(char*)*tmp_sz))) {
			STDERR("Error allocating memory\n");
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
void pop_backs(vector_s* vec, char* ret)
{
	if (ret)
		strcpy(ret, vec->a[--vec->size]);
	free(vec->a[vec->size]);
}

/**
 * Insert a at index i (0 based).
 * Everything from that index and right is shifted one to the right.
 *\todo check for i < 0 or > size ?
 */
int inserts(vector_s* vec, size_t i, char* a)
{
	void* tmp;
	size_t tmp_sz;
	if (vec->capacity == vec->size) {
		tmp_sz = VECS_ALLOCATOR(vec->capacity);
		if (!(tmp = realloc(vec->a, sizeof(char*)*tmp_sz))) {
			STDERR("Error allocating memory\n");
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
int insert_arrays(vector_s* vec, size_t i, char** a, size_t num)
{
	void* tmp;
	size_t tmp_sz, j;
	if (vec->capacity < vec->size + num) {
		tmp_sz = vec->capacity + num + VEC_I_START_SZ;
		if (!(tmp = realloc(vec->a, sizeof(char*)*tmp_sz))) {
			STDERR("Error allocating memory\n");
			return 0;
		}
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}

	memmove(&vec->a[i+num], &vec->a[i], (vec->size-i)*sizeof(char*));
	for (j=0; j<num; ++j)
		vec->a[j+i] = mystrdup(a[j]);
	vec->size += num;
	return 1;
}




/**
 * Erases strings from start to end inclusive.
 * Example erases(myvec, 1, 3) would free and remove strings at 1, 2, and 3 and the string
 * that was at index 4 would now be at 1 etc.
 */
void erases(vector_s* vec, size_t start, size_t end)
{
	size_t i;
	size_t d = end - start + 1;
	for (i=start; i<=end; i++)
		free(vec->a[i]);
	
	memmove(&vec->a[start], &vec->a[end+1], (vec->size-1-end)*sizeof(char*));
	vec->size -= d;
}





/** Makes sure the vector capacity is >= size (parameter not member). */
int reserves(vector_s* vec, size_t size)
{
	void* tmp;
	if (vec->capacity < size) {
		if (!(tmp = realloc(vec->a, sizeof(char*)*(size+VEC_S_START_SZ)))) {
			STDERR("Error allocating memory\n");
			return 0;
		}
		vec->a = tmp;
		vec->capacity = size + VEC_S_START_SZ;
	}
	return 1;
}

/** Set capacity to size.
 * You will lose data if you shrink the capacity below the current size.
 * If you do, the size will be set to capacity of course.
*/
int set_capacitys(vector_s* vec, size_t size)
{
	size_t i;
	void* tmp;
	if (size < vec->size) {
		for(i=vec->size-1; i>size-1; i--)
			free(vec->a[i]);

		vec->size = size;
	}

	if (!(tmp = realloc(vec->a, sizeof(char*)*size))) {
		STDERR("Error allocating memory\n");
		return 0;
	}
	vec->a = tmp;
	vec->capacity = size;
	return 1;
}



/** Sets all size elements to val. */
void set_val_szs(vector_s* vec, char* val)
{
	size_t i;
	for(i=0; i<vec->size; i++) {
		free(vec->a[i]);

		/* not worth checking/reallocing to me */
		vec->a[i] = mystrdup(val);
	}
}


/** Fills entire allocated array (capacity) with val.  Size is set
 * to capacity in this case because strings are individually dynamically allocated.
 * This is different from vector_i, vector_d and vector (without a free function) where the size stays the same. 
   TODO  Remove this function?  even more unnecessary than for vector_i and vector_d and different behavior*/
void set_val_caps(vector_s* vec, char* val)
{
	size_t i;
	for (i=0; i<vec->capacity; i++) {
		if (i<vec->size)
			free(vec->a[i]);

		vec->a[i] = mystrdup(val);
	}
	vec->size = vec->capacity;
}


/** If you don't want to acccess capacity directly for some reason. */
int capacitys(vector_s* vec) { return vec->capacity; }

/** If you don't want to access size directly for some reason. */
int sizes(vector_s* vec) { return vec->size; }


/** Clears the contents of vector (frees all strings) and sets size to 0. */
void clears(vector_s* vec)
{
	int i;
	for (i=0; i<vec->size; i++)
		free(vec->a[i]);
	
	vec->size = 0;
}


/** Frees contents (individual strings and array) and frees vector so don't use after calling this. */
void free_vecs(void* vec)
{
	size_t i;
	vector_s* tmp = vec;
	for (i=0; i<tmp->size; i++)
		free(tmp->a[i]);
	
	free(tmp->a);
	free(tmp);
}


/** Frees the internal array and sets size and capacity to 0 */
void free_vecs_stack(void* vec)
{
	size_t i;
	vector_s* tmp = vec;
	for (i=0; i<tmp->size; i++)
		free(tmp->a[i]);
	
	free(tmp->a);
	tmp->size = 0;
	tmp->capacity = 0;
}




/*  general vector */

/**
 * Creates a new vector.
 * Vector size set to (size > 0) ? size : 0;
 * Capacity to (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + VEC_START_SZ
 * in other words capacity has to be at least 1 and >= to vec->size of course.
 * elem_sz is the size of the type you want to store ( ie sizeof(T) where T is your type ).
 * You can pass in a function, elem_free, to be called on every element before it is removed
 * from the vector to free any dynamically allocated memory.  For example if you passed
 * in sizeof(char*) for elem_sz, and wrappers around the standard free(void*) function for elem_free
 * and strdup (or mystrdup in this project) for elem_init you could
 * make vector work exactly like vector_s.  Pass in NULL, to not use the function parameters.
 *
 * All functions call elem_free before overwriting/popping/erasing elements if elem_free is provided.
 *
 * elem_init is only used in set_val_sz and set_val_cap because in those cases you are setting many elements
 * to a single "value" and using the elem_init functionality you can provide what amounts to a copy constructor
 * which duplicates dynamically allocated memory instead of just copying the pointer ie just like strdup
 * or mystrdup does with a string.  This allows the free function to work correctly when called on all those
 * elements.  If you didn't provide an elem_init function but did provide a free function, then
 * after calling one of the set_val functions, eventually the free function would be called on all those
 * elements and you'd get a double free or corruption error.
 *
 * See the other functions and the tests for more behavioral/usage details.
 */
vector* vec(size_t size, size_t capacity, size_t elem_sz, void(*elem_free)(void*), void(*elem_init)(void*, void*))
{
	vector* vec;
	if (!(vec = malloc(sizeof(vector)))) {
		STDERR("Error allocating memory\n");
		return NULL;
	}

	vec->size = (size > 0) ? size : 0;
	vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + VEC_START_SZ;

	vec->elem_size = elem_sz;
	
	/*not calloc here and init_vec as in vector_s because elem_free cannot be calling free directly*/
	if (!(vec->a = malloc(vec->capacity*elem_sz))) {
		STDERR("Error allocating memory\n");
		free(vec);
		return NULL;
	}

	vec->elem_free = elem_free;
	vec->elem_init = elem_init;

	return vec;
}



/** Create and initialize vector with num elements of vals.
 *  If vals is NULL, capacity is set to num + VEC_I_START_SZ.
 *  Size is set to num in the first place, 0 otherwise.
 *  elem_sz is the size of the type you want to store ( ie sizeof(T) where T is your type ).
 *  See vec() for more information about the elem_free and elem_init parameters.
 */
vector* init_vec(void* vals, size_t num, size_t elem_sz, void(*elem_free)(void*), void(*elem_init)(void*, void*))
{
	vector* vec;
	size_t i;
	
	if (!vals || num < 1)
		return NULL;
	
	if (!(vec = malloc(sizeof(vector)))) {
		STDERR("Error allocating memory\n");
		return NULL;
	}

	vec->elem_size = elem_sz;

	vec->capacity = num+VEC_START_SZ;
	vec->size = num;
	if (!(vec->a = malloc(vec->capacity*elem_sz))) {
		STDERR("Error allocating memory\n");
		free(vec);
		return NULL;
	}

	if (elem_init) {
		for (i=0; i<num; ++i) {
			elem_init(&vec->a[i*elem_sz], &((byte*)vals)[i*elem_sz]);
		}
	} else {
		memcpy(vec->a, vals, elem_sz*num);
	}
	
	vec->elem_free = elem_free;
	vec->elem_init = elem_init;

	return vec;
}

/** Same as vec() except the vector passed in was declared on the stack so
 *  it isn't allocated in this function.  Use the free_vec_stack in that case
 */
int vec_stack(vector* vec, size_t size, size_t capacity, size_t elem_sz, void(*elem_free)(void*), void(*elem_init)(void*, void*))
{
	vec->size = (size > 0) ? size : 0;
	vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + VEC_START_SZ;

	vec->elem_size = elem_sz;
	
	if (!(vec->a = malloc(vec->capacity*elem_sz))) {
		STDERR("Error allocating memory\n");
		vec->size = vec->capacity = 0;
		return 0;
	}

	vec->elem_free = elem_free;
	vec->elem_init = elem_init;

	return 1;
}

/** Same as init_vec() except the vector passed in was declared on the stack so
 *  it isn't allocated in this function.  Use the free_vec_stack in that case
 */
int init_vec_stack(vector* vec, void* vals, size_t num, size_t elem_sz, void(*elem_free)(void*), void(*elem_init)(void*, void*))
{
	size_t i;
	if (!vals || num < 1)
		return 0;
	
	vec->elem_size = elem_sz;

	vec->capacity = num+VEC_START_SZ;
	vec->size = num;
	if (!(vec->a = malloc(vec->capacity*elem_sz))) {
		STDERR("Error allocating memory\n");
		vec->size = vec->capacity = 0;
		return 0;
	}

	if (elem_init) {
		for (i=0; i<num; ++i) {
			elem_init(&vec->a[i*elem_sz], &((byte*)vals)[i*elem_sz]);
		}
	} else {
		memcpy(vec->a, vals, elem_sz*num);
	}

	vec->elem_free = elem_free;
	vec->elem_init = elem_init;

	return 1;
}


/** Makes dest an identical copy of src.  The parameters
 *  are void so it can be used as the constructor when making
 *  a vector of vector's. (I would reccomend against doing that, and using 
 *  generate_code.py to make your own type instead).   Assumes dest (the structure)
 *  is already allocated (probably on the stack).
 */
void vec_copy(void* dest, void* src)
{
	size_t i;
	vector* vec1 = dest;
	vector* vec2 = src;
	
	vec1->size = 0;
	vec1->capacity = 0;
	
	/*not much else we can do here*/
	if (!(vec1->a = malloc(vec2->capacity*vec2->elem_size))) {
		STDERR("Error allocating memory\n");
		return;
	}

	vec1->size = vec2->size;
	vec1->capacity = vec2->capacity;
	vec1->elem_size = vec2->elem_size;
	vec1->elem_init = vec2->elem_init;
	vec1->elem_free = vec2->elem_free;
	
	if (vec1->elem_init) {
		for (i=0; i<vec1->size; ++i)
			vec1->elem_init(&vec1->a[i*vec1->elem_size], &vec2->a[i*vec1->elem_size]);
	} else {
		memcpy(vec1->a, vec2->a, vec1->size*vec1->elem_size);
	}
}


/** Append a to end of vector (size increased 1).
 * Capacity is increased by doubling when necessary.
 */
int push_back(vector* vec, void* a)
{
	void* tmp;
	size_t tmp_sz;
	if (vec->capacity > vec->size) {
		if (vec->elem_init)
			vec->elem_init(&vec->a[vec->size*vec->elem_size], a);
		else
			memcpy(&vec->a[vec->size*vec->elem_size], a, vec->elem_size);

	} else {
		tmp_sz = VEC_ALLOCATOR(vec->capacity);
		if (!(tmp = realloc(vec->a, vec->elem_size*tmp_sz))) {
			STDERR("Error allocating memory\n");
			return 0;
		}
		vec->a = tmp;
		
		if (vec->elem_init)
			vec->elem_init(&vec->a[vec->size*vec->elem_size], a);
		else
			memcpy(&vec->a[vec->size*vec->elem_size], a, vec->elem_size);
		
		vec->capacity = tmp_sz;
	}
	
	vec->size++;
	return 1;
}


/** Remove the last element (size decreased 1).
 * Copy the element into ret.  This function assumes
 * that ret is not NULL and is large accept the element and just memcpy's it in.
 * Similar to pop_backs it is users responsibility.
 */
void pop_back(vector* vec, void* ret)
{
	if (ret)
		memcpy(ret, &vec->a[(--vec->size)*vec->elem_size], vec->elem_size);
	else
		vec->size--;

	if (vec->elem_free)
		vec->elem_free(&vec->a[vec->size*vec->elem_size]);

}

void* vec_get(vector* vec, size_t i)
{
	return &vec->a[i*vec->elem_size];
}



/**
 * Insert a at index i (0 based).
 * Everything from that index and right is shifted one to the right.
 *\todo check for i < 0 or > size-1?
 */
int insert(vector* vec, size_t i, void* a)
{
	void* tmp;
	size_t tmp_sz;
	if (vec->capacity > vec->size) {
		memmove(&vec->a[(i+1)*vec->elem_size], &vec->a[i*vec->elem_size], (vec->size-i)*vec->elem_size);

		if (vec->elem_init)
			vec->elem_init(&vec->a[i*vec->elem_size], a);
		else
			memcpy(&vec->a[i*vec->elem_size], a, vec->elem_size);

	} else {
		tmp_sz = VEC_ALLOCATOR(vec->capacity);
		if (!(tmp = realloc(vec->a, vec->elem_size*tmp_sz))) {
			STDERR("Error allocating memory\n");
			return 0;
		}
		vec->a = tmp;
		
		memmove(&vec->a[(i+1)*vec->elem_size], &vec->a[i*vec->elem_size], (vec->size-i)*vec->elem_size);
		if (vec->elem_init)
			vec->elem_init(&vec->a[i*vec->elem_size], a);
		else
			memcpy(&vec->a[i*vec->elem_size], a, vec->elem_size);
		
		vec->capacity = tmp_sz;
	}

	vec->size++;
	return 1;
}

/**
 * Insert the first num elements of array a at index i.
 * Note that it is the user's responsibility to pass in valid
 * arguments.  Also memcpy is used (when there is no elem_init function) 
 * so don't try to insert part of the vector array into itself 
 * (that would require memmove)
 */
int insert_array(vector* vec, size_t i, void* a, size_t num)
{
	void* tmp;
	size_t tmp_sz, j;
	if (vec->capacity < vec->size + num) {
		tmp_sz = vec->capacity + num + VEC_START_SZ;
		if (!(tmp = realloc(vec->a, vec->elem_size*tmp_sz))) {
			STDERR("Error allocating memory\n");
			return 0;
		}
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}

	memmove(&vec->a[i+num], &vec->a[i], (vec->size-i)*sizeof(int));
	if (vec->elem_init) {
		for (j=0; j<num; ++j)
			vec->elem_init(&vec->a[(j+i)*vec->elem_size], &((byte*)a)[j*vec->elem_size]);
	} else {
		memcpy(&vec->a[i], a, num*vec->elem_size);
	}
	vec->size += num;
	return 1;
}


/**
 * Erases elements from start to end inclusive.
 * Example erases(myvec, 1, 3) would free and remove elements at 1, 2, and 3 and the element
 * that was at index 4 would now be at 1 etc. \todo check start and end in range?
 */
void erase(vector* vec, size_t start, size_t end)
{
	size_t i;
	size_t d = end - start + 1;
	if (vec->elem_free)
		for (i=start; i<=end; i++)
			vec->elem_free(&vec->a[i*vec->elem_size]);

	memmove(&vec->a[start*vec->elem_size], &vec->a[(end+1)*vec->elem_size], (vec->size-1-end)*vec->elem_size);
	vec->size -= d;
}


/** Makes sure capacity >= size (the parameter not the member). */
int reserve(vector* vec, size_t size)
{
	void* tmp;
	if (vec->capacity < size) {
		if( !(tmp = realloc(vec->a, vec->elem_size*(size+VEC_START_SZ))) ) {
			STDERR("Error allocating memory\n");
			return 0;
		}
		vec->a = tmp;
		vec->capacity = size + VEC_START_SZ;
	}
	return 1;
}


/** Set capacity to size.
 * You will lose data if you shrink the capacity below the current size.
 * If you do, the size will be set to capacity of course.
*/
int set_capacity(vector* vec, size_t size)
{
	size_t i;
	void* tmp;
	if (size < vec->size) {
		if (vec->elem_free)
			for (i=vec->size-1; i>=size; i--)
				vec->elem_free(&vec->a[i*vec->elem_size]);

		vec->size = size;
	}

	vec->capacity = size;

	if (!(tmp = realloc(vec->a, vec->elem_size*size))) {
		STDERR("Error allocating memory\n");
		return 0;
	}
	vec-> a = tmp;
	return 1;
}



/** Set all size elements to val. */
void set_val_sz(vector* vec, void* val)
{
	size_t i;

	if (vec->elem_free)
		for(i=0; i<vec->size; i++)
			vec->elem_free(&vec->a[i*vec->elem_size]);

	if (vec->elem_init) {
		for (i=0; i<vec->size; i++)
			vec->elem_init(&vec->a[i*vec->elem_size], val);
	} else {
		for (i=0; i<vec->size; i++)
			memcpy(&vec->a[i*vec->elem_size], val, vec->elem_size);
	}
}


/** Fills entire allocated array (capacity) with val.  If you set a free function
 * then size is set to capacity like vector_s for the same reason, ie I need to know
 * that the free function needs to be called on those elements. 
 * TODO Remove this function?  Same reason as set_val_caps.
 */
void set_val_cap(vector* vec, void* val)
{
	size_t i;
	if (vec->elem_free) {
		for (i=0; i<vec->size; i++)
			vec->elem_free(&vec->a[i*vec->elem_size]);

		vec->size = vec->capacity;
	}

	if (vec->elem_init) {
		for (i=0; i<vec->capacity; i++)
			vec->elem_init(&vec->a[i*vec->elem_size], val);
	} else {
		for (i=0; i<vec->capacity; i++)
			memcpy(&vec->a[i*vec->elem_size], val, vec->elem_size);
	}
}


/** If you don't want to access capacity directly for some reason. */
int capacity(vector* vec) { return vec->capacity; }

/** If you don't want to access size directly for some reason. */
int size(vector* vec) { return vec->size; }


/** Sets size to 0 (does not change contents). */
void clear(vector* vec) { vec->size = 0; }


/** Frees everything so don't use vec after calling this. If you set a free function
 * it will be called on all size elements of course. */
void free_vec(void* vec)
{
	size_t i;
	vector* tmp = vec;
	if (tmp->elem_free)
		for (i=0; i<tmp->size; i++)
			tmp->elem_free(&tmp->a[i*tmp->elem_size]);

	free(tmp->a);
	free(tmp);
}


/** Frees the internal array and sets size and capacity to 0 */
void free_vec_stack(void* vec)
{
	size_t i;
	vector* tmp = vec;
	if (tmp->elem_free)
		for (i=0; i<tmp->size; i++)
			tmp->elem_free(&tmp->a[i*tmp->elem_size]);

	free(tmp->a);

	tmp->size = 0;
	tmp->capacity = 0;
}










