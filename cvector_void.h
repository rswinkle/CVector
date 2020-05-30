#ifndef CVECTORVOID_H
#define CVECTORVOID_H

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char cvec_u8;

/** Data structure for generic type (cast to void) vectors */
typedef struct cvector_void
{
	cvec_u8* a;                 /**< Array. */
	size_t size;             /**< Current size (amount you should use when manipulating array directly). */
	size_t capacity;         /**< Allocated size of array; always >= size. */
	size_t elem_size;        /**< Size in bytes of type stored (sizeof(T) where T is type). */
	void (*elem_free)(void*);
	int (*elem_init)(void*, void*);
} cvector_void;

extern size_t CVEC_VOID_START_SZ;

#define CVEC_GET_VOID(VEC, TYPE, I) ((TYPE*)&(VEC)->a[(I)*(VEC)->elem_size])

int cvec_void(cvector_void* vec, size_t size, size_t capacity, size_t elem_sz, void(*elem_free)(void*), int(*elem_init)(void*, void*));
int cvec_init_void(cvector_void* vec, void* vals, size_t num, size_t elem_sz, void(*elem_free)(void*), int(*elem_init)(void*, void*));

cvector_void* cvec_void_heap(size_t size, size_t capacity, size_t elem_sz, void (*elem_free)(void*), int(*elem_init)(void*, void*));
cvector_void* cvec_init_void_heap(void* vals, size_t num, size_t elem_sz, void (*elem_free)(void*), int(*elem_init)(void*, void*));

int cvec_copyc_void(void* dest, void* src);
int cvec_copy_void(cvector_void* dest, cvector_void* src);

int cvec_push_void(cvector_void* vec, void* val);
void cvec_pop_void(cvector_void* vec, void* ret);
void* cvec_get_void(cvector_void* vec, size_t i);

int cvec_extend_void(cvector_void* vec, size_t num);
int cvec_insert_void(cvector_void* vec, size_t i, void* a);
int cvec_insert_array_void(cvector_void* vec, size_t i, void* a, size_t num);
void cvec_replace_void(cvector_void* vec, size_t i, void* a, void* ret);
void cvec_erase_void(cvector_void* vec, size_t start, size_t end);
void cvec_remove_void(cvector_void* vec, size_t start, size_t end);
int cvec_reserve_void(cvector_void* vec, size_t size);
int cvec_set_cap_void(cvector_void* vec, size_t size);
void cvec_set_val_sz_void(cvector_void* vec, void* val);
void cvec_set_val_cap_void(cvector_void* vec, void* val);

void* cvec_back_void(cvector_void* vec);

void cvec_clear_void(cvector_void* vec);
void cvec_free_void_heap(void* vec);
void cvec_free_void(void* vec);

#ifdef __cplusplus
}
#endif


#endif
