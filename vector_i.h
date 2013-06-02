#ifndef VECTORI_H
#define VECTORI_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


/** Data structure for int vector. */
typedef struct vector_i
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
void vec_i_copy(void* dest, void* src);

int push_i(vector_i* vec, int a);
int pop_i(vector_i* vec);

int extend_i(vector_i* vec, size_t num);
int insert_i(vector_i* vec, size_t i, int a);
int insert_array_i(vector_i* vec, size_t i, int* a, size_t num);
void erase_i(vector_i* vec, size_t start, size_t end);
int reserve_i(vector_i* vec, size_t size);
int set_capacity_i(vector_i* vec, size_t size);
void set_val_sz_i(vector_i* vec, int val);
void set_val_cap_i(vector_i* vec, int val);

int* back_i(vector_i* vec);

void clear_i(vector_i* vec);
void free_vec_i_heap(void* vec);
void free_vec_i(void* vec);



#endif
