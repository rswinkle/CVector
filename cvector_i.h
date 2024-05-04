#ifndef CVECTORI_H
#define CVECTORI_H

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

/** Data structure for int vector. */
typedef struct cvector_i
{
	int* a;            /**< Array. */
	cvec_sz size;       /**< Current size (amount you use when manipulating array directly). */
	cvec_sz capacity;   /**< Allocated size of array; always >= size. */
} cvector_i;

extern cvec_sz CVEC_I_START_SZ;

int cvec_i(cvector_i* vec, cvec_sz size, cvec_sz capacity);
int cvec_init_i(cvector_i* vec, int* vals, cvec_sz num);

cvector_i* cvec_i_heap(cvec_sz size, cvec_sz capacity);
cvector_i* cvec_init_i_heap(int* vals, cvec_sz num);
int cvec_copyc_i(void* dest, void* src);
int cvec_copy_i(cvector_i* dest, cvector_i* src);

int cvec_push_i(cvector_i* vec, int a);
int cvec_pop_i(cvector_i* vec);

int cvec_extend_i(cvector_i* vec, cvec_sz num);
int cvec_insert_i(cvector_i* vec, cvec_sz i, int a);
int cvec_insert_array_i(cvector_i* vec, cvec_sz i, int* a, cvec_sz num);
int cvec_replace_i(cvector_i* vec, cvec_sz i, int a);
void cvec_erase_i(cvector_i* vec, cvec_sz start, cvec_sz end);
int cvec_reserve_i(cvector_i* vec, cvec_sz size);
#define cvec_shrink_to_fit_i(vec) cvec_set_cap_i((vec), (vec)->size)
int cvec_set_cap_i(cvector_i* vec, cvec_sz size);
void cvec_set_val_sz_i(cvector_i* vec, int val);
void cvec_set_val_cap_i(cvector_i* vec, int val);

int* cvec_back_i(cvector_i* vec);

void cvec_clear_i(cvector_i* vec);
void cvec_free_i_heap(void* vec);
void cvec_free_i(void* vec);

#ifdef __cplusplus
}
#endif

#endif
