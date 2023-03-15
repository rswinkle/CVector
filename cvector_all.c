
/*
 This file is a hack to test these, just use cvector.h if you want any
 of the base types

 This annoyance occurs because unlike the other configurable things that
 are only needed in the C file, CVEC_SIZE_T needs to be defined for the
 header as well meaning it has to be defined for multiple translation units

 The other option is to add it to the compile flags -DCVEC_SIZE_T=someinttype

#define CVEC_SIZE_T short
*/

#include "cvector_i.c"
#include "cvector_d.c"
#include "cvector_str.c"
#include "cvector_void.c"
