#ifndef HINC_VEC_H
#define HINC_VEC_H

#include <stdbool.h>
#include <stddef.h>

//==============================================================================
// Vec
//==============================================================================

typedef struct {
    void *data;
    size_t data_size;
    size_t len;   /// 要素数
    size_t size;  /// 割り当て済みサイズ
} _Vec;

_Vec *_vec_new(size_t data_size, size_t len);
_Vec *_vec_from(void *data, size_t data_size, size_t len);

// void vec_free(Vec *vec);
size_t _vec_get(const _Vec *vec, size_t idx);
// void *vec_set(Vec *vec, size_t index, void *data);
// int vec_push(Vec *vec, void *data);
// void *vec_pop(Vec *vec);
// int vec_insert(Vec *vec, size_t index, void *data);
// void *vec_remove(Vec *vec, size_t index);
// // int vec_reserve(Vec *vec, size_t len);
// int vec_resize(Vec *vec, size_t len);
// int vec_shrink(Vec *vec);
// Vec *vec_copy(Vec *vec);
// Vec *vec_slice(Vec *vec, size_t start, size_t end);
// int vec_splice(Vec *vec, size_t start, size_t count, void *data, size_t num);
// int vec_concat(Vec *vec1, Vec *vec2);
// int vec_clear(Vec *vec);
// int vec_sort(Vec *vec, int (*cmp)(const void *, const void *));
// int vec_bsearch(Vec *vec, const void *key,
//                 int (*cmp)(const void *, const void *));
// int vec_reverse(Vec *vec);
// int vec_index_of(Vec *vec, void *data, int (*cmp)(const void *, const void
// *)); int vec_last_index_of(Vec *vec, void *data,
//                       int (*cmp)(const void *, const void *));
// int vec_for_each(Vec *vec, void (*fn)(void *, size_t, void *), void
// *user_data); Vec *vec_map(Vec *vec, void *(*fn)(void *, size_t, void *), void
// *user_data); Vec *vec_filter(Vec *vec, bool (*fn)(void *, size_t, void *),
// void *user_data); void *vec_reduce(Vec *vec, void *(*fn)(void *, void *,
// size_t, void *),
//                  void *init, void *user_data);
// bool vec_every(Vec *vec, bool (*fn)(void *, size_t, void *), void
// *user_data); bool vec_some(Vec *vec, bool (*fn)(void *, size_t, void *), void
// *user_data); Vec *vec_find(Vec *vec, bool (*fn)(void *, size_t, void *), void
// *user_data); int vec_find_index(Vec *vec, bool (*fn)(void *, size_t, void *),
//                    void *user_data);
// int vec_fill(Vec *vec, void *data, size_t start, size_t end);
// bool vec_includes(Vec *vec, void *data, int (*cmp)(const void *, const void
// *)); char *vec_join(Vec *vec, const char *sep); Vec *vec_from(void *data,
// size_t len, size_t data_size); Vec *vec_of(size_t data_size, int count, ...);

#define vec_def(Type)                                            \
    typedef struct {                                             \
        Type *data;                                              \
        size_t data_size;                                        \
        size_t len;                                              \
        size_t size;                                             \
    } vec_##Type;                                                \
    vec_##Type *vec_##Type##_newType(size_t len) {               \
        return (vec_##Type *)_vec_new(sizeof(Type), len);        \
    }                                                            \
    vec_##Type *vec_##Type##_from(Type data[], size_t len) {     \
        return (vec_##Type *)_vec_from(data, sizeof(Type), len); \
    }                                                            \
    const Type *vec_##Type##_get(vec_##Type *vec, size_t idx) {  \
        return &vec->data[_vec_get((_Vec *)vec, idx)];           \
    }

#endif  // HINC_VEC_H
