#include "vector.h"

#define STDERR(X) fprintf(stderr, X)


int VEC_I_START_SZ = 50;
int VEC_D_START_SZ = 50;
int VEC_START_SZ = 20;
int VEC_S_START_SZ = 20;


vector_i* vec_i(int sz)
{
	vector_i* vec;
	if( !(vec = calloc(1, sizeof(vector_i))) ) {

		STDERR("Error allocating memory\n");
		return NULL;
	}

	vec->capacity = ( sz ) ? sz + VEC_I_START_SZ : VEC_I_START_SZ;

	if( !(vec->a = calloc(vec->capacity, sizeof(int))) ) {
		STDERR("Error allocating memory\n");

		return NULL;
	}

	vec->size = sz;
	return vec;
}

// Initialize vector with contents of val (num elements)
// or if vals is NULL set capacity to num+50
vector_i* init_vec_i(int* vals, int num)
{
	vector_i* vec;
	if( !(vec = calloc(1, sizeof(vector_i))) ) {

		STDERR("Error allocating memory\n");

		return NULL;
	}

	if( vals && num>0 ) {
		vec->capacity = num+50;
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


int pop_backi(vector_i* vec)
{
	return vec->a[--vec->size];
}


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


void erasei(vector_i* vec, int start, int end)
{
	int d = end-start+1;
	memmove(&vec->a[start], &vec->a[end+1], (vec->size-1-end)*sizeof(int));
	vec->size -= d;
}

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


void set_val_szi(vector_i* vec, int val)
{
	int i;
	for(i=0; i<vec->size; i++)
		vec->a[i] = val;
}

void set_val_capi(vector_i* vec, int val)
{
	int i;
	for(i=0; i<vec->capacity; i++)
		vec->a[i] = val;
}



int capacityi(vector_i* vec) { return vec->capacity; }
int sizei(vector_i* vec) { return vec->size; }
void cleari(vector_i* vec) { vec->size = 0; }
void free_veci(vector_i* vec)
{
	free(vec->a);
	free(vec);
}




vector_d* vec_d(int sz)
{
	vector_d* vec;
	if( !(vec = calloc(1, sizeof(vector_d))) ) {

		STDERR("Error allocating memory\n");
		return NULL;
	}

	vec->capacity = ( sz ) ? sz + VEC_D_START_SZ : VEC_D_START_SZ;

	if( !(vec->a = calloc(vec->capacity, sizeof(double))) ) {
		STDERR("Error allocating memory\n");
		return NULL;
	}

	vec->size = sz;
	return vec;
}


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



double pop_backd(vector_d* vec)
{
	return vec->a[--vec->size];
}

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


void erased(vector_d* vec, int start, int end)
{
	int d = end-start+1;
	memmove(&vec->a[start], &vec->a[end+1], (vec->size-1-end)*sizeof(double));
	vec->size -= d;
}


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

void set_val_szd(vector_d* vec, double val)
{
	int i;
	for(i=0; i<vec->size; i++)
		vec->a[i] = val;
}

void set_val_capd(vector_d* vec, double val)
{
	int i;
	for(i=0; i<vec->capacity; i++)
		vec->a[i] = val;
}




int capacityd(vector_d* vec) { return vec->capacity; }
int sized(vector_d* vec) { return vec->size; }
void cleard(vector_d* vec) { vec->size = 0; }

void free_vecd(vector_d* vec)
{
	free(vec->a);
	free(vec);
}







/*  String vector */

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
 * Capacity is set to sz + VEC_S_START_SZ
 */
vector_s* vec_s(int sz)
{
	vector_s* vec;
	if( !(vec = calloc(1, sizeof(vector_s))) ) {

		STDERR("Error allocating memory\n");
		return NULL;
	}

	vec->capacity = ( sz ) ? sz + VEC_S_START_SZ : VEC_S_START_SZ;

	if( !(vec->a = calloc(vec->capacity, sizeof(char*))) ) {
		STDERR("Error allocating memory\n");

		return NULL;
	}

	vec->size = sz;
	return vec;
}


/**
 * Create new vector_s and initialize with contents of vals (num elements).
 * If vals is NULL set capacity to num+50
 */
vector_s* init_vec_s(char** vals, int num)
{
	vector_s* vec;
	if( !(vec = calloc(1, sizeof(vector_s))) ) {

		STDERR("Error allocating memory\n");

		return NULL;
	}

	if( vals && num>0 ) {
		vec->capacity = num+50;
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
 * Push back a new string.
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

/**
 * Pop back a string.
 */
void pop_backs(vector_s* vec)
{
	free(vec->a[--vec->size]);
}

/**
 * Insert a string at index i.
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




/**
 * Makes sure the vector size is >= size.
 */
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


/**
 * Sets all size elements to val.
 */
void set_val_szs(vector_s* vec, char* val)
{
	int i;
	for(i=0; i<vec->size; i++) {
		free(vec->a[i]);		//not worth checking/reallocing to me
		vec->a[i] = mystrdup(val);
	}
}

/**
 * Sets the entire array (capacity >= size) to val.
 */
void set_val_caps(vector_s* vec, char* val)
{
	int i;
	for(i=0; i<vec->capacity; i++) {
		free(vec->a[i]);
		vec->a[i] = strdup(val);
	}
}



int capacitys(vector_s* vec) { return vec->capacity; }
int sizes(vector_s* vec) { return vec->size; }

/**
 * Clears the contents of vector.
 */
void clears(vector_s* vec)
{
	int i;
	for(i=0; i<vec->size; i++)
		free(vec->a[i]);
	
	vec->size = 0;
}

/**
 * Clears contents and frees vector
 */
void free_vecs(vector_s* vec)
{
	int i;
	for(i=0; i<vec->size; i++)
		free(vec->a[i]);
	
	free(vec->a);
	free(vec);
}






/*  general vector */

vector* vec(int sz, int elem_sz)
{
	vector* vec;
	if( !(vec = calloc(1, sizeof(vector))) ) {

		STDERR("Error allocating memory\n");
		return NULL;
	}

	vec->capacity = ( sz ) ? sz + VEC_START_SZ : VEC_START_SZ;

	vec->elem_size = elem_sz;

	if( !(vec->a = calloc(vec->capacity, elem_sz)) ) {
		STDERR("Error allocating memory\n");

		return NULL;
	}

	vec->size = sz;
	return vec;
}



// Initialize vector with contents of val (num elements)
// or if vals is NULL set capacity to num+VEC_START_SZ
vector* init_vec(void* vals, int num, int elem_sz)
{
	vector* vec;
	if( !(vec = calloc(1, sizeof(vector))) ) {

		STDERR("Error allocating memory\n");

		return NULL;
	}

	vec->elem_size = elem_sz;

	if( vals && num>0 ) {
		vec->capacity = num+50;
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

	return vec;
}



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


//maybe I should add a return parameter, check for null etc.
void pop_back(vector* vec)
{
	vec->size--;
	//return &vec->a[(--vec->size)*vec->elem_size];
}



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





void erase(vector* vec, int start, int end)
{
	int d = end-start+1;
	memmove(&vec->a[start*vec->elem_size], &vec->a[(end+1)*vec->elem_size], (vec->size-1-end)*vec->elem_size);
	vec->size -= d;
}

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


void set_val_sz(vector* vec, void* val)
{
	int i;
	for(i=0; i<vec->size; i++)
		memcpy(&vec->a[i*vec->elem_size], val, vec->elem_size);
}

void set_val_cap(vector* vec, void* val)
{
	int i;
	for(i=0; i<vec->capacity; i++)
		memcpy(&vec->a[i*vec->elem_size], val, vec->elem_size);
}



int capacity(vector* vec) { return vec->capacity; }
int size(vector* vec) { return vec->size; }
void clear(vector* vec) { vec->size = 0; }
void free_vec(vector* vec)
{
	free(vec->a);
	free(vec);
}













