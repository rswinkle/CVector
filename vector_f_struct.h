#ifndef VECTOR_f_struct_H
#define VECTOR_f_struct_H

#include "test_types.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/** Data structure for generic type (cast to void) vectors */
typedef struct vector_f_struct
{
	f_struct* a;                 /**< Array. */
	size_t size;             /**< Current size (amount you should use when manipulating array directly). */
	size_t capacity;         /**< Allocated size of array; always >= size. */
	void (*elem_init)(void*, void*);
	void (*elem_free)(void*);
} vector_f_struct;

extern size_t VEC_f_struct_SZ;

int vec_f_struct(vector_f_struct* vec, size_t size, size_t capacity, void(*elem_free)(void*), void(*elem_init)(void*, void*));
int init_vec_f_struct(vector_f_struct* vec, f_struct* vals, size_t num, void(*elem_free)(void*), void(*elem_init)(void*, void*));

vector_f_struct* vec_f_struct_heap(size_t size, size_t capacity, void (*elem_free)(void*), void(*elem_init)(void*, void*));
vector_f_struct* init_vec_f_struct_heap(f_struct* vals, size_t num, void (*elem_free)(void*), void(*elem_init)(void*, void*));

void vec_f_struct_copy(void* dest, void* src);

int push_f_struct(vector_f_struct* vec, f_struct* val);
void pop_f_struct(vector_f_struct* vec, f_struct* ret);

int extend_f_struct(vector_f_struct* vec, size_t num);
int insert_f_struct(vector_f_struct* vec, size_t i, f_struct* a);
int insert_array_f_struct(vector_f_struct* vec, size_t i, f_struct* a, size_t num);
void erase_f_struct(vector_f_struct* vec, size_t start, size_t end);
int reserve_f_struct(vector_f_struct* vec, size_t size);
int set_capacity_f_struct(vector_f_struct* vec, size_t size);
void set_val_sz_f_struct(vector_f_struct* vec, f_struct* val);
void set_val_cap_f_struct(vector_f_struct* vec, f_struct* val);

f_struct* back_f_struct(vector_f_struct* vec);

void clear_f_struct(vector_f_struct* vec);
void free_vec_f_struct_heap(void* vec);
void free_vec_f_struct(void* vec);



#endif
