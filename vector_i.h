#ifndef VECTORI_H
#define VECTORI_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Data structure for int vector. */
typedef struct vector_i
{
	int* a;            /**< Array. */
	size_t size;       /**< Current size (amount you use when manipulating array directly). */
	size_t capacity;   /**< Allocated size of array; always >= size. */
} vector_i;


extern size_t VEC_I_START_SZ;


int cvec_i(vector_i* vec, size_t size, size_t capacity);
int cvec_init_i(vector_i* vec, int* vals, size_t num);

vector_i* cvec_i_heap(size_t size, size_t capacity);
vector_i* cvec_init_i_heap(int* vals, size_t num);
void cvec_i_copy(void* dest, void* src);

int cvec_push_i(vector_i* vec, int a);
int cvec_pop_i(vector_i* vec);

int cvec_extend_i(vector_i* vec, size_t num);
int cvec_insert_i(vector_i* vec, size_t i, int a);
int cvec_insert_array_i(vector_i* vec, size_t i, int* a, size_t num);
int cvec_replace_i(vector_i* vec, size_t i, int a);
void cvec_erase_i(vector_i* vec, size_t start, size_t end);
int cvec_reserve_i(vector_i* vec, size_t size);
int cvec_set_cap_i(vector_i* vec, size_t size);
void cvec_set_val_sz_i(vector_i* vec, int val);
void cvec_set_val_cap_i(vector_i* vec, int val);

int* cvec_back_i(vector_i* vec);

void cvec_clear_i(vector_i* vec);
void cvec_free_i_heap(void* vec);
void cvec_free_i(void* vec);


#ifdef __cplusplus
}
#endif

#endif
