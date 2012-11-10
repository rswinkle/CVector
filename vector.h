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
} vector;

extern size_t VEC_START_SZ;



int vec_stack(vector* vec, size_t size, size_t capacity, size_t elem_sz, void(*elem_free)(void*), void(*elem_init)(void*, void*));
int init_vec_stack(vector* vec, void* vals, size_t num, size_t elem_sz, void(*elem_free)(void*), void(*elem_init)(void*, void*));

vector* vec(size_t size, size_t capacity, size_t elem_sz, void (*elem_free)(void*), void(*elem_init)(void*, void*));
vector* init_vec(void* vals, size_t num, size_t elem_sz, void (*elem_free)(void*), void(*elem_init)(void*, void*));

void vec_copy(void* dest, void* src);

int push_back(vector* vec, void* val);
void pop_back(vector* vec, void* ret);
void* vec_get(vector* vec, size_t i);

int extend(vector* vec, size_t num);
int insert(vector* vec, size_t i, void* a);
int insert_array(vector* vec, size_t i, void* a, size_t num);
void erase(vector* vec, size_t start, size_t end);
int reserve(vector* vec, size_t size);
int set_capacity(vector* vec, size_t size);
void set_val_sz(vector* vec, void* val);
void set_val_cap(vector* vec, void* val);

void* back(vector* vec);

void clear(vector* vec);
void free_vec(void* vec);
void free_vec_stack(void* vec);



#endif
