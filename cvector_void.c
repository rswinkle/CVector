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

size_t CVEC_VOID_START_SZ = 20;


#define CVEC_VOID_ALLOCATOR(x) ((x+1) * 2)


/*  general vector */

/**
 * Creates a new vector on the heap.
 * Vector size set to (size > 0) ? size : 0;
 * Capacity to (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + CVEC_VOID_START_SZ
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
cvector_void* cvec_void_heap(size_t size, size_t capacity, size_t elem_sz, void(*elem_free)(void*), void(*elem_init)(void*, void*))
{
	cvector_void* vec;
	if (!(vec = (cvector_void*)CVEC_MALLOC(sizeof(cvector_void)))) {
		CVEC_ASSERT(vec != NULL);
		return NULL;
	}

	vec->size = size;
	vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + CVEC_VOID_START_SZ;

	vec->elem_size = elem_sz;
	
	if (!(vec->a = (byte*)CVEC_MALLOC(vec->capacity*elem_sz))) {
		CVEC_ASSERT(vec->a != NULL);
		CVEC_FREE(vec);
		return NULL;
	}
	/* not clearing to 0 here as in vector_str because elem_free cannot be calling CVEC_FREE directly
	 * since it takes the address of the element not the element itself */

	vec->elem_free = elem_free;
	vec->elem_init = elem_init;

	return vec;
}



/** Create (on the heap) and initialize vector with num elements of vals.
 *  elem_sz is the size of the type you want to store ( ie sizeof(T) where T is your type ).
 *  See cvec_void_heap() for more information about the elem_free and elem_init parameters.
 */
