#include "vector.h"

#define STDERR(X) fprintf(stderr, X)


int VEC_I_START_SZ = 50;
int VEC_D_START_SZ = 50;
int VEC_START_SZ = 20;
int VEC_S_START_SZ = 20;


/**
 * Creates a new vector_i.
 * Vector size is set to sz, capacity to sz+VEC_I_START_SZ.
 */
vector_i* vec_i(int sz)
{
	vector_i* vec;
	if( !(vec = calloc(1, sizeof(vector_i))) ) {

		STDERR("Error allocating memory\n");
		return NULL;
	}

	vec->capacity = ( sz>0 ) ? sz + VEC_I_START_SZ : VEC_I_START_SZ;

	if( !(vec->a = calloc(vec->capacity, sizeof(int))) ) {
		STDERR("Error allocating memory\n");

		return NULL;
	}

	vec->size = ( sz>=0 ) ? sz : 0;
	return vec;
}

/** Create and initialize vector_i with num elements of vals.
 *  If vals is NULL, capacity is set to num + VEC_I_START_SZ.
 *  Size is set to num in the first place, 0 otherwise.
 */
vector_i* init_vec_i(int* vals, int num)
{
	vector_i* vec;
	if( !(vec = calloc(1, sizeof(vector_i))) ) {

		STDERR("Error allocating memory\n");

		return NULL;
	}

	if( vals && num>0 ) {
		vec->capacity = num+VEC_I_START_SZ;
		vec->size = num;
		if( !(vec->a = calloc(vec->capacity, sizeof(int))) ) {
			STDERR("Error allocating memory\n");
			return NULL;
		}

		memcpy(vec->a, vals, sizeof(int)*num);
	} else if( !vals && num>0 ) {
		vec->capacity = num+VEC_I_START_SZ;
		vec->size = 0;
		if( !(vec->a = calloc(vec->capacity, sizeof(int))) ) {
			STDERR("Error allocating memory\n");
			return NULL;
		}
	} else
		return NULL;

	return vec;
}



/**
 * Append a to end of vector (size increased 1).
 * Capacity is increased by doubling when necessary.
 */
