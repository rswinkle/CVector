#ifndef VECTOR_TYPE_H
#define VECTOR_TYPE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>



/** Data structure for TYPE vector. */
typedef struct vector_TYPE
{
	TYPE* a;           /**< Array. */
	size_t size;       /**< Current size (amount you use when manipulating array directly). */
	size_t capacity;   /**< Allocated size of array; always >= size. */
} vector_TYPE;



extern size_t VECTOR_TYPE_SZ;

int vec_TYPE(vector_TYPE* vec, size_t size, size_t capacity);
int init_vec_TYPE(vector_TYPE* vec, TYPE* vals, size_t num);

vector_TYPE* vec_TYPE_heap(size_t size, size_t capacity);
vector_TYPE* init_vec_TYPE_heap(TYPE* vals, size_t num);

void vec_TYPE_copy(void* dest, void* src);

int push_TYPE(vector_TYPE* vec, TYPE a);
TYPE pop_TYPE(vector_TYPE* vec);

int extend_TYPE(vector_TYPE* vec, size_t num);
int insert_TYPE(vector_TYPE* vec, size_t i, TYPE a);
int insert_array_TYPE(vector_TYPE* vec, size_t i, TYPE* a, size_t num);
void erase_TYPE(vector_TYPE* vec, size_t start, size_t end);
int reserve_TYPE(vector_TYPE* vec, size_t size);
int set_capacity_TYPE(vector_TYPE* vec, size_t size);
void set_val_sz_TYPE(vector_TYPE* vec, TYPE val);
void set_val_cap_TYPE(vector_TYPE* vec, TYPE val);

TYPE* back_TYPE(vector_TYPE* vec);

void clear_TYPE(vector_TYPE* vec);
void free_vec_TYPE_heap(void* vec);
void free_vec_TYPE(void* vec);



#endif
