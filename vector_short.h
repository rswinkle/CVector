#ifndef VECTOR_short_H
#define VECTOR_short_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>



/** Data structure for short vector. */
typedef struct vector_short_
{
	short* a;           /**< Array. */
	size_t size;       /**< Current size (amount you use when manipulating array directly). */
	size_t capacity;   /**< Allocated size of array; always >= size. */
} vector_short;



extern size_t VECTOR_short_SZ;

int vec_short(vector_short* vec, size_t size, size_t capacity);
int init_vec_short(vector_short* vec, short* vals, size_t num);

vector_short* vec_short_heap(size_t size, size_t capacity);
vector_short* init_vec_short_heap(short* vals, size_t num);

void vec_short_copy(void* dest, void* src);

int push_short(vector_short* vec, short a);
short pop_short(vector_short* vec);

int extend_short(vector_short* vec, size_t num);
int insert_short(vector_short* vec, size_t i, short a);
int insert_array_short(vector_short* vec, size_t i, short* a, size_t num);
void erase_short(vector_short* vec, size_t start, size_t end);
int reserve_short(vector_short* vec, size_t size);
int set_capacity_short(vector_short* vec, size_t size);
void set_val_sz_short(vector_short* vec, short val);
void set_val_cap_short(vector_short* vec, short val);

short* back_short(vector_short* vec);

void clear_short(vector_short* vec);
void free_vec_short_heap(void* vec);
void free_vec_short(void* vec);



#endif