int push_backi(vector_i* vec, int a)
{
	if( vec->capacity > vec->size ) {
		vec->a[vec->size++] = a;
	} else {
		vec->capacity *= 2;
		if( !(vec->a = realloc(vec->a, sizeof(int)*vec->capacity)) ) {
			STDERR("Error allocating memory\n");
			return 0;
		}
		vec->a[vec->size++] = a;
	}
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
int inserti(vector_i* vec, int i, int a)
{
	if( vec->capacity > vec->size ) {
		memmove(&vec->a[i+1], &vec->a[i], (vec->size-i)*sizeof(int));
		vec->a[i] = a;
	} else {
		vec->capacity *= 2;
		if( !(vec->a = realloc(vec->a, sizeof(int)*vec->capacity)) ) {
			STDERR("Error allocating memory\n");
			return 0;
		}

		memmove(&vec->a[i+1], &vec->a[i], (vec->size-i)*sizeof(int));
		vec->a[i] = a;
	}

	vec->size++;
	return 1;
}


/**
 * Erases elements from start to end inclusive.
 * Example erases(myvec, 1, 3) would free and remove elements at 1, 2, and 3 and the element
 * that was at index 4 would now be at 1 etc. \todo check start and end in range?
 */
void erasei(vector_i* vec, int start, int end)
{
	int d = end-start+1;
	memmove(&vec->a[start], &vec->a[end+1], (vec->size-1-end)*sizeof(int));
	vec->size -= d;
}


/** Make sure capacity is at least size(parameter not member). */
int reservei(vector_i* vec, int size)
{
	if( vec->capacity < size ) {
		if( !(vec->a = realloc(vec->a, sizeof(int)*(size+20))) ) {
			STDERR("Error allocating memory\n");
			return 0;
		}
		vec->capacity = size+20;
	}
	return 1;
}



/** Set capacity to size.
 * You will lose data if you shrink the capacity below the current size.
 * If you do, the size will be set to capacity of course.
*/
int set_capacityi(vector_i* vec, int size)
{
	if( size<vec->size )
		vec->size = size;

	vec->capacity = size;

	if( !(vec->a = realloc(vec->a, sizeof(int)*size)) ) {
		STDERR("Error allocating memory\n");
		return 0;
	}
	return 1;
}



/** Set all size elements to val. */
void set_val_szi(vector_i* vec, int val)
{
	int i;
	for(i=0; i<vec->size; i++)
		vec->a[i] = val;
}


/** Fills entire allocated array (capacity) with val. */
void set_val_capi(vector_i* vec, int val)
{
	int i;
	for(i=0; i<vec->capacity; i++)
		vec->a[i] = val;
}


/**If you don't want to access capacity directly for some reason.*/
int capacityi(vector_i* vec) { return vec->capacity; }

/**If you don't want to access size directly for some reason.*/
int sizei(vector_i* vec) { return vec->size; }

/** Sets size to 0 (does not clear contents).*/
void cleari(vector_i* vec) { vec->size = 0; }

/** Frees everything so don't use vec after calling this. */
void free_veci(vector_i* vec)
{
	free(vec->a);
	free(vec);
}



/**
 * Creates a new vector_D.
 * Vector size is set to sz, capacity to sz+VEC_D_START_SZ.
 */
vector_d* vec_d(int sz)
{
	vector_d* vec;
	if( !(vec = calloc(1, sizeof(vector_d))) ) {

		STDERR("Error allocating memory\n");
		return NULL;
	}

	vec->capacity = ( sz>0 ) ? sz + VEC_D_START_SZ : VEC_D_START_SZ;

	if( !(vec->a = calloc(vec->capacity, sizeof(double))) ) {
		STDERR("Error allocating memory\n");
		return NULL;
	}

	vec->size = ( sz>0 ) ? sz : 0;
	return vec;
}


/** Create and initialize vector_d with num elements of vals.
 *  If vals is NULL, capacity is set to num + VEC_D_START_SZ.
 *  Size is set to num in the first place, 0 otherwise.
 */
vector_d* init_vec_d(double* vals, int num)
{
	vector_d* vec;
	if( !(vec = calloc(1, sizeof(vector_d))) ) {

		STDERR("Error allocating memory\n");
		return NULL;
	}

	if( vals && num>0 ) {
		vec->capacity = num+50;
		vec->size = num;
		if( !(vec->a = calloc(vec->capacity, sizeof(double))) ) {

			return NULL;
		}

		memcpy(vec->a, vals, sizeof(double)*num);
	} else if( !vals && num>0 ) {
		vec->capacity = num+VEC_D_START_SZ;
		vec->size = 0;
		if( !(vec->a = calloc(vec->capacity, sizeof(double))) ) {
			STDERR("Error allocating memory\n");
			return NULL;
		}
	} else
		return NULL;

	return vec;
}


/** Append a to end of vector (size increased 1).
 * Capacity is increased by doubling when necessary.
 */
int push_backd(vector_d* vec, double a)
{
	if( vec->capacity > vec->size ) {
		vec->a[vec->size++] = a;
	} else {
		vec->capacity *= 2;
		if( !(vec->a = realloc(vec->a, sizeof(double)*vec->capacity)) ) {
			STDERR("Error allocating memory\n");
			return 0;
		}
		vec->a[vec->size++] = a;
	}
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
int insertd(vector_d* vec, int i, double a)
{
	if( vec->capacity > vec->size ) {
		memmove(&vec->a[i+1], &vec->a[i], (vec->size-i)*sizeof(double));
		vec->a[i] = a;
	} else {
		vec->capacity *= 2;
		if( !(vec->a = realloc(vec->a, sizeof(double)*vec->capacity)) ) {

			STDERR("Error allocating memory\n");
			return 0;
		}

		memmove(&vec->a[i+1], &vec->a[i], (vec->size-i)*sizeof(double));
		vec->a[i] = a;
	}

	vec->size++;
	return 1;
}

/**
 * Erases elements from start to end inclusive.
 * Example erases(myvec, 1, 3) would free and remove elements at 1, 2, and 3 and the element
 * that was at index 4 would now be at 1 etc. \todo check start and end in range?
 */
void erased(vector_d* vec, int start, int end)
{
	int d = end-start+1;
	memmove(&vec->a[start], &vec->a[end+1], (vec->size-1-end)*sizeof(double));
	vec->size -= d;
}


/** Make sure capacity is at least size(parameter not member). */
int reserved(vector_d* vec, int size)
{
	if( vec->capacity < size ) {
		if( !(vec->a = realloc(vec->a, sizeof(double)*(size+20))) ) {
			STDERR("Error allocating memory\n");
			return 0;
		}
		vec->capacity = size+20;
	}
	return 1;
}


/** Set capacity to size.
 * You will lose data if you shrink the capacity below the current size.
 * If you do, the size will be set to capacity of course.
*/
int set_capacityd(vector_d* vec, int size)
{
	if( size<vec->size )
		vec->size = size;

	vec->capacity = size;

	if( !(vec->a = realloc(vec->a, sizeof(double)*size)) ) {
		STDERR("Error allocating memory\n");
		return 0;
	}
	return 1;
}


/** Set all size elements to val. */
void set_val_szd(vector_d* vec, double val)
{
	int i;
	for(i=0; i<vec->size; i++)
		vec->a[i] = val;
}


/** Fills entire allocated array (capacity) with val. */
void set_val_capd(vector_d* vec, double val)
{
	int i;
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
void free_vecd(vector_d* vec)
{
	free(vec->a);
	free(vec);
}







/*  String vector */

/** Useful utility function since strdup isn't standard C.*/
char* mystrdup(const char* str)
{
	char *temp = calloc(strlen(str)+1, sizeof(char));
	if( temp == NULL )
		return NULL;
	
	strcpy(temp, str);
	return temp;
}



/**
 * Create a new vector_s with size sz.
 * Capacity is set to sz + VEC_S_START_SZ.
 * Note: vector_s does not copy pointers passed in but duplicates the strings
 * they point to (using mystrdup()) so you don't have to worry about freeing
 * or changing the contents of variables that you've pushed or inserted; it
 * won't affect the vector.
 */
vector_s* vec_s(int sz)
{
	vector_s* vec;
	if( !(vec = calloc(1, sizeof(vector_s))) ) {

		STDERR("Error allocating memory\n");
		return NULL;
	}

	vec->capacity = ( sz>0 ) ? sz + VEC_S_START_SZ : VEC_S_START_SZ;

	if( !(vec->a = calloc(vec->capacity, sizeof(char*))) ) {
		STDERR("Error allocating memory\n");

		return NULL;
	}

	vec->size = ( sz>0 ) ? sz : 0;
	return vec;
}


/** Create and initialize vector_s with num elements of vals.
 *  If vals is NULL, capacity is set to num + VEC_D_START_SZ.
 *  Size is set to num in the first place, 0 otherwise.
 */
vector_s* init_vec_s(char** vals, int num)
{
	vector_s* vec;
	if( !(vec = calloc(1, sizeof(vector_s))) ) {

		STDERR("Error allocating memory\n");

		return NULL;
	}

	if( vals && num>0 ) {
		vec->capacity = num+VEC_S_START_SZ;
		vec->size = num;
		if( !(vec->a = calloc(vec->capacity, sizeof(char*))) ) {
			STDERR("Error allocating memory\n");
			return NULL;
		}

		int i;
		for(i=0; i<num; i++)
			vec->a[i] = mystrdup(vals[i]);
		
	} else if( !vals && num>0 ) {
		vec->capacity = num+VEC_S_START_SZ;
		vec->size = 0;
		if( !(vec->a = calloc(vec->capacity, sizeof(char*))) ) {
			STDERR("Error allocating memory\n");
			return NULL;
		}
	} else
		return NULL;

	return vec;
}



/**
 * Append a to end of vector (size increased 1).
 * Capacity is increased by doubling when necessary.
 */
int push_backs(vector_s* vec, char* a)
{
	if( vec->capacity > vec->size ) {
		vec->a[vec->size++] = mystrdup(a);
	} else {
		vec->capacity *= 2;
		if( !(vec->a = realloc(vec->a, sizeof(char*)*vec->capacity)) ) {
			STDERR("Error allocating memory\n");
			return 0;
		}
		vec->a[vec->size++] = mystrdup(a);
	}
	return 1;
}

/** Remove the last element (size decreased 1).
 *	String is freed.  If ret is not NULL, strcpy the last element into ret.
 *	It is the user's responsibility to make sure ret can receive it without error. */
void pop_backs(vector_s* vec, char* ret)
{
	strcpy(ret, vec->a[--vec->size]);
	free(vec->a[vec->size]);
}

/**
 * Insert a at index i (0 based).
 * Everything from that index and right is shifted one to the right.
 *\todo check for i < 0 or > size ?
 */
int inserts(vector_s* vec, int i, char* a)
{
	if( vec->capacity > vec->size ) {
		memmove(&vec->a[i+1], &vec->a[i], (vec->size-i)*sizeof(char*));
		vec->a[i] = mystrdup(a);
	} else {
		vec->capacity *= 2;
		if( !(vec->a = realloc(vec->a, sizeof(char*)*vec->capacity)) ) {
			STDERR("Error allocating memory\n");
			return 0;
		}

		memmove(&vec->a[i+1], &vec->a[i], (vec->size-i)*sizeof(char*));
		vec->a[i] = mystrdup(a);
	}

	vec->size++;
	return 1;
}


/**
 * Erases strings from start to end inclusive.
 * Example erases(myvec, 1, 3) would free and remove strings at 1, 2, and 3 and the string
 * that was at index 4 would now be at 1 etc.
 */
void erases(vector_s* vec, int start, int end)
{
	int d = end-start+1;
	int i;
	for(i=start; i<=end; i++)
		free(vec->a[i]);
	
	memmove(&vec->a[start], &vec->a[end+1], (vec->size-1-end)*sizeof(int));
	vec->size -= d;
}





/** Makes sure the vector capacity is >= size (parameter not member). */
int reserves(vector_s* vec, int size)
{
	if( vec->capacity < size ) {
		if( !(vec->a = realloc(vec->a, sizeof(char*)*(size+20))) ) {
			STDERR("Error allocating memory\n");
			return 0;
		}
		vec->capacity = size+20;
	}
	return 1;
}

/** Set capacity to size.
 * You will lose data if you shrink the capacity below the current size.
 * If you do, the size will be set to capacity of course.
*/
int set_capacitys(vector_s* vec, int size)
{
	int i;
	if( size<vec->size ) {
		for(i=vec->size-1; i>size-1; i--)
			free(vec->a[i]);

		vec->size = size;
	}
	vec->capacity = size;

	if( !(vec->a = realloc(vec->a, sizeof(char*)*size)) ) {
		STDERR("Error allocating memory\n");
		return 0;
	}
	return 1;
}



/** Sets all size elements to val. */
void set_val_szs(vector_s* vec, char* val)
{
	int i;
	for(i=0; i<vec->size; i++) {
		free(vec->a[i]);		//not worth checking/reallocing to me
		vec->a[i] = mystrdup(val);
	}
}


/** Fills entire allocated array (capacity) with val.  Size is set
 * to capacity in this case because strings are individually dynamically allocated.
 * This is different from vector_i, vector_d and vector (without a free function) where the size stays the same. */
void set_val_caps(vector_s* vec, char* val)
{
	int i;
	for(i=0; i<vec->capacity; i++) {
		if( i<vec->size )
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
	for(i=0; i<vec->size; i++)
		free(vec->a[i]);
	
	vec->size = 0;
}


/** Frees contents (individual strings and array) and frees vector so don't use after calling this. */
void free_vecs(vector_s* vec)
{
	int i;
	for(i=0; i<vec->size; i++)
		free(vec->a[i]);
	
	free(vec->a);
	free(vec);
}






/*  general vector */

/**
 * Creates a new vector.
 * Vector size is set to sz, capacity to sz+VEC_I_START_SZ.
 * elem_sz is the size of the type you want to store ( ie sizeof(T) where T is your type ).
 * You can pass in a function, elem_free, to be called on every element before it is removed
 * from the vector to free any dynamically allocated memory.  For example if you passed
 * in sizeof(char*) for elem_sz and the standard free(void*) function for elem_free you could
 * make vector work exactly like vector_s.  Pass in NULL, to not use it.
 * See the other functions and the tests for more behavioral details.
 */
vector* vec(int sz, int elem_sz, void(*elem_free)(void*), void(*elem_init)(void*))
{
	vector* vec;
	if( !(vec = calloc(1, sizeof(vector))) ) {

		STDERR("Error allocating memory\n");
		return NULL;
	}

	vec->capacity = ( sz>0 ) ? sz + VEC_START_SZ : VEC_START_SZ;

	vec->elem_size = elem_sz;

	if( !(vec->a = calloc(vec->capacity, elem_sz)) ) {
		STDERR("Error allocating memory\n");

		return NULL;
	}

	vec->size = ( sz>0 ) ? sz : 0;
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
vector* init_vec(void* vals, int num, int elem_sz, void(*elem_free)(void*), void(*elem_init)(void*, void*))
{
	vector* vec;
	if( !(vec = calloc(1, sizeof(vector))) ) {

		STDERR("Error allocating memory\n");

		return NULL;
	}

	vec->elem_size = elem_sz;

	if( vals && num>0 ) {
		vec->capacity = num+VEC_START_SZ;
		vec->size = num;
		if( !(vec->a = calloc(vec->capacity, elem_sz)) ) {
			STDERR("Error allocating memory\n");
			return NULL;
		}

		memcpy(vec->a, vals, elem_sz*num);
	} else if( !vals && num>0 ) {
		vec->capacity = num+VEC_START_SZ;
		vec->size = 0;
		if( !(vec->a = calloc(vec->capacity, elem_sz)) ) {
			STDERR("Error allocating memory\n");
			return NULL;
		}
	} else
		return NULL;

	vec->elem_free = elem_free;
	vec->elem_init = elem_init;

	return vec;
}


/** Append a to end of vector (size increased 1).
 * Capacity is increased by doubling when necessary.
 */
int push_back(vector* vec, void* a)
{
	if( vec->capacity > vec->size ) {

		memcpy(&vec->a[vec->size*vec->elem_size], a, vec->elem_size);
		vec->size++;

	} else {
		vec->capacity *= 2;
		if( !(vec->a = realloc(vec->a, vec->elem_size*vec->capacity)) ) {
			STDERR("Error allocating memory\n");
			return 0;
		}

		memcpy(&vec->a[vec->size*vec->elem_size], a, vec->elem_size);
		vec->size++;
	}
	return 1;
}


/** Remove the last element (size decreased 1).
 * If ret is not NULL, copy the element into it.  This function assumes
 * that ret is large accept the element and just memcpy's it in. */
void pop_back(vector* vec, void* ret)
{
	if( ret!=NULL )
		memcpy(ret, &vec->a[(--vec->size)*vec->elem_size], vec->elem_size);
	else
		vec->size--;

	if( vec->elem_free )
		vec->elem_free(&vec->a[vec->size*vec->elem_size]);

}


/**
 * Insert a at index i (0 based).
 * Everything from that index and right is shifted one to the right.
 *\todo check for i < 0 or > size-1?
 */
int insert(vector* vec, int i, void* a)
{
	if( vec->capacity > vec->size ) {
		memmove(&vec->a[(i+1)*vec->elem_size], &vec->a[i*vec->elem_size], (vec->size-i)*vec->elem_size);
		memcpy(&vec->a[i*vec->elem_size], a, vec->elem_size);

	} else {
		vec->capacity *= 2;
		if( !(vec->a = realloc(vec->a, vec->elem_size*vec->capacity)) ) {
			STDERR("Error allocating memory\n");
			return 0;
		}

		memmove(&vec->a[(i+1)*vec->elem_size], &vec->a[i*vec->elem_size], (vec->size-i)*vec->elem_size);
		memcpy(&vec->a[i*vec->elem_size], a, vec->elem_size);
	}

	vec->size++;
	return 1;
}




/**
 * Erases elements from start to end inclusive.
 * Example erases(myvec, 1, 3) would free and remove elements at 1, 2, and 3 and the element
 * that was at index 4 would now be at 1 etc. \todo check start and end in range?
 */
void erase(vector* vec, int start, int end)
{
	int d = end-start+1;
	int i;
	if( vec->elem_free )
		for(i=start; i<=end; i++)
			vec->elem_free(&vec->a[i*vec->elem_size]);

	memmove(&vec->a[start*vec->elem_size], &vec->a[(end+1)*vec->elem_size], (vec->size-1-end)*vec->elem_size);
	vec->size -= d;
}


/** Makes sure capacity >= size (the parameter not the member). */
int reserve(vector* vec, int size)
{
	if( vec->capacity < size ) {
		if( !(vec->a = realloc(vec->a, vec->elem_size*(size+20))) ) {
			STDERR("Error allocating memory\n");
			return 0;
		}
		vec->capacity = size+20;
	}
	return 1;
}


/** Set capacity to size.
 * You will lose data if you shrink the capacity below the current size.
 * If you do, the size will be set to capacity of course.
*/
int set_capacity(vector* vec, int size)
{
	int i;
	if( size<vec->size ) {

		if( vec->elem_free )
			for(i=vec->size-1; i>=size; i--)
				vec->elem_free(&vec->a[i*vec->elem_size]);

		vec->size = size;
	}

	vec->capacity = size;

	if( !(vec->a = realloc(vec->a, vec->elem_size*size)) ) {
		STDERR("Error allocating memory\n");
		return 0;
	}
	return 1;
}



/** Set all size elements to val. */
void set_val_sz(vector* vec, void* val)
{
	int i;

	if( vec->elem_free )
		for(i=0; i<vec->size; i++)
			vec->elem_free(&vec->a[i*vec->elem_size]);

	if( vec->elem_init ) {
		for(i=0; i<vec->size; i++)
			vec->elem_init(&vec->a[i*vec->elem_size], val);
	} else {
		for(i=0; i<vec->size; i++)
			memcpy(&vec->a[i*vec->elem_size], val, vec->elem_size);
	}
}


/** Fills entire allocated array (capacity) with val.  If you set a free function
 * then size is set to capacity like vector_s for the same reason, ie I need to know
 * that the free function needs to be called on those elements. */
void set_val_cap(vector* vec, void* val)
{
	int i;
	if( vec->elem_free ) {
		for(i=0; i<vec->size; i++)
			vec->elem_free(&vec->a[i*vec->elem_size]);

		vec->size = vec->capacity;
	}

	if( vec->elem_init ) {
		for(i=0; i<vec->capacity; i++)
			vec->elem_init(&vec->a[i*vec->elem_size], val);
	} else {
		for(i=0; i<vec->capacity; i++)
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
void free_vec(vector* vec)
{
	int i;
	if( vec->elem_free )
		for(i=0; i<vec->size; i++)
			vec->elem_free(&vec->a[i*vec->elem_size]);

	free(vec->a);
	free(vec);
}













