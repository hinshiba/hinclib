#ifndef HINC_ITER_H
#define HINC_ITER_H

#include <stdbool.h>
#include <stddef.h>

#include "datastructure/base/hinc_list.h"
#include "datastructure/base/hinc_vec.h"

typedef struct _Iter _Iter;

enum _IterConType {
    _ITER_NO,
    _ITER_VEC,
    _ITER_LIST,
};

union _IterCon {
    _Vec *vec;
    _Node *list_node;
};

struct _Iter {
    void *ref;
    bool has_next;
    bool has_prev;
    union _IterCon _con;
    enum _IterConType _con_type;
    bool (*inext)(_Iter *);
    bool (*iprev)(_Iter *);
};

bool _iter_has_next(_Iter *iter);
void _iter_next(_Iter *iter);

bool _iter_has_prev(_Iter *iter);
void _iter_prev(_Iter *iter);

void iter_free(void *iter);

#define iter_def(Type)                      \
    typedef struct iter_##Type iter_##Type; \
                                            \
    struct iter_##Type {                    \
        Type *ref;                          \
        bool has_next;                      \
        bool has_prev;                      \
        union _IterCon _con;                \
        enum _IterConType _con_type;        \
        void (*inext)(iter_##Type *);       \
        void (*iprev)(iter_##Type *);       \
    };

// #define iter_def_for_vec(Type)

#define iter_def_for_list(Type)                               \
    iter_##Type iter_##Type##_list_begin(list_##Type *list) { \
        iter_##Type iter = {                                  \
            .ref = &list->head->data,                         \
            .has_next = list->len > 0,                        \
            .has_prev = list->len > 0,                        \
            ._con.list_node = (_Node *)list->head,            \
            ._con_type = _ITER_LIST,                          \
            .inext = (void (*)(iter_##Type *))_iter_next,     \
            .iprev = (void (*)(iter_##Type *))_iter_prev,     \
        };                                                    \
        return iter;                                          \
    }                                                         \
                                                              \
    iter_##Type iter_##Type##_list_end(list_##Type *list) {   \
        iter_##Type iter = {                                  \
            .ref = &list->tail->data,                         \
            .has_next = list->len > 0,                        \
            .has_prev = list->len > 0,                        \
            ._con.list_node = (_Node *)list->tail,            \
            ._con_type = _ITER_LIST,                          \
            .inext = (void (*)(iter_##Type *))_iter_next,     \
            .iprev = (void (*)(iter_##Type *))_iter_prev,     \
        };                                                    \
                                                              \
        return iter;                                          \
    }

#define iter_next(iter) iter.inext(&iter)
#define iter_prev(iter) iter.iprev(&iter)

#endif  // HINC_ITER_H
