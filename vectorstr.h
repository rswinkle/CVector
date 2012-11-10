#ifndef VECTORSTR_H
#define VECTORSTR_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/** Data structure for string vector. */
typedef struct vector_s_
{
	char** a;          /**< Array. */
	size_t size;       /**< Current size (amount you use when manipulating array directly). */
	size_t capacity;   /**< Allocated size of array; always >= size. */
} vector_s;


extern size_t VEC_S_START_SZ;

char* mystrdup(const char* str);

int vec_s_stack(vector_s* vec, size_t size, size_t capacity);
int init_vec_s_stack(vector_s* vec, char** vals, size_t num);

vector_s* vec_s(size_t size, size_t capacity);
vector_s* init_vec_s(char** vals, size_t num);

void vecs_copy(void* dest, void* src);

int push_backs(vector_s* vec, char* a);
void pop_backs(vector_s* vec, char* ret);

int extends(vector_s* vec, size_t num);
int inserts(vector_s* vec, size_t i, char* a);
int insert_arrays(vector_s* vec, size_t i, char** , size_t num);
void erases(vector_s* vec, size_t start, size_t end);
int reserves(vector_s* vec, size_t size);
int set_capacitys(vector_s* vec, size_t size);
void set_val_szs(vector_s* vec, char* val);
void set_val_caps(vector_s* vec, char* val);

char** backs(vector_s* vec);

void clears(vector_s* vec);
void free_vecs(void* vec);
void free_vecs_stack(void* vec);



#endif