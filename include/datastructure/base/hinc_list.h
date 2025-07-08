#ifndef HINC_LIST_H
#define HINC_LIST_H

#include <stdbool.h>
#include <stddef.h>

typedef struct _Node _Node;

struct _Node {
    _Node *next;
    _Node *prev;
    char data[];
};

typedef struct {
    _Node *tail;
    size_t data_size;
    size_t len;          /// 要素数
    size_t size;         /// 割り当て済み要素数
    _Node *_free_head;   /// 割り当て済み
    _Node *_block_head;  /// 解放すべきブロックの先頭
    _Node head;
} _List;

_List *_list_new(size_t data_size, size_t len);
_List *_list_from(void *data, size_t data_size, size_t len);
void list_free(void *list);

size_t _list_get(const _List *list, size_t idx);
size_t _list_get_mut(_List *list, size_t idx);

// size_t _list_push_front(_List *list);
// size_t _list_pop_front(_List *list);

_Node *_list_push_back(_List *list);
_Node *_list_pop_back(_List *list);

// insert
// erase
// swap

void _list_expand(_List *list, size_t len);
// void _list_resize(_List *list, size_t len);
// void list_shrink(_List *list);

// _List *_list_cpy(_List *list);
// void list_reverse(_List *list);
// void list_concat(_List *list1, _List *list2);

// void list_sort(Vec *list, int (*cmp)(const void *, const void *));
// int list_index_of(Vec *list, void *data, int (*cmp)(const void *, const void
// *));

// list_map
// list_reduce
// list_all
// list_any

// Vec *list_slice(Vec *list, size_t start, size_t end);

// int list_insert(Vec *list, size_t index, void *data);
// void *list_remove(Vec *list, size_t index);

#define list_def(Type)                                                 \
    typedef struct _node_internal_##Type _node_internal_##Type;        \
    struct _node_internal_##Type {                                     \
        struct node_##Type *next;                                      \
        struct node_##Type *prev;                                      \
    };                                                                 \
                                                                       \
    typedef struct node_##Type node_##Type;                            \
    struct node_##Type {                                               \
        struct node_##Type *next;                                      \
        struct node_##Type *prev;                                      \
        Type data;                                                     \
    };                                                                 \
                                                                       \
    typedef struct {                                                   \
        node_##Type *tail;                                             \
        size_t data_size;                                              \
        size_t len;                                                    \
        size_t size;                                                   \
        _node_internal_##Type *_free_head;                             \
        _node_internal_##Type *_block_head;                            \
        _node_internal_##Type head;                                    \
    } list_##Type;                                                     \
                                                                       \
    list_##Type *list_##Type##_new(size_t len) {                       \
        return (list_##Type *)_list_new(sizeof(Type), len);            \
    }                                                                  \
                                                                       \
    list_##Type *list_##Type##_from(Type data[], size_t len) {         \
        list_##Type *list = list_##Type##_new(len);                    \
        for (size_t i = 0; i < len; ++i) {                             \
            *(Type *)(_list_push_back((_List *)list)->data) = data[i]; \
        }                                                              \
                                                                       \
        return list;                                                   \
    }                                                                  \
                                                                       \
    void list_##Type##_push_back(list_##Type *list, const Type elem) { \
        *(Type *)(_list_push_back((_List *)list)->data) = elem;        \
        return;                                                        \
    }                                                                  \
                                                                       \
    Type list_##Type##_pop_back(list_##Type *list) {                   \
        return *(Type *)(_list_pop_back((_List *)list)->data);         \
    }

#endif  // HINC_LIST_H
