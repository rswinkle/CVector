#include "vector_void.h"

#define STDERR(X) fprintf(stderr, X)


size_t VEC_START_SZ = 20;


#define VEC_ALLOCATOR(x) (x*2)






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
vector_void* vec_void_heap(size_t size, size_t capacity, size_t elem_sz, void(*elem_free)(void*), void(*elem_init)(void*, void*))
{
	vector_void* vec;
	if (!(vec = malloc(sizeof(vector_void)))) {
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
vector_void* init_vec_void_heap(void* vals, size_t num, size_t elem_sz, void(*elem_free)(void*), void(*elem_init)(void*, void*))
{
	vector_void* vec;
	size_t i;
	
	if (!vals || num < 1)
		return NULL;
	
	if (!(vec = malloc(sizeof(vector_void)))) {
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
int vec_void(vector_void* vec, size_t size, size_t capacity, size_t elem_sz, void(*elem_free)(void*), void(*elem_init)(void*, void*))
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
int init_vec_void(vector_void* vec, void* vals, size_t num, size_t elem_sz, void(*elem_free)(void*), void(*elem_init)(void*, void*))
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
 *  a vector of generic vector's. (I would recommend against doing that, and using 
 *  generate_code.py to make your own vector type and do a vector of those
 *  instead).  Assumes dest (the structure)
 *  is already allocated (probably on the stack) and that
 *  capacity is 0 (ie the array doesn't need to be freed).
 */
void vec_void_copy(void* dest, void* src)
{
	size_t i;
	vector_void* vec1 = dest;
	vector_void* vec2 = src;
	
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
int push_void(vector_void* vec, void* a)
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
void pop_void(vector_void* vec, void* ret)
{
	if (ret)
		memcpy(ret, &vec->a[(--vec->size)*vec->elem_size], vec->elem_size);
	else
		vec->size--;

	if (vec->elem_free)
		vec->elem_free(&vec->a[vec->size*vec->elem_size]);
}

/** Return pointer to last element */
void* back_void(vector_void* vec)
{
	return &vec->a[(vec->size-1)*vec->elem_size];
}




/** Increase the size of the array num items.  Items
 *  are not initialized to anything! */
int extend_void(vector_void* vec, size_t num)
{
	void* tmp;
	size_t tmp_sz;
	if (vec->capacity < vec->size + num) {
		tmp_sz = vec->capacity + num + VEC_START_SZ;
		if (!(tmp = realloc(vec->a, vec->elem_size*tmp_sz))) {
			STDERR("Error allocating memory\n");
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
  * #define GET_ELEMENT(X,Y,TYPE) ((TYPE*)&X.a[Y*X.elem_size])
*/
void* vec_void_get(vector_void* vec, size_t i)
{
	return &vec->a[i*vec->elem_size];
}



/**
 * Insert a at index i (0 based).
 * Everything from that index and right is shifted one to the right.
 *\todo check for i < 0 or > size-1?
 */
int insert_void(vector_void* vec, size_t i, void* a)
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
int insert_array_void(vector_void* vec, size_t i, void* a, size_t num)
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
void erase_void(vector_void* vec, size_t start, size_t end)
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
int reserve_void(vector_void* vec, size_t size)
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
int set_capacity_void(vector_void* vec, size_t size)
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
void set_val_sz_void(vector_void* vec, void* val)
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
void set_val_cap_void(vector_void* vec, void* val)
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


/** Sets size to 0 (does not change contents unless elem_free is set
 *  then it will free all size elements as in vector_s). */
void clear_void(vector_void* vec)
{
	size_t i;
	if (vec->elem_free) {
		for (i=0; i<vec->size; ++i) {
			vec->elem_free(&vec->a[i*vec->elem_size]);
		}
	}
	vec->size = 0;
}


/** Frees everything so don't use vec after calling this. If you set a free function
 * it will be called on all size elements of course. */
void free_vec_void_heap(void* vec)
{
	size_t i;
	vector_void* tmp = vec;
	if (tmp->elem_free)
		for (i=0; i<tmp->size; i++)
			tmp->elem_free(&tmp->a[i*tmp->elem_size]);

	free(tmp->a);
	free(tmp);
}


/** Frees the internal array and sets size and capacity to 0 */
void free_vec_void(void* vec)
{
	size_t i;
	vector_void* tmp = vec;
	if (tmp->elem_free)
		for (i=0; i<tmp->size; i++)
			tmp->elem_free(&tmp->a[i*tmp->elem_size]);

	free(tmp->a);

	tmp->size = 0;
	tmp->capacity = 0;
}



/*! \mainpage CVector notes
 *

\section Intro
This is a relatively simple ANSI compliant C vector library with specific structures and
functions for int's, double's and string's and support for all other types
using a generic structure where the type is passed in as void* and stored in a byte array
(to avoid dereferencing void* warnings and frequent casting) .
The generic vector is very flexible and allows you to provide free and init functions 
if you like that it will call at appropriate times similar to the way C++ containers
will call destructors.

Other modifiable parameters are at the top of vector.c
<pre>
size_t VEC_I_START_SZ = 50;
size_t VEC_D_START_SZ = 50;
size_t VEC_START_SZ = 20;
size_t VEC_S_START_SZ = 20;

#define VECI_ALLOCATOR(x) (x*2)
#define VECD_ALLOCATOR(x) (x*2)
#define VECS_ALLOCATOR(x) (x*2)
#define VEC_ALLOCATOR(x) (x*2)
</pre>
The allocator macros are used in all functions that increase the size by 1.
In others (constructors, insert_array, reserve) VEC_X_START_SZ is the amount
extra allocated.


With version 2.0 I've added vector_template.c and vector_template.h which are
used to generate code for any type (that doesn't require individual allocation/freeing
like vector_s).  It behaves exactly like vector_i (or d).  This is preferable to using
the generic vector for simple types and basic structures etc. since it's faster and clearer.

To use generate your own c and h file for a type just run:
<pre>
python3 generate_code.py yourtype
</pre>

which will generate vector_yourtype.c and vector_yourtype.h

vector_short is an example of the process and how to add it to the testing.



\section Building
I use premake so the command on linux is premake4 gmake which
will generate a build directory.  cd into that and run make
or make config=release.  I have not tried it on windows though
it should work (well I'm not sure about CUnit ...).

There is no output of any kind, no errors or warnings.


It has been relatively well tested using Cunit tests which all pass.
I've also run it under valgrind and there are no memory leaks.

valgrind --leak-check=yes ./vector

<pre>
==12365== HEAP SUMMARY:
==12365==     in use at exit: 0 bytes in 0 blocks
==12365==   total heap usage: 4,957 allocs, 4,957 frees, 797,993 bytes allocated
==12365== 
==12365== All heap blocks were freed -- no leaks are possible
==12365== 
==12365== For counts of detected and suppressed errors, rerun with: -v
==12365== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
</pre>

You can probably get Cunit from your package manager but
if you want to get the most up to date version of CUnit go here:

http://cunit.sourceforge.net/index.html
http://sourceforge.net/projects/cunit/

I used version 2.1-2.


\section Usage
To actually use the library just copy vector.c and vector.h to your project.
Also copy in generated types to your project as well.
To get a good idea of how to use the library and see it in action and how it should
behave, look at vector_tests.c

\section LICENSE
CVector is licensed under the MIT License.
Copyright (c) 2011-2012 Robert Winkler

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






