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


int vec_d(vector_d* vec, size_t size, size_t capacity);
int init_vec_d(vector_d* vec, double* vals, size_t num);

vector_d* vec_d_heap(size_t size, size_t capacity);
vector_d* init_vec_d_heap(double* vals, size_t num);

void vec_d_copy(void* dest, void* src);


int push_d(vector_d* vec, double a);
double pop_d(vector_d* vec);

int extend_d(vector_d* vec, size_t num);
int insert_d(vector_d* vec, size_t i, double a);
int insert_array_d(vector_d* vec, size_t i, double* a, size_t num);
double replace_d(vector_d* vec, size_t i, double a);
void erase_d(vector_d* vec, size_t start, size_t end);
int reserve_d(vector_d* vec, size_t size);
int set_capacity_d(vector_d* vec, size_t size);
void set_val_sz_d(vector_d* vec, double val);
void set_val_cap_d(vector_d* vec, double val);

double* back_d(vector_d* vec);

void clear_d(vector_d* vec);
void free_vec_d_heap(void* vec);
void free_vec_d(void* vec);

#ifdef __cplusplus
}
#endif

#endif
