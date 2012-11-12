#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef unsigned char byte;

/** Data structure for generic type (cast to void) vectors */
typedef struct vector_
{
	byte* a;                 /**< Array. */
	size_t size;             /**< Current size (amount you should use when manipulating array directly). */
	size_t capacity;         /**< Allocated size of array; always >= size. */
	size_t elem_size;        /**< Size in bytes of type stored (sizeof(T) where T is type). */
	void (*elem_init)(void*, void*);
	void (*elem_free)(void*);
} vector_void;

extern size_t VEC_START_SZ;



int vec_void(vector_void* vec, size_t size, size_t capacity, size_t elem_sz, void(*elem_free)(void*), void(*elem_init)(void*, void*));
int init_vec_void(vector_void* vec, void* vals, size_t num, size_t elem_sz, void(*elem_free)(void*), void(*elem_init)(void*, void*));

vector_void* vec_void_heap(size_t size, size_t capacity, size_t elem_sz, void (*elem_free)(void*), void(*elem_init)(void*, void*));
vector_void* init_vec_void_heap(void* vals, size_t num, size_t elem_sz, void (*elem_free)(void*), void(*elem_init)(void*, void*));

void vec_void_copy(void* dest, void* src);

int push_void(vector_void* vec, void* val);
void pop_void(vector_void* vec, void* ret);
void* vec_void_get(vector_void* vec, size_t i);

int extend_void(vector_void* vec, size_t num);
int insert_void(vector_void* vec, size_t i, void* a);
int insert_array_void(vector_void* vec, size_t i, void* a, size_t num);
void erase_void(vector_void* vec, size_t start, size_t end);
int reserve_void(vector_void* vec, size_t size);
int set_capacity_void(vector_void* vec, size_t size);
void set_val_sz_void(vector_void* vec, void* val);
void set_val_cap_void(vector_void* vec, void* val);

void* back_void(vector_void* vec);

void clear_void(vector_void* vec);
void free_vec_void_heap(void* vec);
void free_vec_void(void* vec);



#endif