cvector_void* cvec_init_void_heap(void* vals, size_t num, size_t elem_sz, void(*elem_free)(void*), void(*elem_init)(void*, void*))
{
	cvector_void* vec;
	size_t i;
	
	if (!(vec = (cvector_void*)CVEC_MALLOC(sizeof(cvector_void)))) {
		CVEC_ASSERT(vec != NULL);
		return NULL;
	}

	vec->elem_size = elem_sz;

	vec->capacity = num + CVEC_VOID_START_SZ;
	vec->size = num;
	if (!(vec->a = (byte*)CVEC_MALLOC(vec->capacity*elem_sz))) {
		CVEC_ASSERT(vec->a != NULL);
		CVEC_FREE(vec);
		return NULL;
	}

	if (elem_init) {
		for (i=0; i<num; ++i) {
			elem_init(&vec->a[i*elem_sz], &((byte*)vals)[i*elem_sz]);
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
int cvec_void(cvector_void* vec, size_t size, size_t capacity, size_t elem_sz, void(*elem_free)(void*), void(*elem_init)(void*, void*))
{
	vec->size = size;
	vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + CVEC_VOID_START_SZ;

	vec->elem_size = elem_sz;
	
	if (!(vec->a = (byte*)CVEC_MALLOC(vec->capacity*elem_sz))) {
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
int cvec_init_void(cvector_void* vec, void* vals, size_t num, size_t elem_sz, void(*elem_free)(void*), void(*elem_init)(void*, void*))
{
	size_t i;
	
	vec->elem_size = elem_sz;

	vec->capacity = num + CVEC_VOID_START_SZ;
	vec->size = num;
	if (!(vec->a = (byte*)CVEC_MALLOC(vec->capacity*elem_sz))) {
		CVEC_ASSERT(vec->a != NULL);
		vec->size = vec->capacity = 0;
		return 0;
	}

	if (elem_init) {
		for (i=0; i<num; ++i) {
			elem_init(&vec->a[i*elem_sz], &((byte*)vals)[i*elem_sz]);
		}
	} else {
		CVEC_MEMMOVE(vec->a, vals, elem_sz*num);
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
void cvec_void_copy(void* dest, void* src)
{
	size_t i;
	cvector_void* vec1 = (cvector_void*)dest;
	cvector_void* vec2 = (cvector_void*)src;
	
	vec1->size = 0;
	vec1->capacity = 0;
	
	/*not much else we can do here*/
	if (!(vec1->a = (byte*)CVEC_MALLOC(vec2->capacity*vec2->elem_size))) {
		CVEC_ASSERT(vec1->a != NULL);
		return;
	}

	vec1->size = vec2->size;
	vec1->capacity = vec2->capacity;
	vec1->elem_size = vec2->elem_size;
	vec1->elem_init = vec2->elem_init;
	vec1->elem_free = vec2->elem_free;
	
	if (vec1->elem_init) {
		for (i=0; i<vec1->size; ++i) {
			vec1->elem_init(&vec1->a[i*vec1->elem_size], &vec2->a[i*vec1->elem_size]);
		}
	} else {
		CVEC_MEMMOVE(vec1->a, vec2->a, vec1->size*vec1->elem_size);
	}
}


/** Append a to end of vector (size increased 1).
 * Capacity is increased by doubling when necessary.
 */
int cvec_push_void(cvector_void* vec, void* a)
{
	byte* tmp;
	size_t tmp_sz;
	if (vec->capacity == vec->size) {
		tmp_sz = CVEC_VOID_ALLOCATOR(vec->capacity);
		if (!(tmp = (byte*)CVEC_REALLOC(vec->a, vec->elem_size*tmp_sz))) {
			CVEC_ASSERT(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}
	if (vec->elem_init) {
		vec->elem_init(&vec->a[vec->size*vec->elem_size], a);
	} else {
		CVEC_MEMMOVE(&vec->a[vec->size*vec->elem_size], a, vec->elem_size);
	}
	
	vec->size++;
	return 1;
}


/** Remove the last element (size decreased 1).
 * Copy the element into ret.  This function assumes
 * that ret is not NULL and is large accept the element and just CVEC_MEMMOVE's it in.
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

/** Return pointer to last element */
void* cvec_back_void(cvector_void* vec)
{
	return &vec->a[(vec->size-1)*vec->elem_size];
}




/** Increase the size of the array num items.  Items
 *  are not initialized to anything! */
int cvec_extend_void(cvector_void* vec, size_t num)
{
	byte* tmp;
	size_t tmp_sz;
	if (vec->capacity < vec->size + num) {
		tmp_sz = vec->capacity + num + CVEC_VOID_START_SZ;
		if (!(tmp = (byte*)CVEC_REALLOC(vec->a, vec->elem_size*tmp_sz))) {
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
  * #define GET_ELEMENT(X,Y,TYPE) ((TYPE*)&X.a[Y*X.elem_size])
*/
void* cvec_get_void(cvector_void* vec, size_t i)
{
	return &vec->a[i*vec->elem_size];
}



/**
 * Insert a at index i (0 based).
 * Everything from that index and right is shifted one to the right.
 */
int cvec_insert_void(cvector_void* vec, size_t i, void* a)
{
	byte* tmp;
	size_t tmp_sz;
	if (vec->capacity == vec->size) {
		tmp_sz = CVEC_VOID_ALLOCATOR(vec->capacity);
		if (!(tmp = (byte*)CVEC_REALLOC(vec->a, vec->elem_size*tmp_sz))) {
			CVEC_ASSERT(tmp != NULL);
			return 0;
		}
		
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}
	CVEC_MEMMOVE(&vec->a[(i+1)*vec->elem_size], &vec->a[i*vec->elem_size], (vec->size-i)*vec->elem_size);

	if (vec->elem_init) {
		vec->elem_init(&vec->a[i*vec->elem_size], a);
	} else {
		CVEC_MEMMOVE(&vec->a[i*vec->elem_size], a, vec->elem_size);
	}

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
int cvec_insert_array_void(cvector_void* vec, size_t i, void* a, size_t num)
{
	byte* tmp;
	size_t tmp_sz, j;
	if (vec->capacity < vec->size + num) {
		tmp_sz = vec->capacity + num + CVEC_VOID_START_SZ;
		if (!(tmp = (byte*)CVEC_REALLOC(vec->a, vec->elem_size*tmp_sz))) {
			CVEC_ASSERT(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}

	CVEC_MEMMOVE(&vec->a[(i+num)*vec->elem_size], &vec->a[i*vec->elem_size], (vec->size-i)*vec->elem_size);
	if (vec->elem_init) {
		for (j=0; j<num; ++j) {
			vec->elem_init(&vec->a[(j+i)*vec->elem_size], &((byte*)a)[j*vec->elem_size]);
		}
	} else {
		CVEC_MEMMOVE(&vec->a[i*vec->elem_size], a, num*vec->elem_size);
	}
	vec->size += num;
	return 1;
}

/**
 * Replace value at i with a, return old value in ret if non-NULL.
 */
void cvec_replace_void(cvector_void* vec, size_t i, void* a, void* ret)
{
	if (ret)
		CVEC_MEMMOVE(ret, &vec->a[i*vec->elem_size], vec->elem_size);
	CVEC_MEMMOVE(&vec->a[i*vec->elem_size], a, vec->elem_size);
}

/**
 * Erases elements from start to end inclusive.
 * Example cvec_erase_void(myvec, 1, 3) would CVEC_FREE (if an elem_free function was provided) and remove elements at 1, 2, and 3 and the element
 * that was at index 4 would now be at 1 etc.
 */
void cvec_erase_void(cvector_void* vec, size_t start, size_t end)
{
	size_t i;
	size_t d = end - start + 1;
	if (vec->elem_free) {
		for (i=start; i<=end; i++) {
			vec->elem_free(&vec->a[i*vec->elem_size]);
		}
	}
	CVEC_MEMMOVE(&vec->a[start*vec->elem_size], &vec->a[(end+1)*vec->elem_size], (vec->size-1-end)*vec->elem_size);
	vec->size -= d;
}


/** Makes sure capacity >= size (the parameter not the member). */
int cvec_reserve_void(cvector_void* vec, size_t size)
{
	byte* tmp;
	if (vec->capacity < size) {
		if (!(tmp = (byte*)CVEC_REALLOC(vec->a, vec->elem_size*(size+CVEC_VOID_START_SZ)))) {
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
int cvec_set_cap_void(cvector_void* vec, size_t size)
{
	size_t i;
	byte* tmp;
	if (size < vec->size) {
		if (vec->elem_free) {
			for (i=vec->size-1; i>=size; i--) {
				vec->elem_free(&vec->a[i*vec->elem_size]);
			}
		}
		vec->size = size;
	}

	vec->capacity = size;

	if (!(tmp = (byte*)CVEC_REALLOC(vec->a, vec->elem_size*size))) {
		CVEC_ASSERT(tmp != NULL);
		return 0;
	}
	vec-> a = tmp;
	return 1;
}



/** Set all size elements to val. */
void cvec_set_val_sz_void(cvector_void* vec, void* val)
{
	size_t i;

	if (vec->elem_free) {
		for(i=0; i<vec->size; i++) {
			vec->elem_free(&vec->a[i*vec->elem_size]);
		}
	}
	
	if (vec->elem_init) {
		for (i=0; i<vec->size; i++) {
			vec->elem_init(&vec->a[i*vec->elem_size], val);
		}
	} else {
		for (i=0; i<vec->size; i++) {
			CVEC_MEMMOVE(&vec->a[i*vec->elem_size], val, vec->elem_size);
		}
	}
}


/** Fills entire allocated array (capacity) with val.  If you set a CVEC_FREE function
 * then size is set to capacity like vector_s for the same reason, ie I need to know
 * that the CVEC_FREE function needs to be called on those elements.
 * TODO Remove this function?  Same reason as set_val_caps.
 */
void cvec_set_val_cap_void(cvector_void* vec, void* val)
{
	size_t i;
	if (vec->elem_free) {
		for (i=0; i<vec->size; i++) {
			vec->elem_free(&vec->a[i*vec->elem_size]);
		}
		vec->size = vec->capacity;
	}

	if (vec->elem_init) {
		for (i=0; i<vec->capacity; i++) {
			vec->elem_init(&vec->a[i*vec->elem_size], val);
		}
	} else {
		for (i=0; i<vec->capacity; i++) {
			CVEC_MEMMOVE(&vec->a[i*vec->elem_size], val, vec->elem_size);
		}
	}
}


/** Sets size to 0 (does not change contents unless elem_free is set
 *  then it will CVEC_FREE all size elements as in vector_s). */
void cvec_clear_void(cvector_void* vec)
{
	size_t i;
	if (vec->elem_free) {
		for (i=0; i<vec->size; ++i) {
			vec->elem_free(&vec->a[i*vec->elem_size]);
		}
	}
	vec->size = 0;
}


/** Frees everything so don't use vec after calling this. If you set a CVEC_FREE function
 * it will be called on all size elements of course. */
void cvec_free_void_heap(void* vec)
{
	size_t i;
	cvector_void* tmp = (cvector_void*)vec;
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
	size_t i;
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
using a generic structure where the type is passed in as void* and stored in a byte array
(to avoid dereferencing void* warnings and frequent casting) .
The generic vector is very flexible and allows you to provide CVEC_FREE and init functions
if you like that it will call at appropriate times similar to the way C++ containers
will call destructors.

Other modifiable parameters are at the top of the respective cvector.c's
<pre>
size_t CVEC_I_START_SZ = 50;
size_t CVEC_D_START_SZ = 50;
size_t CVEC_STR_START_SZ = 20;
size_t CVEC_VOID_START_SZ = 20;

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

There are also 2 templates, one for basic types and one for types that contain
dynamically allocated memory and you might want a CVEC_FREE and/or init function.
In other words the first template is based off cvector_i and the second is based
off of cvector_void, so look at the corresponding documentation for behavior.

There are 2 ways to use/create your own cvector types.  The easiest way is to use
the macros defined in cvector_macro.h which are also included in the all-in-one header
cvector.h.  You can see how to use them in cvector_tests.c:

	#define RESIZE(a) ((a)*2)

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

cvector_short and cvector_f_struct are examples of the generated files.  While I
now test the macros instead of the files, it's the same code, and you can still
see how I used to test them.


\section des_notes Design Notes
Memory allocations are checked and asserted.  If not in debug mode (ie NDEBUG is defined)
0 is returned on allocation failure.

No other error checking is performed.  If you pass bad parameters, bad things will probably happen.
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
I use premake to generate the make files in the build directory.  The command is premake5 gmake.
cd into build and run make or make config=release.  I have not tried it on windows though
it should work (well I'm not sure about CUnit ...).

There is no output of any kind, no errors or warnings.


It has been relatively well tested using CUnit tests which all pass.
I've also run it under valgrind and there are no memory leaks.

<pre>
valgrind --leak-check=full -v ./cvector

==4682== 
==4682== HEAP SUMMARY:
==4682==     in use at exit: 0 bytes in 0 blocks
==4682==   total heap usage: 6,466 allocs, 6,466 frees, 936,809 bytes allocated
==4682== 
==4682== All heap blocks were freed -- no leaks are possible
==4682== 
==4682== For counts of detected and suppressed errors, rerun with: -v
==4682== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
</pre>

You can probably get Cunit from your package manager but
if you want to get the most up to date version of CUnit go here:

http://cunit.sourceforge.net/index.html
http://sourceforge.net/projects/cunit/

I'm using version 2.1-3.


\section Usage
To actually use the library just copy the appropriate c/h file pair(s) to your project
or just use cvector.h.
To get a good idea of how to use the library and see it in action and how it should
behave, look at cvector_tests.c

\section LICENSE
CVector is licensed under the MIT License.

Copyright (c) 2011-2016 Robert Winkler

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






