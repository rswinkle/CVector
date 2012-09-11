#ifndef VECTOR_TEMPLATE_H
#define VECTOR_TEMPLATE_H

#define TYPE short
#define VECTOR_STRUCT vector_short


/** Data structur for int vector. */
typedef struct VECTOR_STRUCT_
{
	TYPE* a;        /**< Array. */
	int size;       /**< Current size (amount you use when manipulating array directly). */
	int capacity;   /**< Allocated size of array; always >= size. */
} VECTOR_STRUCT;



extern int VECTOR_START_SZ;


VECTOR_STRUCT* vec_TYPE(size_t size, size_t capacity);
VECTOR_STRUCT* init_vec_TYPE(int* vals, size_t num);
int push_bacK_TYPE(VECTOR_STRUCT* vec, int a);
int pop_back_TYPE(VECTOR_STRUCT* vec);

int insert_TYPE(VECTOR_STRUCT* vec, size_t i, int a);
void erase_TYPE(VECTOR_STRUCT* vec, size_t start, size_t end);
int reserve_TYPE(VECTOR_STRUCT* vec, size_t size);
int set_capacity_TYPE(VECTOR_STRUCT* vec, size_t size);
void set_val_sz_TYPE(VECTOR_STRUCT* vec, int val);
void set_val_cap_TYPE(VECTOR_STRUCT* vec, int val);

/*you really don't need these; you could just check
them directly */
int capacity_TYPE(VECTOR_STRUCT* vec);
int size_TYPE(VECTOR_STRUCT* vec);
void clear_TYPE(VECTOR_STRUCT* vec);
void free_vec_TYPE(VECTOR_STRUCT* vec);


#endif