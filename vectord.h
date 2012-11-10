#ifndef VECTORD_H
#define VECTORD_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/** Data structure for double vector. */
typedef struct vector_d_
{
	double* a;         /**< Array. */
	size_t size;       /**< Current size (amount you use when manipulating array directly). */
	size_t capacity;   /**< Allocated size of array; always >= size. */
} vector_d;

extern size_t VEC_D_START_SZ;


int vec_d_stack(vector_d* vec, size_t size, size_t capacity);
int init_vec_d_stack(vector_d* vec, double* vals, size_t num);

vector_d* vec_d(size_t size, size_t capacity);
vector_d* init_vec_d(double* vals, size_t num);

void vecd_copy(void* dest, void* src);


int push_backd(vector_d* vec, double a);
double pop_backd(vector_d* vec);

int extendd(vector_d* vec, size_t num);
int insertd(vector_d* vec, size_t i, double a);
int insert_arrayd(vector_d* vec, size_t i, double* a, size_t num);
void erased(vector_d* vec, size_t start, size_t end);
int reserved(vector_d* vec, size_t size);
int set_capacityd(vector_d* vec, size_t size);
void set_val_szd(vector_d* vec, double val);
void set_val_capd(vector_d* vec, double val);

double* backd(vector_d* vec);

void cleard(vector_d* vec);
void free_vecd(void* vec);
void free_vecd_stack(void* vec);

#endif