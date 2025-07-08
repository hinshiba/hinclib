#ifndef HINC_LIST_H
#define HINC_LIST_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
    void *data;
    struct _Node *next;
    struct Node *prev;
} _Node;

typedef struct {
    size_t data_size;
    size_t len;   /// 要素数
    size_t size;  /// 割り当て済み要素数
} _List;

_List *_list_new(size_t data_size, size_t len);
_List *_list_from(void *data, size_t data_size, size_t len);
void list_free(void *list);

size_t _list_get(const _List *list, size_t idx);
size_t _list_get_mut(_List *list, size_t idx);

size_t _list_push(_List *list);
size_t _list_pop(_List *list);

// insert
// delete
// swap

void _list_resize(_List *list, size_t len);
// void list_shrink(_List *list);

// _List *_list_cpy(_List *list);
// void list_reverse(_List *list);
// void list_concat(_List *list1, _List *list2);

// void list_sort(Vec *list, int (*cmp)(const void *, const void *));
// int list_bsearch(Vec *list, const void *key,
//                 int (*cmp)(const void *, const void *));
// int list_index_of(Vec *list, void *data, int (*cmp)(const void *, const void
// *));

// list_map
// list_reduce
// list_all
// list_any

// Vec *list_slice(Vec *list, size_t start, size_t end);

// int list_insert(Vec *list, size_t index, void *data);
// void *list_remove(Vec *list, size_t index);

#define list_def(Type)                                                    \
    typedef struct {                                                      \
        Type *data;                                                       \
        size_t data_size;                                                 \
        size_t len;                                                       \
        size_t size;                                                      \
    } list_##Type;                                                        \
                                                                          \
    list_##Type *list_##Type##_new(size_t len) {                          \
        return (list_##Type *)_list_new(sizeof(Type), len);               \
    }                                                                     \
                                                                          \
    list_##Type *list_##Type##_from(Type data[], size_t len) {            \
        return (list_##Type *)_list_from(data, sizeof(Type), len);        \
    }                                                                     \
                                                                          \
    const Type *list_##Type##_get(const list_##Type *list, size_t idx) {  \
        return &list->data[_list_get((_List *)list, idx)];                \
    }                                                                     \
                                                                          \
    Type *list_##Type##_get_mut(list_##Type *list, size_t idx) {          \
        return &list->data[_list_get_mut((_List *)list, idx)];            \
    }                                                                     \
                                                                          \
    void list_##Type##_push(list_##Type *list, const Type elem) {         \
        list->data[_list_push((_List *)list)] = elem;                     \
        return;                                                           \
    }                                                                     \
                                                                          \
    Type list_##Type##_pop(list_##Type *list) {                           \
        return list->data[_list_pop((_List *)list)];                      \
    }                                                                     \
                                                                          \
    void list_##Type##_resize(list_##Type *list, size_t len) {            \
        return _list_resize((_List *)list, len);                          \
    }                                                                     \
                                                                          \
    list_##Type *list_##Type##_cpy(list_##Type *list) {                   \
        return (list_##Type *)_list_cpy((_List *)list);                   \
    }                                                                     \
                                                                          \
    void list_##Type##_foreach(list_##Type *list, void (*func)(Type *)) { \
        for (size_t i = 0; i < list->len; ++i) {                          \
            func(list_##Type##_get_mut(list, i));                         \
        }                                                                 \
    }                                                                     \
                                                                          \
    list_##Type *list_##Type##_filter(list_##Type *list,                  \
                                      bool (*func)(const Type *)) {       \
        list_##Type *new_list = list_##Type##_new(list->len);             \
        for (size_t i = 0; i < list->len; ++i) {                          \
            const Type *elem = list_##Type##_get(list, i);                \
            if (func(elem)) list_##Type##_push(new_list, *elem);          \
        }                                                                 \
        return new_list;                                                  \
    }                                                                     \
    Type list_##Type##_reduce(list_##Type *list,                          \
                              Type (*func)(const Type *, const Type *),   \
                              Type first) {                               \
        Type res = first;                                                 \
        for (size_t i = 0; i < list->len; ++i) {                          \
            const Type *elem = list_##Type##_get(list, i);                \
            res = func(&res, elem);                                       \
        }                                                                 \
        return res;                                                       \
    }

#endif  // HINC_LIST_H
