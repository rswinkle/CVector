#ifndef CVECTOR_MACRO_H
#define CVECTOR_MACRO_H

/* size_t, malloc/realloc/free */
#include <stdlib.h>
/* memmove */
#include <string.h>
/* assert */
#include <assert.h>

/*
 User can optionally wrap CVEC_NEW_DECLS(2) with extern "C"
 since it's not part of the macro
*/

#define CVEC_NEW_DECLS(TYPE)                                                          \
	typedef struct cvector_##TYPE {                                                   \
		TYPE* a;                                                                      \
		size_t size;                                                                  \
		size_t capacity;                                                              \
	} cvector_##TYPE;                                                                 \
                                                                                      \
	extern size_t CVEC_##TYPE##_SZ;                                                   \
                                                                                      \
	int cvec_##TYPE(cvector_##TYPE* vec, size_t size, size_t capacity);               \
	int cvec_init_##TYPE(cvector_##TYPE* vec, TYPE* vals, size_t num);                \
                                                                                      \
	cvector_##TYPE* cvec_##TYPE##_heap(size_t size, size_t capacity);                 \
	cvector_##TYPE* cvec_init_##TYPE##_heap(TYPE* vals, size_t num);                  \
                                                                                      \
	void cvec_##TYPE##_copy(void* dest, void* src);                                   \
                                                                                      \
	int cvec_push_##TYPE(cvector_##TYPE* vec, TYPE a);                                \
	TYPE cvec_pop_##TYPE(cvector_##TYPE* vec);                                        \
                                                                                      \
	int cvec_extend_##TYPE(cvector_##TYPE* vec, size_t num);                          \
	int cvec_insert_##TYPE(cvector_##TYPE* vec, size_t i, TYPE a);                    \
	int cvec_insert_array_##TYPE(cvector_##TYPE* vec, size_t i, TYPE* a, size_t num); \
	TYPE cvec_replace_##TYPE(cvector_##TYPE* vec, size_t i, TYPE a);                  \
	void cvec_erase_##TYPE(cvector_##TYPE* vec, size_t start, size_t end);            \
	int cvec_reserve_##TYPE(cvector_##TYPE* vec, size_t size);                        \
	int cvec_set_cap_##TYPE(cvector_##TYPE* vec, size_t size);                        \
	void cvec_set_val_sz_##TYPE(cvector_##TYPE* vec, TYPE val);                       \
	void cvec_set_val_cap_##TYPE(cvector_##TYPE* vec, TYPE val);                      \
                                                                                      \
	TYPE* cvec_back_##TYPE(cvector_##TYPE* vec);                                      \
                                                                                      \
	void cvec_clear_##TYPE(cvector_##TYPE* vec);                                      \
	void cvec_free_##TYPE##_heap(void* vec);                                          \
	void cvec_free_##TYPE(void* vec);

