#ifndef VECTOR_short_H
#define VECTOR_short_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>



/** Data structur for int vector. */
typedef struct vector_short_
{
	short* a;           /**< Array. */
	size_t size;       /**< Current size (amount you use when manipulating array directly). */
	size_t capacity;   /**< Allocated size of array; always >= size. */
} vector_short;



extern size_t VECTOR_short_SZ;

int vec_short_stack(vector_short* vec, size_t size, size_t capacity);
int init_vec_short_stack(vector_short* vec, short* vals, size_t num);

vector_short* vec_short(size_t size, size_t capacity);
vector_short* init_vec_short(short* vals, size_t num);

void vec_short_copy(void* dest, void* src);

int push_back_short(vector_short* vec, short a);
short pop_back_short(vector_short* vec);

int insert_short(vector_short* vec, size_t i, short a);
int insert_array_short(vector_short* vec, size_t i, short* a, size_t num);
void erase_short(vector_short* vec, size_t start, size_t end);
int reserve_short(vector_short* vec, size_t size);
int set_capacity_short(vector_short* vec, size_t size);
void set_val_sz_short(vector_short* vec, short val);
void set_val_cap_short(vector_short* vec, short val);

/*you really don't need these; you could just check
them directly */
int capacity_short(vector_short* vec);
int size_short(vector_short* vec);
void clear_short(vector_short* vec);
void free_vec_short(void* vec);
void free_vec_short_stack(void* vec);



#endif