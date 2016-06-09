#ifndef VECTORSTR_H
#define VECTORSTR_H

#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Data structure for string vector. */
typedef struct cvector_str
{
	char** a;          /**< Array. */
	size_t size;       /**< Current size (amount you use when manipulating array directly). */
	size_t capacity;   /**< Allocated size of array; always >= size. */
} cvector_str;


extern size_t CVEC_STR_START_SZ;

char* mystrdup(const char* str);

int cvec_str(cvector_str* vec, size_t size, size_t capacity);
int cvec_init_str(cvector_str* vec, char** vals, size_t num);

cvector_str* cvec_str_heap(size_t size, size_t capacity);
cvector_str* cvec_init_str_heap(char** vals, size_t num);

void cvec_str_copy(void* dest, void* src);

int cvec_push_str(cvector_str* vec, char* a);
void cvec_pop_str(cvector_str* vec, char* ret);

int cvec_extend_str(cvector_str* vec, size_t num);
int cvec_insert_str(cvector_str* vec, size_t i, char* a);
int cvec_insert_array_str(cvector_str* vec, size_t i, char** a, size_t num);
void cvec_replace_str(cvector_str* vec, size_t i, char* a, char* ret);
void cvec_erase_str(cvector_str* vec, size_t start, size_t end);
int cvec_reserve_str(cvector_str* vec, size_t size);
int cvec_set_cap_str(cvector_str* vec, size_t size);
void cvec_set_val_sz_str(cvector_str* vec, char* val);
void cvec_set_val_cap_str(cvector_str* vec, char* val);

char** cvec_back_str(cvector_str* vec);

void cvec_clear_str(cvector_str* vec);
void cvec_free_str_heap(void* vec);
void cvec_free_str(void* vec);

#ifdef __cplusplus
}
#endif


#endif
