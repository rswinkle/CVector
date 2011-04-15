#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct vector_i_
{
	int* a;
	int size;
	int capacity;
} vector_i;

typedef struct vector_d_
{
	double* a;
	int size;
	int capacity;
} vector_d;


typedef struct vector_s_
{
	char** a;
	int size;
	int capacity;
} vector_s;



//int VEC_I_START_SZ = 50;
//int VEC_D_START_SZ = 50;


vector_i* vec_i(int sz);
vector_i* init_vec_i(int* vals, int num);
int push_backi(vector_i* vec, int a);
int pop_backi(vector_i* vec);

int inserti(vector_i* vec, int i, int a);
void erasei(vector_i* vec, int start, int end);
int reservei(vector_i* vec, int size);
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
void pop_backs(vector_s* vec);

int inserts(vector_s* vec, int i, char* a);
void erases(vector_s* vec, int start, int end);
int reserves(vector_s* vec, int size);
void set_val_szs(vector_s* vec, char* val);
void set_val_caps(vector_s* vec, char* val);

//you really don't need these; you could just check
//them directly
int capacitys(vector_s* vec);
int sizes(vector_s* vec);
void clears(vector_s* vec);
void free_vecs(vector_s* vec);



























#endif
