#ifndef CVECTORVOID_H
#define CVECTORVOID_H

#include <stdlib.h>

#ifndef CVEC_SIZE_T
#define CVEC_SIZE_T size_t
#endif

#ifndef CVEC_SZ
#define CVEC_SZ
typedef CVEC_SIZE_T cvec_sz;
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char cvec_u8;

/** Data structure for generic type (cast to void) vectors */
typedef struct cvector_void
{
	cvec_u8* a;                 /**< Array. */
	cvec_sz size;             /**< Current size (amount you should use when manipulating array directly). */
	cvec_sz capacity;         /**< Allocated size of array; always >= size. */
	cvec_sz elem_size;        /**< Size in bytes of type stored (sizeof(T) where T is type). */
	void (*elem_free)(void*);
	int (*elem_init)(void*, void*);
} cvector_void;

extern cvec_sz CVEC_VOID_START_SZ;

#define CVEC_GET_VOID(VEC, TYPE, I) ((TYPE*)&(VEC)->a[(I)*(VEC)->elem_size])

int cvec_void(cvector_void* vec, cvec_sz size, cvec_sz capacity, cvec_sz elem_sz, void(*elem_free)(void*), int(*elem_init)(void*, void*));
int cvec_init_void(cvector_void* vec, void* vals, cvec_sz num, cvec_sz elem_sz, void(*elem_free)(void*), int(*elem_init)(void*, void*));

cvector_void* cvec_void_heap(cvec_sz size, cvec_sz capacity, cvec_sz elem_sz, void (*elem_free)(void*), int(*elem_init)(void*, void*));
cvector_void* cvec_init_void_heap(void* vals, cvec_sz num, cvec_sz elem_sz, void (*elem_free)(void*), int(*elem_init)(void*, void*));

int cvec_copyc_void(void* dest, void* src);
int cvec_copy_void(cvector_void* dest, cvector_void* src);

int cvec_push_void(cvector_void* vec, void* a);
void cvec_pop_void(cvector_void* vec, void* ret);
void* cvec_get_void(cvector_void* vec, cvec_sz i);

int cvec_pushm_void(cvector_void* vec, void* a);
void cvec_popm_void(cvector_void* vec, void* ret);
int cvec_insertm_void(cvector_void* vec, cvec_sz i, void* a);
int cvec_insert_arraym_void(cvector_void* vec, cvec_sz i, void* a, cvec_sz num);
void cvec_replacem_void(cvector_void* vec, cvec_sz i, void* a, void* ret);

int cvec_extend_void(cvector_void* vec, cvec_sz num);
int cvec_insert_void(cvector_void* vec, cvec_sz i, void* a);
int cvec_insert_array_void(cvector_void* vec, cvec_sz i, void* a, cvec_sz num);
int cvec_replace_void(cvector_void* vec, cvec_sz i, void* a, void* ret);
void cvec_erase_void(cvector_void* vec, cvec_sz start, cvec_sz end);
void cvec_remove_void(cvector_void* vec, cvec_sz start, cvec_sz end);
int cvec_reserve_void(cvector_void* vec, cvec_sz size);
int cvec_set_cap_void(cvector_void* vec, cvec_sz size);
int cvec_set_val_sz_void(cvector_void* vec, void* val);
int cvec_set_val_cap_void(cvector_void* vec, void* val);

void* cvec_back_void(cvector_void* vec);

void cvec_clear_void(cvector_void* vec);
void cvec_free_void_heap(void* vec);
void cvec_free_void(void* vec);

#ifdef __cplusplus
}
#endif


#endif
