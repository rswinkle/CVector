import sys, os, glob, re

cvector_str = """
/* header starts */

#ifndef CVECTOR_H
#define CVECTOR_H

#include <stdlib.h>
#include <string.h>
#include <assert.h>
"""

cvector_str += """
#ifdef __cplusplus
extern "C" {
#endif
"""


def get_header(filename):
	header_text = open(filename).read()
	start = header_text.find("#endif") + 6 # #endif of extern "C"
	end = header_text.find("#ifdef", start) #ifdef close of extern "C"
	return header_text[start:end]


cvector_str += get_header("cvector_i.h")
cvector_str += get_header("cvector_d.h")
cvector_str += get_header("cvector_str.h")
cvector_str += get_header("cvector_void.h")

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

"""

def get_c_file(filename):
	c_text = open(filename).read()
	return c_text[c_text.find("size_t"):]

cvector_str += get_c_file("cvector_i.c")
cvector_str += get_c_file("cvector_d.c")
cvector_str += get_c_file("cvector_str.c")
cvector_str += get_c_file("cvector_void.c")

cvector_str += """
#endif
"""

open("cvector.h", "w").write(cvector_str)
