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
    union _IterCon _con;
    enum _IterConType _con_type;
    bool (*next)(_Iter *);
    bool (*prev)(_Iter *);
};

bool _iter_next(_Iter *iter);

_Iter *_iter_new(void);
void iter_free(void *iter);

#define iter_def(Type)                      \
    typedef struct iter_##Type iter_##Type; \
                                            \
    struct iter_##Type {                    \
        Type *ref;                          \
        union _IterCon _con;                \
        enum _IterConType _con_type;        \
        bool (*next)(iter_##Type *);        \
        bool (*prev)(iter_##Type *);        \
    };

// #define iter_def_for_vec(Type)

#define iter_def_for_list(Type)                                  \
    iter_##Type iter_##Type##_new_from_list(list_##Type *list) { \
        iter_##Type iter = {                                     \
            .ref = &list->head->data,                            \
            ._con.list_node = (_Node *)list->head,               \
            ._con_type = _ITER_LIST,                             \
            .next = (bool (*)(iter_##Type *))_iter_next,         \
        };                                                       \
                                                                 \
        return iter;                                             \
    }

#define iter_next(iter) iter.next(&iter)

#endif  // HINC_ITER_H
