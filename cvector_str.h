#ifndef CVECTORSTR_H
#define CVECTORSTR_H

#include <stdlib.h>

#ifndef CVEC_SIZE_T
#define CVEC_SIZE_T size_t
#endif

#ifndef CVEC_SZ
#define CVEC_SZ
typedef CVEC_SIZE_T cvec_sz;
#endif

#ifdef __cplusplus
extern "C" {
#endif

/** Data structure for string vector. */
typedef struct cvector_str
{
	char** a;          /**< Array. */
	cvec_sz size;       /**< Current size (amount you use when manipulating array directly). */
	cvec_sz capacity;   /**< Allocated size of array; always >= size. */
} cvector_str;

extern cvec_sz CVEC_STR_START_SZ;

#ifndef CVEC_STRDUP
#define CVEC_STRDUP cvec_strdup
char* cvec_strdup(const char* str);
#endif

int cvec_str(cvector_str* vec, cvec_sz size, cvec_sz capacity);
int cvec_init_str(cvector_str* vec, char** vals, cvec_sz num);

cvector_str* cvec_str_heap(cvec_sz size, cvec_sz capacity);
cvector_str* cvec_init_str_heap(char** vals, cvec_sz num);
int cvec_copyc_str(void* dest, void* src);
int cvec_copy_str(cvector_str* dest, cvector_str* src);

int cvec_push_str(cvector_str* vec, char* a);
void cvec_pop_str(cvector_str* vec, char* ret);

int cvec_pushm_str(cvector_str* vec, char* a);
#define cvec_popm_str(vec) (vec).a[--(vec).size]
int cvec_insertm_str(cvector_str* vec, cvec_sz i, char* a);
int cvec_insert_arraym_str(cvector_str* vec, cvec_sz i, char** a, cvec_sz num);
#define cvec_replacem_str(vec, i, s, ret) ((ret) = (vec).a[i], (vec).a[i] = (s))

int cvec_extend_str(cvector_str* vec, cvec_sz num);
int cvec_insert_str(cvector_str* vec, cvec_sz i, char* a);
int cvec_insert_array_str(cvector_str* vec, cvec_sz i, char** a, cvec_sz num);
void cvec_replace_str(cvector_str* vec, cvec_sz i, char* a, char* ret);
void cvec_erase_str(cvector_str* vec, cvec_sz start, cvec_sz end);
void cvec_remove_str(cvector_str* vec, cvec_sz start, cvec_sz end);
int cvec_reserve_str(cvector_str* vec, cvec_sz size);
int cvec_set_cap_str(cvector_str* vec, cvec_sz size);
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
