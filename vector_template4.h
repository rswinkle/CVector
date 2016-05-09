#ifndef VECTOR_TYPE_H
#define VECTOR_TYPE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#ifdef __cplusplus
extern "C" {
#endif


typedef struct vector_TYPE
{
	TYPE* a;
	size_t size;
	size_t capacity;
	void (*elem_init)(void*, void*);
	void (*elem_free)(void*);
} vector_TYPE;

extern size_t VEC_TYPE_SZ;

int vec_TYPE(vector_TYPE* vec, size_t size, size_t capacity, void(*elem_free)(void*), void(*elem_init)(void*, void*));
int init_vec_TYPE(vector_TYPE* vec, TYPE* vals, size_t num, void(*elem_free)(void*), void(*elem_init)(void*, void*));

vector_TYPE* vec_TYPE_heap(size_t size, size_t capacity, void (*elem_free)(void*), void(*elem_init)(void*, void*));
vector_TYPE* init_vec_TYPE_heap(TYPE* vals, size_t num, void (*elem_free)(void*), void(*elem_init)(void*, void*));

void vec_TYPE_copy(void* dest, void* src);

int push_TYPE(vector_TYPE* vec, TYPE* val);
void pop_TYPE(vector_TYPE* vec, TYPE* ret);

int extend_TYPE(vector_TYPE* vec, size_t num);
int insert_TYPE(vector_TYPE* vec, size_t i, TYPE* a);
int insert_array_TYPE(vector_TYPE* vec, size_t i, TYPE* a, size_t num);
void replace_TYPE(vector_TYPE* vec, size_t i, TYPE* a, TYPE* ret);
void erase_TYPE(vector_TYPE* vec, size_t start, size_t end);
int reserve_TYPE(vector_TYPE* vec, size_t size);
int set_capacity_TYPE(vector_TYPE* vec, size_t size);
void set_val_sz_TYPE(vector_TYPE* vec, TYPE* val);
void set_val_cap_TYPE(vector_TYPE* vec, TYPE* val);

TYPE* back_TYPE(vector_TYPE* vec);

void clear_TYPE(vector_TYPE* vec);
void free_vec_TYPE_heap(void* vec);
void free_vec_TYPE(void* vec);

#ifdef __cplusplus
}
#endif


#endif
