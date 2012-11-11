#ifndef VECTORI_H
#define VECTORI_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


/** Data structure for int vector. */
typedef struct vector_i_
{
	int* a;            /**< Array. */
	size_t size;       /**< Current size (amount you use when manipulating array directly). */
	size_t capacity;   /**< Allocated size of array; always >= size. */
} vector_i;


extern size_t VEC_I_START_SZ;


int vec_i(vector_i* vec, size_t size, size_t capacity);
int init_vec_i(vector_i* vec, int* vals, size_t num);

vector_i* vec_i_heap(size_t size, size_t capacity);
vector_i* init_vec_i_heap(int* vals, size_t num);
void veci_copy(void* dest, void* src);

int push_i(vector_i* vec, int a);
int pop_i(vector_i* vec);

int extendi(vector_i* vec, size_t num);
int inserti(vector_i* vec, size_t i, int a);
int insert_arrayi(vector_i* vec, size_t i, int* a, size_t num);
void erasei(vector_i* vec, size_t start, size_t end);
int reservei(vector_i* vec, size_t size);
int set_capacityi(vector_i* vec, size_t size);
void set_val_szi(vector_i* vec, int val);
void set_val_capi(vector_i* vec, int val);

int* backi(vector_i* vec);

void cleari(vector_i* vec);
void free_veci_heap(void* vec);
void free_veci(void* vec);



#endif