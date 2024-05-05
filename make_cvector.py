import sys, os, glob, re

# In the middle of the night I get really lazy, even though I already had it hard coded
# https://stackoverflow.com/questions/1883980/find-the-nth-occurrence-of-substring-in-a-string 
def find_nth(haystack, needle, n):
	start = haystack.find(needle)
	while start >= 0 and n > 1:
		start = haystack.find(needle, start+len(needle))
		n -= 1
	return start

def get_header(filename):
	header_text = open(filename).read()
	# 3rd because 1st is the CVEC_SIZE_T #endif, 2nd is CVEC_SZ #endif
	start = find_nth(header_text, "#endif", 3) + 6 # #endif of extern "C"
	end = header_text.find("#ifdef", start) #ifdef close of extern "C"
	return header_text[start:end]

def get_c_file(filename):
	c_text = open(filename).read()
	return c_text[c_text.find("cvec_sz"):]


cvector_str = """

/* header starts */

#ifndef CVECTOR_H
#define CVECTOR_H

#if defined(CVEC_ONLY_INT) || defined(CVEC_ONLY_DOUBLE) || defined(CVEC_ONLY_STR) \
|| defined(CVEC_ONLY_VOID)
   #ifndef CVEC_ONLY_INT
   #define CVEC_NO_INT
   #endif
   #ifndef CVEC_ONLY_DOUBLE
   #define CVEC_NO_DOUBLE
   #endif
   #ifndef CVEC_ONLY_STR
   #define CVEC_NO_STR
   #endif
   #ifndef CVEC_ONLY_VOID
   #define CVEC_NO_VOID
   #endif
#endif

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

#ifndef CVEC_SIZE_T
#include <stdlib.h>
#define CVEC_SIZE_T size_t
#endif

#ifndef CVEC_SZ
#define CVEC_SZ
typedef CVEC_SIZE_T cvec_sz;
#endif

#ifdef __cplusplus
extern "C" {
#endif

"""

cvector_str += "#ifndef CVEC_NO_INT\n\n"
cvector_str += get_header("cvector_i.h")
cvector_str += "#endif\n\n"

cvector_str += "#ifndef CVEC_NO_DOUBLE\n\n"
cvector_str += get_header("cvector_d.h")
cvector_str += "#endif\n\n"

cvector_str += "#ifndef CVEC_NO_STR\n\n"
cvector_str += get_header("cvector_str.h")
cvector_str += "#endif\n\n"

cvector_str += "#ifndef CVEC_NO_VOID\n\n"
cvector_str += get_header("cvector_void.h")
cvector_str += "#endif\n\n"


cvector_str += """
#ifdef __cplusplus
}
#endif


"""

c_text = open("cvector_macro.h").read()
cvector_str += c_text[c_text.find("#define CVEC_NEW_DECLS(TYPE)"):c_text.rfind("#endif")]

cvector_str += """

/* header ends */
#endif


#ifdef CVECTOR_IMPLEMENTATION

"""


cvector_str += "#ifndef CVEC_NO_INT\n\n"
cvector_str += get_c_file("cvector_i.c")
cvector_str += "#endif\n\n"

cvector_str += "#ifndef CVEC_NO_DOUBLE\n\n"
cvector_str += get_c_file("cvector_d.c")
cvector_str += "#endif\n\n"

cvector_str += "#ifndef CVEC_NO_STR\n\n"
cvector_str += get_c_file("cvector_str.c")
cvector_str += "#endif\n\n"

cvector_str += "#ifndef CVEC_NO_VOID\n\n"
cvector_str += get_c_file("cvector_void.c")
cvector_str += "#endif\n\n"

cvector_str += """
#endif
"""

cvector_h = open("cvector.h", "w")

cvector_h.write("/*\n")
cvector_h.write(open("header_docs.txt").read())
cvector_h.write("*/\n")

cvector_h.write(cvector_str)

cvector_h.close()
