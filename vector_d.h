#ifndef VECTORD_H
#define VECTORD_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Data structure for double vector. */
typedef struct vector_d
{
	double* a;         /**< Array. */
	size_t size;       /**< Current size (amount you use when manipulating array directly). */
	size_t capacity;   /**< Allocated size of array; always >= size. */
} vector_d;

extern size_t VEC_D_START_SZ;


int cvec_d(vector_d* vec, size_t size, size_t capacity);
int cvec_init_d(vector_d* vec, double* vals, size_t num);

vector_d* cvec_d_heap(size_t size, size_t capacity);
vector_d* cvec_init_d_heap(double* vals, size_t num);

void cvec_d_copy(void* dest, void* src);


int cvec_push_d(vector_d* vec, double a);
double cvec_pop_d(vector_d* vec);

int cvec_extend_d(vector_d* vec, size_t num);
int cvec_insert_d(vector_d* vec, size_t i, double a);
int cvec_insert_array_d(vector_d* vec, size_t i, double* a, size_t num);
double cvec_replace_d(vector_d* vec, size_t i, double a);
void cvec_erase_d(vector_d* vec, size_t start, size_t end);
int cvec_reserve_d(vector_d* vec, size_t size);
int cvec_set_cap_d(vector_d* vec, size_t size);
void cvec_set_val_sz_d(vector_d* vec, double val);
void cvec_set_val_cap_d(vector_d* vec, double val);

double* cvec_back_d(vector_d* vec);

void cvec_clear_d(vector_d* vec);
void cvec_free_d_heap(void* vec);
void cvec_free_d(void* vec);

#ifdef __cplusplus
}
#endif

#endif
