#ifndef HINC_VEC_H
#define HINC_VEC_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
    void *data;
    size_t data_size;
    size_t len;   /// 要素数
    size_t size;  /// 割り当て済み要素数
} _Vec;

_Vec *_vec_new(size_t data_size, size_t len);
_Vec *_vec_from(void *data, size_t data_size, size_t len);
void vec_free(void *vec);

size_t _vec_get(const _Vec *vec, size_t idx);
size_t _vec_get_mut(_Vec *vec, size_t idx);

size_t _vec_push(_Vec *vec);
size_t _vec_pop(_Vec *vec);

void _vec_resize(_Vec *vec, size_t len);
// void vec_shrink(_Vec *vec);

_Vec *_vec_cpy(_Vec *vec);
// void vec_reverse(_Vec *vec);
// void vec_concat(_Vec *vec1, _Vec *vec2);

// void vec_sort(Vec *vec, int (*cmp)(const void *, const void *));
// int vec_bsearch(Vec *vec, const void *key,
//                 int (*cmp)(const void *, const void *));
// int vec_index_of(Vec *vec, void *data, int (*cmp)(const void *, const void
// *));

// int vec_for_each(Vec *vec, void (*fn)(void *, size_t, void *), void
// *user_data); Vec *vec_map(Vec *vec, void *(*fn)(void *, size_t, void *), void
// *user_data); Vec *vec_filter(Vec *vec, bool (*fn)(void *, size_t, void *),
// void *user_data); void *vec_reduce(Vec *vec, void *(*fn)(void *, void *,
// size_t, void *),
//                  void *init, void *user_data);
// bool vec_all(Vec *vec, bool (*fn)(void *, size_t, void *), void
// *user_data);bool vec_any(Vec *vec, bool (*fn)(void *, size_t, void *), void

// Vec *vec_slice(Vec *vec, size_t start, size_t end);

// int vec_insert(Vec *vec, size_t index, void *data);
// void *vec_remove(Vec *vec, size_t index);

#define vec_def(Type)                                                 \
    typedef struct {                                                  \
        Type *data;                                                   \
        size_t data_size;                                             \
        size_t len;                                                   \
        size_t size;                                                  \
    } vec_##Type;                                                     \
                                                                      \
    vec_##Type *vec_##Type##_new(size_t len) {                        \
        return (vec_##Type *)_vec_new(sizeof(Type), len);             \
    }                                                                 \
                                                                      \
    vec_##Type *vec_##Type##_from(Type data[], size_t len) {          \
        return (vec_##Type *)_vec_from(data, sizeof(Type), len);      \
    }                                                                 \
                                                                      \
    const Type *vec_##Type##_get(const vec_##Type *vec, size_t idx) { \
        return &vec->data[_vec_get((_Vec *)vec, idx)];                \
    }                                                                 \
                                                                      \
    Type *vec_##Type##_get_mut(vec_##Type *vec, size_t idx) {         \
        return &vec->data[_vec_get_mut((_Vec *)vec, idx)];            \
    }                                                                 \
                                                                      \
    void vec_##Type##_push(vec_##Type *vec, const Type elem) {        \
        vec->data[_vec_push((_Vec *)vec)] = elem;                     \
        return;                                                       \
    }                                                                 \
                                                                      \
    Type vec_##Type##_pop(vec_##Type *vec) {                          \
        return vec->data[_vec_pop((_Vec *)vec)];                      \
    }                                                                 \
                                                                      \
    void vec_##Type##_resize(vec_##Type *vec, size_t len) {           \
        return _vec_resize((_Vec *)vec, len);                         \
    }                                                                 \
                                                                      \
    vec_##Type *vec_##Type##_cpy(vec_##Type *vec) {                   \
        return (vec_##Type *)_vec_cpy((_Vec *)vec);                   \
    }

#endif  // HINC_VEC_H
