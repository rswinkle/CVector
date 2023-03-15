#ifndef CVECTORD_H
#define CVECTORD_H

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

/** Data structure for double vector. */
typedef struct cvector_d
{
	double* a;         /**< Array. */
	cvec_sz size;       /**< Current size (amount you use when manipulating array directly). */
	cvec_sz capacity;   /**< Allocated size of array; always >= size. */
} cvector_d;

extern cvec_sz CVEC_D_START_SZ;

int cvec_d(cvector_d* vec, cvec_sz size, cvec_sz capacity);
int cvec_init_d(cvector_d* vec, double* vals, cvec_sz num);

cvector_d* cvec_d_heap(cvec_sz size, cvec_sz capacity);
cvector_d* cvec_init_d_heap(double* vals, cvec_sz num);
int cvec_copyc_d(void* dest, void* src);
int cvec_copy_d(cvector_d* dest, cvector_d* src);

int cvec_push_d(cvector_d* vec, double a);
double cvec_pop_d(cvector_d* vec);

int cvec_extend_d(cvector_d* vec, cvec_sz num);
int cvec_insert_d(cvector_d* vec, cvec_sz i, double a);
int cvec_insert_array_d(cvector_d* vec, cvec_sz i, double* a, cvec_sz num);
double cvec_replace_d(cvector_d* vec, cvec_sz i, double a);
void cvec_erase_d(cvector_d* vec, cvec_sz start, cvec_sz end);
int cvec_reserve_d(cvector_d* vec, cvec_sz size);
int cvec_set_cap_d(cvector_d* vec, cvec_sz size);
void cvec_set_val_sz_d(cvector_d* vec, double val);
void cvec_set_val_cap_d(cvector_d* vec, double val);

double* cvec_back_d(cvector_d* vec);

void cvec_clear_d(cvector_d* vec);
void cvec_free_d_heap(void* vec);
void cvec_free_d(void* vec);

#ifdef __cplusplus
}
#endif

#endif
