#ifndef HINC_ITER_H
#define HINC_ITER_H

#include <stdbool.h>
#include <stddef.h>

typedef struct _Iter _Iter;

struct _Iter {
    bool (*next)(_Iter *);
    bool (*prev)(_Iter *);
    void *ref;
};

#define iter_def(Type)                      \
    typedef struct iter_##Type iter_##Type; \
                                            \
    struct iter_##Type {                    \
        bool (*next)(_Iter *);              \
        bool (*prev)(_Iter *);              \
        Type *data;                         \
    };

// #define iter_def_for_vec(Type)

#define iter_def_for_list(Type) iter_##Type *iter_##Type##_new_from_list()

#define iter_next(iter) iter.next((_Iter *)&iter)

#define iter_prev(iter) iter.prev((_Iter *)&iter)

#endif  // HINC_ITER_H
