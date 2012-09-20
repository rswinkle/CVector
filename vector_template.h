#ifndef VECTOR_TYPE_H
#define VECTOR_TYPE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TYPE short
#define vector_TYPE vector_short


/** Data structur for int vector. */
typedef struct vector_TYPE_
{
	TYPE* a;        /**< Array. */
	int size;       /**< Current size (amount you use when manipulating array directly). */
	int capacity;   /**< Allocated size of array; always >= size. */
} vector_TYPE;



extern int VECTOR_TYPE_SZ;


vector_TYPE* vec_TYPE(size_t size, size_t capacity);
vector_TYPE* init_vec_TYPE(TYPE* vals, size_t num);
int push_back_TYPE(vector_TYPE* vec, TYPE a);
TYPE pop_back_TYPE(vector_TYPE* vec);

int insert_TYPE(vector_TYPE* vec, size_t i, TYPE a);
void erase_TYPE(vector_TYPE* vec, size_t start, size_t end);
int reserve_TYPE(vector_TYPE* vec, size_t size);
int set_capacity_TYPE(vector_TYPE* vec, size_t size);
void set_val_sz_TYPE(vector_TYPE* vec, TYPE val);
void set_val_cap_TYPE(vector_TYPE* vec, TYPE val);

/*you really don't need these; you could just check
them directly */
int capacity_TYPE(vector_TYPE* vec);
int size_TYPE(vector_TYPE* vec);
void clear_TYPE(vector_TYPE* vec);
void free_vec_TYPE(vector_TYPE* vec);


#endif