CVECTOR
=======

[Download](https://github.com/rswinkle/cvector)

This is a relatively simple ANSI compliant C vector library with specific structures and
functions for int's, double's and string's and support for all other types
using a generic structure where the type is passed in as void\* and stored in a byte array
(to avoid dereferencing void\* warnings and frequent casting) .
The generic vector is very flexible and allows you to provide free and init functions
if you like that it will call at appropriate times similar to the way C++ containers
will call destructors and copy constructors.

Other modifiable parameters are at the top of the respective cvector.c's

	cvec_sz CVEC_I_START_SZ = 50;
	cvec_sz CVEC_D_START_SZ = 50;
	cvec_sz CVEC_STR_START_SZ = 20;
	cvec_sz CVEC_VOID_START_SZ = 20;

	#define CVEC_I_ALLOCATOR(x) ((x+1) * 2)
	#define CVEC_D_ALLOCATOR(x) ((x+1) * 2)
	#define CVEC_STR_ALLOCATOR(x) ((x+1) * 2)
	#define CVEC_VOID_ALLOCATOR(x) ((x+1) * 2)

The allocator macros are used in all functions that increase the size by 1.
In others (constructors, insert_array, reserve) CVEC_X_START_SZ is the amount
extra allocated.

Note that the (x+1) portion allows you to use the non-void vectors
without calling any of the init functions first *if* you zero them out.  This
means size, capacity, and a are 0/NULL which is valid because realloc, acts like
malloc when given a NULL pointer.  With cvector_void you still have to set
elem_size, and optionally elem_free/elem_init. See the zero_init_x_test()'s
in cvector_tests.c for example of that use.

The `cvec_sz` type defaults to `size_t` but if you define CVEC_SIZE_T before including
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

cvector_short and cvector_f_struct are examples of the generated files.

Building
========
I use [premake](http://premake.github.io/) generated makefiles which are
included in the build subdirectory.  However if you modified premake4.lua
the command to regenerate them is `premake4 gmake`.  cd into the build
directory and run `make` or `make config=release`. I have not tried it on
windows though it should work (well I'm not sure about CUnit ...).

There is no output of any kind, no errors or warnings.

It has been relatively well tested using CUnit tests which all pass.
I've also run it under valgrind and there are no memory leaks.

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


I plan to continue to improve/modify it but probably only in minor ways and
only if I think something should be changed.  This library is mostly
for my own use but I thought I would share it and use it as a platform
to learn CUnit and Doxygen.

You can probably get Cunit from your package manager but
if you want to get the most up to date version of CUnit go here:

http://cunit.sourceforge.net/index.html
http://sourceforge.net/projects/cunit/

I'm using version 2.1-3.

Usage
=====
To actually use the library just copy the appropriate c/h file pair(s) to your project
or just use cvector.h.  To get a good idea of how to use the library and see it in
action and how it should behave, you can look at cvector_tests.c but for more practical
examples, you can look at my other projects that use cvector like [C_Interpreter](https://github.com/rswinkle/c_interpreter),
[CPIM2](https://github.com/rswinkle/cpim2), [sdl_img](https://github.com/rswinkle/sdl_img),
[c_bigint](https://github.com/rswinkle/c_bigint) and [spelling_game](https://github.com/rswinkle/spelling_game).

Documentation
=============
The Doxygen documentation is generated with the command

	doxygen Doxyfile

in the root directory.  The Doxygen generated html docs are
[online here](http://www.robertwinkler.com/projects/cvector/)

Test Results
============
The automated Travis build also runs the tests and fails if any tests fail.
If you want a pretty visualization of the test results (updated with every release
not every commit), you can see it
[here](http://www.robertwinkler.com/projects/cvector/CUnitAutomated-Results.xml).

LICENSE
=======
CVector is licensed under the MIT License.

Copyright (c) 2011-2024 Robert Winkler

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