#define CVEC_NEW_DEFS(TYPE, RESIZE_MACRO)                                                    \
	size_t CVEC_##TYPE##_SZ = 50;                                                            \
                                                                                             \
	cvector_##TYPE* cvec_##TYPE##_heap(size_t size, size_t capacity)                         \
	{                                                                                        \
		cvector_##TYPE* vec;                                                                 \
		if (!(vec = (cvector_##TYPE*)malloc(sizeof(cvector_##TYPE)))) {                      \
			assert(vec != NULL);                                                             \
			return NULL;                                                                     \
		}                                                                                    \
                                                                                             \
		vec->size     = size;                                                                \
		vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size))       \
		                    ? capacity                                                       \
		                    : vec->size + CVEC_##TYPE##_SZ;                                  \
                                                                                             \
		if (!(vec->a = (TYPE*)malloc(vec->capacity * sizeof(TYPE)))) {                       \
			assert(vec->a != NULL);                                                          \
			free(vec);                                                                       \
			return NULL;                                                                     \
		}                                                                                    \
                                                                                             \
		return vec;                                                                          \
	}                                                                                        \
                                                                                             \
	cvector_##TYPE* cvec_init_##TYPE##_heap(TYPE* vals, size_t num)                          \
	{                                                                                        \
		cvector_##TYPE* vec;                                                                 \
                                                                                             \
		if (!(vec = (cvector_##TYPE*)malloc(sizeof(cvector_##TYPE)))) {                      \
			assert(vec != NULL);                                                             \
			return NULL;                                                                     \
		}                                                                                    \
                                                                                             \
		vec->capacity = num + CVEC_##TYPE##_SZ;                                              \
		vec->size     = num;                                                                 \
		if (!(vec->a = (TYPE*)malloc(vec->capacity * sizeof(TYPE)))) {                       \
			assert(vec->a != NULL);                                                          \
			free(vec);                                                                       \
			return NULL;                                                                     \
		}                                                                                    \
                                                                                             \
		memmove(vec->a, vals, sizeof(TYPE) * num);                                           \
                                                                                             \
		return vec;                                                                          \
	}                                                                                        \
                                                                                             \
	int cvec_##TYPE(cvector_##TYPE* vec, size_t size, size_t capacity)                       \
	{                                                                                        \
		vec->size     = size;                                                                \
		vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size))       \
		                    ? capacity                                                       \
		                    : vec->size + CVEC_##TYPE##_SZ;                                  \
                                                                                             \
		if (!(vec->a = (TYPE*)malloc(vec->capacity * sizeof(TYPE)))) {                       \
			assert(vec->a != NULL);                                                          \
			vec->size = vec->capacity = 0;                                                   \
			return 0;                                                                        \
		}                                                                                    \
                                                                                             \
		return 1;                                                                            \
	}                                                                                        \
                                                                                             \
	int cvec_init_##TYPE(cvector_##TYPE* vec, TYPE* vals, size_t num)                        \
	{                                                                                        \
		vec->capacity = num + CVEC_##TYPE##_SZ;                                              \
		vec->size     = num;                                                                 \
		if (!(vec->a = (TYPE*)malloc(vec->capacity * sizeof(TYPE)))) {                       \
			assert(vec->a != NULL);                                                          \
			vec->size = vec->capacity = 0;                                                   \
			return 0;                                                                        \
		}                                                                                    \
                                                                                             \
		memmove(vec->a, vals, sizeof(TYPE) * num);                                           \
                                                                                             \
		return 1;                                                                            \
	}                                                                                        \
                                                                                             \
	void cvec_##TYPE##_copy(void* dest, void* src)                                           \
	{                                                                                        \
		cvector_##TYPE* vec1 = (cvector_##TYPE*)dest;                                        \
		cvector_##TYPE* vec2 = (cvector_##TYPE*)src;                                         \
                                                                                             \
		vec1->size     = 0;                                                                  \
		vec1->capacity = 0;                                                                  \
                                                                                             \
		/*not much else we can do here*/                                                     \
		if (!(vec1->a = (TYPE*)malloc(vec2->capacity * sizeof(TYPE)))) {                     \
			assert(vec1->a != NULL);                                                         \
			return;                                                                          \
		}                                                                                    \
                                                                                             \
		memmove(vec1->a, vec2->a, vec2->size * sizeof(TYPE));                                \
		vec1->size     = vec2->size;                                                         \
		vec1->capacity = vec2->capacity;                                                     \
	}                                                                                        \
                                                                                             \
	int cvec_push_##TYPE(cvector_##TYPE* vec, TYPE a)                                        \
	{                                                                                        \
		TYPE* tmp;                                                                           \
		size_t tmp_sz;                                                                       \
		if (vec->capacity > vec->size) {                                                     \
			vec->a[vec->size++] = a;                                                         \
		} else {                                                                             \
			tmp_sz = RESIZE_MACRO(vec->capacity);                                            \
			if (!(tmp = (TYPE*)realloc(vec->a, sizeof(TYPE) * tmp_sz))) {                    \
				assert(tmp != NULL);                                                         \
				return 0;                                                                    \
			}                                                                                \
			vec->a              = tmp;                                                       \
			vec->a[vec->size++] = a;                                                         \
			vec->capacity       = tmp_sz;                                                    \
		}                                                                                    \
		return 1;                                                                            \
	}                                                                                        \
                                                                                             \
	TYPE cvec_pop_##TYPE(cvector_##TYPE* vec) { return vec->a[--vec->size]; }                \
                                                                                             \
	TYPE* cvec_back_##TYPE(cvector_##TYPE* vec) { return &vec->a[vec->size - 1]; }           \
                                                                                             \
	int cvec_extend_##TYPE(cvector_##TYPE* vec, size_t num)                                  \
	{                                                                                        \
		TYPE* tmp;                                                                           \
		size_t tmp_sz;                                                                       \
		if (vec->capacity < vec->size + num) {                                               \
			tmp_sz = vec->capacity + num + CVEC_##TYPE##_SZ;                                 \
			if (!(tmp = (TYPE*)realloc(vec->a, sizeof(TYPE) * tmp_sz))) {                    \
				assert(tmp != NULL);                                                         \
				return 0;                                                                    \
			}                                                                                \
			vec->a        = tmp;                                                             \
			vec->capacity = tmp_sz;                                                          \
		}                                                                                    \
                                                                                             \
		vec->size += num;                                                                    \
		return 1;                                                                            \
	}                                                                                        \
                                                                                             \
	int cvec_insert_##TYPE(cvector_##TYPE* vec, size_t i, TYPE a)                            \
	{                                                                                        \
		TYPE* tmp;                                                                           \
		size_t tmp_sz;                                                                       \
		if (vec->capacity > vec->size) {                                                     \
			memmove(&vec->a[i + 1], &vec->a[i], (vec->size - i) * sizeof(TYPE));             \
			vec->a[i] = a;                                                                   \
		} else {                                                                             \
			tmp_sz = RESIZE_MACRO(vec->capacity);                                            \
			if (!(tmp = (TYPE*)realloc(vec->a, sizeof(TYPE) * tmp_sz))) {                    \
				assert(tmp != NULL);                                                         \
				return 0;                                                                    \
			}                                                                                \
			vec->a = tmp;                                                                    \
			memmove(&vec->a[i + 1], &vec->a[i], (vec->size - i) * sizeof(TYPE));             \
			vec->a[i]     = a;                                                               \
			vec->capacity = tmp_sz;                                                          \
		}                                                                                    \
                                                                                             \
		vec->size++;                                                                         \
		return 1;                                                                            \
	}                                                                                        \
                                                                                             \
	int cvec_insert_array_##TYPE(cvector_##TYPE* vec, size_t i, TYPE* a, size_t num)         \
	{                                                                                        \
		TYPE* tmp;                                                                           \
		size_t tmp_sz;                                                                       \
		if (vec->capacity < vec->size + num) {                                               \
			tmp_sz = vec->capacity + num + CVEC_##TYPE##_SZ;                                 \
			if (!(tmp = (TYPE*)realloc(vec->a, sizeof(TYPE) * tmp_sz))) {                    \
				assert(tmp != NULL);                                                         \
				return 0;                                                                    \
			}                                                                                \
			vec->a        = tmp;                                                             \
			vec->capacity = tmp_sz;                                                          \
		}                                                                                    \
                                                                                             \
		memmove(&vec->a[i + num], &vec->a[i], (vec->size - i) * sizeof(TYPE));               \
		memmove(&vec->a[i], a, num * sizeof(TYPE));                                          \
		vec->size += num;                                                                    \
		return 1;                                                                            \
	}                                                                                        \
                                                                                             \
	TYPE cvec_replace_##TYPE(cvector_##TYPE* vec, size_t i, TYPE a)                          \
	{                                                                                        \
		TYPE tmp  = vec->a[i];                                                               \
		vec->a[i] = a;                                                                       \
		return tmp;                                                                          \
	}                                                                                        \
                                                                                             \
	void cvec_erase_##TYPE(cvector_##TYPE* vec, size_t start, size_t end)                    \
	{                                                                                        \
		size_t d = end - start + 1;                                                          \
		memmove(&vec->a[start], &vec->a[end + 1], (vec->size - 1 - end) * sizeof(TYPE));     \
		vec->size -= d;                                                                      \
	}                                                                                        \
                                                                                             \
	int cvec_reserve_##TYPE(cvector_##TYPE* vec, size_t size)                                \
	{                                                                                        \
		TYPE* tmp;                                                                           \
		if (vec->capacity < size) {                                                          \
			if (!(tmp = (TYPE*)realloc(vec->a, sizeof(TYPE) * (size + CVEC_##TYPE##_SZ)))) { \
				assert(tmp != NULL);                                                         \
				return 0;                                                                    \
			}                                                                                \
			vec->a        = tmp;                                                             \
			vec->capacity = size + CVEC_##TYPE##_SZ;                                         \
		}                                                                                    \
		return 1;                                                                            \
	}                                                                                        \
                                                                                             \
	int cvec_set_cap_##TYPE(cvector_##TYPE* vec, size_t size)                                \
	{                                                                                        \
		TYPE* tmp;                                                                           \
		if (size < vec->size) {                                                              \
			vec->size = size;                                                                \
		}                                                                                    \
                                                                                             \
		if (!(tmp = (TYPE*)realloc(vec->a, sizeof(TYPE) * size))) {                          \
			assert(tmp != NULL);                                                             \
			return 0;                                                                        \
		}                                                                                    \
		vec->a        = tmp;                                                                 \
		vec->capacity = size;                                                                \
		return 1;                                                                            \
	}                                                                                        \
                                                                                             \
	void cvec_set_val_sz_##TYPE(cvector_##TYPE* vec, TYPE val)                               \
	{                                                                                        \
		size_t i;                                                                            \
		for (i = 0; i < vec->size; i++) {                                                    \
			vec->a[i] = val;                                                                 \
		}                                                                                    \
	}                                                                                        \
                                                                                             \
	void cvec_set_val_cap_##TYPE(cvector_##TYPE* vec, TYPE val)                              \
	{                                                                                        \
		size_t i;                                                                            \
		for (i = 0; i < vec->capacity; i++) {                                                \
			vec->a[i] = val;                                                                 \
		}                                                                                    \
	}                                                                                        \
                                                                                             \
	void cvec_clear_##TYPE(cvector_##TYPE* vec) { vec->size = 0; }                           \
                                                                                             \
	void cvec_free_##TYPE##_heap(void* vec)                                                  \
	{                                                                                        \
		cvector_##TYPE* tmp = (cvector_##TYPE*)vec;                                          \
		free(tmp->a);                                                                        \
		free(tmp);                                                                           \
	}                                                                                        \
                                                                                             \
	void cvec_free_##TYPE(void* vec)                                                         \
	{                                                                                        \
		cvector_##TYPE* tmp = (cvector_##TYPE*)vec;                                          \
		free(tmp->a);                                                                        \
		tmp->size     = 0;                                                                   \
		tmp->capacity = 0;                                                                   \
	}

#define CVEC_NEW_DECLS2(TYPE)                                                                    \
	typedef struct cvector_##TYPE {                                                              \
		TYPE* a;                                                                                 \
		size_t size;                                                                             \
		size_t capacity;                                                                         \
		void (*elem_init)(void*, void*);                                                         \
		void (*elem_free)(void*);                                                                \
	} cvector_##TYPE;                                                                            \
                                                                                                 \
	extern size_t CVEC_##TYPE##_SZ;                                                              \
                                                                                                 \
	int cvec_##TYPE(cvector_##TYPE* vec, size_t size, size_t capacity, void (*elem_free)(void*), \
	                void (*elem_init)(void*, void*));                                            \
	int cvec_init_##TYPE(cvector_##TYPE* vec, TYPE* vals, size_t num, void (*elem_free)(void*),  \
	                     void (*elem_init)(void*, void*));                                       \
                                                                                                 \
	cvector_##TYPE* cvec_##TYPE##_heap(size_t size, size_t capacity, void (*elem_free)(void*),   \
	                                   void (*elem_init)(void*, void*));                         \
	cvector_##TYPE* cvec_init_##TYPE##_heap(TYPE* vals, size_t num, void (*elem_free)(void*),    \
	                                        void (*elem_init)(void*, void*));                    \
                                                                                                 \
	void cvec_##TYPE##_copy(void* dest, void* src);                                              \
                                                                                                 \
	int cvec_push_##TYPE(cvector_##TYPE* vec, TYPE* val);                                        \
	void cvec_pop_##TYPE(cvector_##TYPE* vec, TYPE* ret);                                        \
                                                                                                 \
	int cvec_extend_##TYPE(cvector_##TYPE* vec, size_t num);                                     \
	int cvec_insert_##TYPE(cvector_##TYPE* vec, size_t i, TYPE* a);                              \
	int cvec_insert_array_##TYPE(cvector_##TYPE* vec, size_t i, TYPE* a, size_t num);            \
	void cvec_replace_##TYPE(cvector_##TYPE* vec, size_t i, TYPE* a, TYPE* ret);                 \
	void cvec_erase_##TYPE(cvector_##TYPE* vec, size_t start, size_t end);                       \
	int cvec_reserve_##TYPE(cvector_##TYPE* vec, size_t size);                                   \
	int cvec_set_cap_##TYPE(cvector_##TYPE* vec, size_t size);                                   \
	void cvec_set_val_sz_##TYPE(cvector_##TYPE* vec, TYPE* val);                                 \
	void cvec_set_val_cap_##TYPE(cvector_##TYPE* vec, TYPE* val);                                \
                                                                                                 \
	TYPE* cvec_back_##TYPE(cvector_##TYPE* vec);                                                 \
                                                                                                 \
	void cvec_clear_##TYPE(cvector_##TYPE* vec);                                                 \
	void cvec_free_##TYPE##_heap(void* vec);                                                     \
	void cvec_free_##TYPE(void* vec);

