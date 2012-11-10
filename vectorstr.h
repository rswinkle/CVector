#ifndef VECTORSTR_H
#define VECTORSTR_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/** Data structure for string vector. */
typedef struct vector_str_
{
	char** a;          /**< Array. */
	size_t size;       /**< Current size (amount you use when manipulating array directly). */
	size_t capacity;   /**< Allocated size of array; always >= size. */
} vector_str;


extern size_t VEC_STR_START_SZ;

char* mystrdup(const char* str);

int vec_str_stack(vector_str* vec, size_t size, size_t capacity);
int init_vec_str_stack(vector_str* vec, char** vals, size_t num);

vector_str* vec_str(size_t size, size_t capacity);
vector_str* init_vec_str(char** vals, size_t num);

void vec_str_copy(void* dest, void* src);

int push_back_str(vector_str* vec, char* a);
void pop_back_str(vector_str* vec, char* ret);

int extend_str(vector_str* vec, size_t num);
int insert_str(vector_str* vec, size_t i, char* a);
int insert_array_str(vector_str* vec, size_t i, char** , size_t num);
void erase_str(vector_str* vec, size_t start, size_t end);
int reserve_str(vector_str* vec, size_t size);
int set_capacity_str(vector_str* vec, size_t size);
void set_val_sz_str(vector_str* vec, char* val);
void set_val_cap_str(vector_str* vec, char* val);

char** back_str(vector_str* vec);

void clear_str(vector_str* vec);
void free_vec_str(void* vec);
void free_vec_str_stack(void* vec);



#endif