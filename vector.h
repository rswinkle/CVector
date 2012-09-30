#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


typedef unsigned char byte;

/** Data structur for int vector. */
typedef struct vector_i_
{
	int* a;            /**< Array. */
	size_t size;       /**< Current size (amount you use when manipulating array directly). */
	size_t capacity;   /**< Allocated size of array; always >= size. */
} vector_i;


/** Data structure for double vector. */
typedef struct vector_d_
{
	double* a;         /**< Array. */
	size_t size;       /**< Current size (amount you use when manipulating array directly). */
	size_t capacity;   /**< Allocated size of array; always >= size. */
} vector_d;

/** Data structure for string vector. */
typedef struct vector_s_
{
	char** a;          /**< Array. */
	size_t size;       /**< Current size (amount you use when manipulating array directly). */
	size_t capacity;   /**< Allocated size of array; always >= size. */
} vector_s;


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

char* mystrdup(const char* str);


extern size_t VEC_I_START_SZ;
extern size_t VEC_D_START_SZ;
extern size_t VEC_START_SZ;
extern size_t VEC_S_START_SZ;


int vec_i_stack(vector_i* vec, size_t size, size_t capacity);
int init_vec_i_stack(vector_i* vec, int* vals, size_t num);

vector_i* vec_i(size_t size, size_t capacity);
vector_i* init_vec_i(int* vals, size_t num);
void veci_copy(void* dest, void* src);

int push_backi(vector_i* vec, int a);
int pop_backi(vector_i* vec);

int inserti(vector_i* vec, size_t i, int a);
int insert_arrayi(vector_i* vec, size_t i, int* a, size_t num);
void erasei(vector_i* vec, size_t start, size_t end);
int reservei(vector_i* vec, size_t size);
int set_capacityi(vector_i* vec, size_t size);
void set_val_szi(vector_i* vec, int val);
void set_val_capi(vector_i* vec, int val);

void cleari(vector_i* vec);
void free_veci(void* vec);
void free_veci_stack(void* vec);



int vec_d_stack(vector_d* vec, size_t size, size_t capacity);
int init_vec_d_stack(vector_d* vec, double* vals, size_t num);

vector_d* vec_d(size_t size, size_t capacity);
vector_d* init_vec_d(double* vals, size_t num);

void vecd_copy(void* dest, void* src);


int push_backd(vector_d* vec, double a);
double pop_backd(vector_d* vec);

int insertd(vector_d* vec, size_t i, double a);
int insert_arrayd(vector_d* vec, size_t i, double* a, size_t num);
void erased(vector_d* vec, size_t start, size_t end);
int reserved(vector_d* vec, size_t size);
int set_capacityd(vector_d* vec, size_t size);
void set_val_szd(vector_d* vec, double val);
void set_val_capd(vector_d* vec, double val);

void cleard(vector_d* vec);
void free_vecd(void* vec);
void free_vecd_stack(void* vec);



int vec_s_stack(vector_s* vec, size_t size, size_t capacity);
int init_vec_s_stack(vector_s* vec, char** vals, size_t num);

vector_s* vec_s(size_t size, size_t capacity);
vector_s* init_vec_s(char** vals, size_t num);

void vecs_copy(void* dest, void* src);

int push_backs(vector_s* vec, char* a);
void pop_backs(vector_s* vec, char* ret);

int inserts(vector_s* vec, size_t i, char* a);
int insert_arrays(vector_s* vec, size_t i, char** , size_t num);
void erases(vector_s* vec, size_t start, size_t end);
int reserves(vector_s* vec, size_t size);
int set_capacitys(vector_s* vec, size_t size);
void set_val_szs(vector_s* vec, char* val);
void set_val_caps(vector_s* vec, char* val);

void clears(vector_s* vec);
void free_vecs(void* vec);
void free_vecs_stack(void* vec);





int vec_stack(vector* vec, size_t size, size_t capacity, size_t elem_sz, void(*elem_free)(void*), void(*elem_init)(void*, void*));
int init_vec_stack(vector* vec, void* vals, size_t num, size_t elem_sz, void(*elem_free)(void*), void(*elem_init)(void*, void*));

vector* vec(size_t size, size_t capacity, size_t elem_sz, void (*elem_free)(void*), void(*elem_init)(void*, void*));
vector* init_vec(void* vals, size_t num, size_t elem_sz, void (*elem_free)(void*), void(*elem_init)(void*, void*));

void vec_copy(void* dest, void* src);

int push_back(vector* vec, void* val);
void pop_back(vector* vec, void* ret);
void* vec_get(vector* vec, size_t i);

int insert(vector* vec, size_t i, void* a);
int insert_array(vector* vec, size_t i, void* a, size_t num);
void erase(vector* vec, size_t start, size_t end);
int reserve(vector* vec, size_t size);
int set_capacity(vector* vec, size_t size);
void set_val_sz(vector* vec, void* val);
void set_val_cap(vector* vec, void* val);

void clear(vector* vec);
void free_vec(void* vec);
void free_vec_stack(void* vec);


















#endif
