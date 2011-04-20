#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


/** Data structur for int vector. */
typedef struct vector_i_
{
	int* a;			///< Array.
	int size;		///< Current size (amount you use when manipulating array directly).
	int capacity;	///< Allocated size of array; always >= size.
} vector_i;


/** Data structure for double vector. */
typedef struct vector_d_
{
	double* a;		///< Array.
	int size;		///< Current size (amount you use when manipulating array directly).
	int capacity;	///< Allocated size of array; always >= size.
} vector_d;

/** Data structure for string vector. */
typedef struct vector_s_
{
	char** a;		///< Array.
	int size;		///< Current size (amount you use when manipulating array directly).
	int capacity;	///< Allocated size of array; always >= size.
} vector_s;


/** Data structure for generic type (cast to void) vectors */
typedef struct vector_
{
	void* a;		///< Array.
	int size;		///< Current size (amount you should use when manipulating array directly).
	int capacity;	///< Allocated size of array; always >= size.
	int elem_size;	///< Size in bytes of type stored (sizeof(T) where T is type).
} vector;

char* mystrdup(const char* str);

//int VEC_I_START_SZ = 50;
//int VEC_D_START_SZ = 50;

extern int VEC_I_START_SZ;
extern int VEC_D_START_SZ;
extern int VEC_START_SZ;
extern int VEC_S_START_SZ;


vector_i* vec_i(int sz);
vector_i* init_vec_i(int* vals, int num);
int push_backi(vector_i* vec, int a);
int pop_backi(vector_i* vec);

int inserti(vector_i* vec, int i, int a);
void erasei(vector_i* vec, int start, int end);
int reservei(vector_i* vec, int size);
int set_capacityi(vector_i* vec, int size);
void set_val_szi(vector_i* vec, int val);
void set_val_capi(vector_i* vec, int val);

//you really don't need these; you could just check
//them directly
int capacityi(vector_i* vec);
int sizei(vector_i* vec);
void cleari(vector_i* vec);
void free_veci(vector_i* vec);




vector_d* vec_d(int sz);
vector_d* init_vec_d(double* vals, int num);
int push_backd(vector_d* vec, double a);
double pop_backd(vector_d* vec);

int insertd(vector_d* vec, int i, double a);
void erased(vector_d* vec, int start, int end);
int reserved(vector_d* vec, int size);
int set_capacityd(vector_d* vec, int size);
void set_val_szd(vector_d* vec, double val);
void set_val_capd(vector_d* vec, double val);

//you really don't need these; you could just check
//them directly
int capacityd(vector_d* vec);
int sized(vector_d* vec);
void cleard(vector_d* vec);
void free_vecd(vector_d* vec);





vector_s* vec_s(int sz);
vector_s* init_vec_s(char** vals, int num);
int push_backs(vector_s* vec, char* a);
void pop_backs(vector_s* vec, char* ret);

int inserts(vector_s* vec, int i, char* a);
void erases(vector_s* vec, int start, int end);
int reserves(vector_s* vec, int size);
int set_capacitys(vector_s* vec, int size);
void set_val_szs(vector_s* vec, char* val);
void set_val_caps(vector_s* vec, char* val);

//you really don't need these; you could just check
//them directly
int capacitys(vector_s* vec);
int sizes(vector_s* vec);
void clears(vector_s* vec);
void free_vecs(vector_s* vec);








vector* vec(int sz, int elem_sz);
vector* init_vec(void* vals, int num, int elem_sz);
int push_back(vector* vec, void* val);
void pop_back(vector* vec, void* ret);

int insert(vector* vec, int i, void* a);
void erase(vector* vec, int start, int end);
int reserve(vector* vec, int size);
int set_capacity(vector* vec, int size);
void set_val_sz(vector* vec, void* val);
void set_val_cap(vector* vec, void* val);

//you really don't need these; you could just check
//them directly
int capacity(vector* vec);
int size(vector* vec);
void clear(vector* vec);
void free_vec(vector* vec);


















#endif