#define CVEC_NEW_DEFS2(TYPE, RESIZE_MACRO)                                                       \
	size_t CVEC_##TYPE##_SZ = 20;                                                                \
                                                                                                 \
	cvector_##TYPE* cvec_##TYPE##_heap(size_t size, size_t capacity, void (*elem_free)(void*),   \
	                                   void (*elem_init)(void*, void*))                          \
	{                                                                                            \
		cvector_##TYPE* vec;                                                                     \
		if (!(vec = (cvector_##TYPE*)malloc(sizeof(cvector_##TYPE)))) {                          \
			assert(vec != NULL);                                                                 \
			return NULL;                                                                         \
		}                                                                                        \
                                                                                                 \
		vec->size     = size;                                                                    \
		vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size))           \
		                    ? capacity                                                           \
		                    : vec->size + CVEC_##TYPE##_SZ;                                      \
                                                                                                 \
		/*not calloc here and init_vec as in vector_s because elem_free cannot be                \
		 * calling free directly*/                                                               \
		if (!(vec->a = (TYPE*)malloc(vec->capacity * sizeof(TYPE)))) {                           \
			assert(vec->a != NULL);                                                              \
			free(vec);                                                                           \
			return NULL;                                                                         \
		}                                                                                        \
                                                                                                 \
		vec->elem_free = elem_free;                                                              \
		vec->elem_init = elem_init;                                                              \
                                                                                                 \
		return vec;                                                                              \
	}                                                                                            \
                                                                                                 \
	cvector_##TYPE* cvec_init_##TYPE##_heap(TYPE* vals, size_t num, void (*elem_free)(void*),    \
	                                        void (*elem_init)(void*, void*))                     \
	{                                                                                            \
		cvector_##TYPE* vec;                                                                     \
		size_t i;                                                                                \
                                                                                                 \
		if (!(vec = (cvector_##TYPE*)malloc(sizeof(cvector_##TYPE)))) {                          \
			assert(vec != NULL);                                                                 \
			return NULL;                                                                         \
		}                                                                                        \
                                                                                                 \
		vec->capacity = num + CVEC_##TYPE##_SZ;                                                  \
		vec->size     = num;                                                                     \
		if (!(vec->a = (TYPE*)malloc(vec->capacity * sizeof(TYPE)))) {                           \
			assert(vec->a != NULL);                                                              \
			free(vec);                                                                           \
			return NULL;                                                                         \
		}                                                                                        \
                                                                                                 \
		if (elem_init) {                                                                         \
			for (i = 0; i < num; ++i) {                                                          \
				elem_init(&vec->a[i], &vals[i]);                                                 \
			}                                                                                    \
		} else {                                                                                 \
			memmove(vec->a, vals, sizeof(TYPE) * num);                                           \
		}                                                                                        \
                                                                                                 \
		vec->elem_free = elem_free;                                                              \
		vec->elem_init = elem_init;                                                              \
                                                                                                 \
		return vec;                                                                              \
	}                                                                                            \
                                                                                                 \
	int cvec_##TYPE(cvector_##TYPE* vec, size_t size, size_t capacity, void (*elem_free)(void*), \
	                void (*elem_init)(void*, void*))                                             \
	{                                                                                            \
		vec->size     = size;                                                                    \
		vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size))           \
		                    ? capacity                                                           \
		                    : vec->size + CVEC_##TYPE##_SZ;                                      \
                                                                                                 \
		if (!(vec->a = (TYPE*)malloc(vec->capacity * sizeof(TYPE)))) {                           \
			assert(vec->a != NULL);                                                              \
			vec->size = vec->capacity = 0;                                                       \
			return 0;                                                                            \
		}                                                                                        \
                                                                                                 \
		vec->elem_free = elem_free;                                                              \
		vec->elem_init = elem_init;                                                              \
                                                                                                 \
		return 1;                                                                                \
	}                                                                                            \
                                                                                                 \
	int cvec_init_##TYPE(cvector_##TYPE* vec, TYPE* vals, size_t num, void (*elem_free)(void*),  \
	                     void (*elem_init)(void*, void*))                                        \
	{                                                                                            \
		size_t i;                                                                                \
                                                                                                 \
		vec->capacity = num + CVEC_##TYPE##_SZ;                                                  \
		vec->size     = num;                                                                     \
		if (!(vec->a = (TYPE*)malloc(vec->capacity * sizeof(TYPE)))) {                           \
			assert(vec->a != NULL);                                                              \
			vec->size = vec->capacity = 0;                                                       \
			return 0;                                                                            \
		}                                                                                        \
                                                                                                 \
		if (elem_init) {                                                                         \
			for (i = 0; i < num; ++i) {                                                          \
				elem_init(&vec->a[i], &vals[i]);                                                 \
			}                                                                                    \
		} else {                                                                                 \
			memmove(vec->a, vals, sizeof(TYPE) * num);                                           \
		}                                                                                        \
                                                                                                 \
		vec->elem_free = elem_free;                                                              \
		vec->elem_init = elem_init;                                                              \
                                                                                                 \
		return 1;                                                                                \
	}                                                                                            \
                                                                                                 \
	void cvec_##TYPE##_copy(void* dest, void* src)                                               \
	{                                                                                            \
		size_t i;                                                                                \
		cvector_##TYPE* vec1 = (cvector_##TYPE*)dest;                                            \
		cvector_##TYPE* vec2 = (cvector_##TYPE*)src;                                             \
                                                                                                 \
		vec1->size     = 0;                                                                      \
		vec1->capacity = 0;                                                                      \
                                                                                                 \
		/*not much else we can do here*/                                                         \
		if (!(vec1->a = (TYPE*)malloc(vec2->capacity * sizeof(TYPE)))) {                         \
			assert(vec1->a != NULL);                                                             \
			return;                                                                              \
		}                                                                                        \
                                                                                                 \
		vec1->size      = vec2->size;                                                            \
		vec1->capacity  = vec2->capacity;                                                        \
		vec1->elem_init = vec2->elem_init;                                                       \
		vec1->elem_free = vec2->elem_free;                                                       \
                                                                                                 \
		if (vec1->elem_init) {                                                                   \
			for (i = 0; i < vec1->size; ++i) {                                                   \
				vec1->elem_init(&vec1->a[i], &vec2->a[i]);                                       \
			}                                                                                    \
		} else {                                                                                 \
			memmove(vec1->a, vec2->a, vec1->size * sizeof(TYPE));                                \
		}                                                                                        \
	}                                                                                            \
                                                                                                 \
	int cvec_push_##TYPE(cvector_##TYPE* vec, TYPE* a)                                           \
	{                                                                                            \
		TYPE* tmp;                                                                               \
		size_t tmp_sz;                                                                           \
		if (vec->capacity == vec->size) {                                                        \
			tmp_sz = RESIZE_MACRO(vec->capacity);                                                \
			if (!(tmp = (TYPE*)realloc(vec->a, sizeof(TYPE) * tmp_sz))) {                        \
				assert(tmp != NULL);                                                             \
				return 0;                                                                        \
			}                                                                                    \
			vec->a        = tmp;                                                                 \
			vec->capacity = tmp_sz;                                                              \
		}                                                                                        \
		if (vec->elem_init) {                                                                    \
			vec->elem_init(&vec->a[vec->size], a);                                               \
		} else {                                                                                 \
			memmove(&vec->a[vec->size], a, sizeof(TYPE));                                        \
		}                                                                                        \
                                                                                                 \
		vec->size++;                                                                             \
		return 1;                                                                                \
	}                                                                                            \
                                                                                                 \
	void cvec_pop_##TYPE(cvector_##TYPE* vec, TYPE* ret)                                         \
	{                                                                                            \
		if (ret) {                                                                               \
			memmove(ret, &vec->a[--vec->size], sizeof(TYPE));                                    \
		} else {                                                                                 \
			vec->size--;                                                                         \
		}                                                                                        \
                                                                                                 \
		if (vec->elem_free) {                                                                    \
			vec->elem_free(&vec->a[vec->size]);                                                  \
		}                                                                                        \
	}                                                                                            \
                                                                                                 \
	TYPE* cvec_back_##TYPE(cvector_##TYPE* vec) { return &vec->a[vec->size - 1]; }               \
                                                                                                 \
	int cvec_extend_##TYPE(cvector_##TYPE* vec, size_t num)                                      \
	{                                                                                            \
		TYPE* tmp;                                                                               \
		size_t tmp_sz;                                                                           \
		if (vec->capacity < vec->size + num) {                                                   \
			tmp_sz = vec->capacity + num + CVEC_##TYPE##_SZ;                                     \
			if (!(tmp = (TYPE*)realloc(vec->a, sizeof(TYPE) * tmp_sz))) {                        \
				assert(tmp != NULL);                                                             \
				return 0;                                                                        \
			}                                                                                    \
			vec->a        = tmp;                                                                 \
			vec->capacity = tmp_sz;                                                              \
		}                                                                                        \
                                                                                                 \
		vec->size += num;                                                                        \
		return 1;                                                                                \
	}                                                                                            \
                                                                                                 \
	int cvec_insert_##TYPE(cvector_##TYPE* vec, size_t i, TYPE* a)                               \
	{                                                                                            \
		TYPE* tmp;                                                                               \
		size_t tmp_sz;                                                                           \
		if (vec->capacity == vec->size) {                                                        \
			tmp_sz = RESIZE_MACRO(vec->capacity);                                                \
			if (!(tmp = (TYPE*)realloc(vec->a, sizeof(TYPE) * tmp_sz))) {                        \
				assert(tmp != NULL);                                                             \
				return 0;                                                                        \
			}                                                                                    \
                                                                                                 \
			vec->a        = tmp;                                                                 \
			vec->capacity = tmp_sz;                                                              \
		}                                                                                        \
		memmove(&vec->a[i + 1], &vec->a[i], (vec->size - i) * sizeof(TYPE));                     \
                                                                                                 \
		if (vec->elem_init) {                                                                    \
			vec->elem_init(&vec->a[i], a);                                                       \
		} else {                                                                                 \
			memmove(&vec->a[i], a, sizeof(TYPE));                                                \
		}                                                                                        \
                                                                                                 \
		vec->size++;                                                                             \
		return 1;                                                                                \
	}                                                                                            \
                                                                                                 \
	int cvec_insert_array_##TYPE(cvector_##TYPE* vec, size_t i, TYPE* a, size_t num)             \
	{                                                                                            \
		TYPE* tmp;                                                                               \
		size_t tmp_sz, j;                                                                        \
		if (vec->capacity < vec->size + num) {                                                   \
			tmp_sz = vec->capacity + num + CVEC_##TYPE##_SZ;                                     \
			if (!(tmp = (TYPE*)realloc(vec->a, sizeof(TYPE) * tmp_sz))) {                        \
				assert(tmp != NULL);                                                             \
				return 0;                                                                        \
			}                                                                                    \
			vec->a        = tmp;                                                                 \
			vec->capacity = tmp_sz;                                                              \
		}                                                                                        \
                                                                                                 \
		memmove(&vec->a[i + num], &vec->a[i], (vec->size - i) * sizeof(TYPE));                   \
		if (vec->elem_init) {                                                                    \
			for (j = 0; j < num; ++j) {                                                          \
				vec->elem_init(&vec->a[j + i], &a[j]);                                           \
			}                                                                                    \
		} else {                                                                                 \
			memmove(&vec->a[i], a, num * sizeof(TYPE));                                          \
		}                                                                                        \
		vec->size += num;                                                                        \
		return 1;                                                                                \
	}                                                                                            \
                                                                                                 \
	void cvec_replace_##TYPE(cvector_##TYPE* vec, size_t i, TYPE* a, TYPE* ret)                  \
	{                                                                                            \
		if (ret) memmove(ret, &vec->a[i], sizeof(TYPE));                                         \
		memmove(&vec->a[i], a, sizeof(TYPE));                                                    \
	}                                                                                            \
                                                                                                 \
	void cvec_erase_##TYPE(cvector_##TYPE* vec, size_t start, size_t end)                        \
	{                                                                                            \
		size_t i;                                                                                \
		size_t d = end - start + 1;                                                              \
		if (vec->elem_free) {                                                                    \
			for (i = start; i <= end; i++) {                                                     \
				vec->elem_free(&vec->a[i]);                                                      \
			}                                                                                    \
		}                                                                                        \
		memmove(&vec->a[start], &vec->a[end + 1], (vec->size - 1 - end) * sizeof(TYPE));         \
		vec->size -= d;                                                                          \
	}                                                                                            \
                                                                                                 \
	int cvec_reserve_##TYPE(cvector_##TYPE* vec, size_t size)                                    \
	{                                                                                            \
		TYPE* tmp;                                                                               \
		if (vec->capacity < size) {                                                              \
			if (!(tmp = (TYPE*)realloc(vec->a, sizeof(TYPE) * (size + CVEC_##TYPE##_SZ)))) {     \
				assert(tmp != NULL);                                                             \
				return 0;                                                                        \
			}                                                                                    \
			vec->a        = tmp;                                                                 \
			vec->capacity = size + CVEC_##TYPE##_SZ;                                             \
		}                                                                                        \
		return 1;                                                                                \
	}                                                                                            \
                                                                                                 \
	int cvec_set_cap_##TYPE(cvector_##TYPE* vec, size_t size)                                    \
	{                                                                                            \
		size_t i;                                                                                \
		TYPE* tmp;                                                                               \
		if (size < vec->size) {                                                                  \
			if (vec->elem_free) {                                                                \
				for (i = vec->size - 1; i >= size; i--) {                                        \
					vec->elem_free(&vec->a[i]);                                                  \
				}                                                                                \
			}                                                                                    \
			vec->size = size;                                                                    \
		}                                                                                        \
                                                                                                 \
		vec->capacity = size;                                                                    \
                                                                                                 \
		if (!(tmp = (TYPE*)realloc(vec->a, sizeof(TYPE) * size))) {                              \
			assert(tmp != NULL);                                                                 \
			return 0;                                                                            \
		}                                                                                        \
		vec->a = tmp;                                                                            \
		return 1;                                                                                \
	}                                                                                            \
                                                                                                 \
	void cvec_set_val_sz_##TYPE(cvector_##TYPE* vec, TYPE* val)                                  \
	{                                                                                            \
		size_t i;                                                                                \
                                                                                                 \
		if (vec->elem_free) {                                                                    \
			for (i = 0; i < vec->size; i++) {                                                    \
				vec->elem_free(&vec->a[i]);                                                      \
			}                                                                                    \
		}                                                                                        \
                                                                                                 \
		if (vec->elem_init) {                                                                    \
			for (i = 0; i < vec->size; i++) {                                                    \
				vec->elem_init(&vec->a[i], val);                                                 \
			}                                                                                    \
		} else {                                                                                 \
			for (i = 0; i < vec->size; i++) {                                                    \
				memmove(&vec->a[i], val, sizeof(TYPE));                                          \
			}                                                                                    \
		}                                                                                        \
	}                                                                                            \
                                                                                                 \
	void cvec_set_val_cap_##TYPE(cvector_##TYPE* vec, TYPE* val)                                 \
	{                                                                                            \
		size_t i;                                                                                \
		if (vec->elem_free) {                                                                    \
			for (i = 0; i < vec->size; i++) {                                                    \
				vec->elem_free(&vec->a[i]);                                                      \
			}                                                                                    \
			vec->size = vec->capacity;                                                           \
		}                                                                                        \
                                                                                                 \
		if (vec->elem_init) {                                                                    \
			for (i = 0; i < vec->capacity; i++) {                                                \
				vec->elem_init(&vec->a[i], val);                                                 \
			}                                                                                    \
		} else {                                                                                 \
			for (i = 0; i < vec->capacity; i++) {                                                \
				memmove(&vec->a[i], val, sizeof(TYPE));                                          \
			}                                                                                    \
		}                                                                                        \
	}                                                                                            \
                                                                                                 \
	void cvec_clear_##TYPE(cvector_##TYPE* vec)                                                  \
	{                                                                                            \
		size_t i;                                                                                \
		if (vec->elem_free) {                                                                    \
			for (i = 0; i < vec->size; ++i) {                                                    \
				vec->elem_free(&vec->a[i]);                                                      \
			}                                                                                    \
		}                                                                                        \
		vec->size = 0;                                                                           \
	}                                                                                            \
                                                                                                 \
	void cvec_free_##TYPE##_heap(void* vec)                                                      \
	{                                                                                            \
		size_t i;                                                                                \
		cvector_##TYPE* tmp = (cvector_##TYPE*)vec;                                              \
		if (tmp->elem_free) {                                                                    \
			for (i = 0; i < tmp->size; i++) {                                                    \
				tmp->elem_free(&tmp->a[i]);                                                      \
			}                                                                                    \
		}                                                                                        \
		free(tmp->a);                                                                            \
		free(tmp);                                                                               \
	}                                                                                            \
                                                                                                 \
	void cvec_free_##TYPE(void* vec)                                                             \
	{                                                                                            \
		size_t i;                                                                                \
		cvector_##TYPE* tmp = (cvector_##TYPE*)vec;                                              \
		if (tmp->elem_free) {                                                                    \
			for (i = 0; i < tmp->size; i++) {                                                    \
				tmp->elem_free(&tmp->a[i]);                                                      \
			}                                                                                    \
		}                                                                                        \
                                                                                                 \
		free(tmp->a);                                                                            \
                                                                                                 \
		tmp->size     = 0;                                                                       \
		tmp->capacity = 0;                                                                       \
	}

#endif
