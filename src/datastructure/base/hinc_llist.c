#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "datastructure/base/hinc_list.h"

#define VEC_BLOCK_SIZE 128

/*------------------------------------------------------------*/
/* MARK: 汎用
/*------------------------------------------------------------*/

bool _is_in_len(const _List *list, size_t idx) { return idx < list->len; }
void _must_in_len(const _List *list, size_t idx) {
    if (_is_in_len(list, idx)) return;
    fprintf(stderr, "out of renge\n");
    exit(EXIT_FAILURE);
}

/*------------------------------------------------------------*/
/* MARK: 作成と消去 (new, from, free)
/*------------------------------------------------------------*/

_List *_list_new(size_t data_size, size_t size) {
    _List *list = malloc(sizeof(_List));
    if (list == NULL) {
        perror("malloc miss in _list_new list\n");
        exit(EXIT_FAILURE);
    }
    list->data = NULL;
    list->data_size = data_size;
    list->len = 0;
    _list_resize(list, size);
    return list;
}

_List *_list_from(void *data, size_t data_size, size_t len) {
    _List *list = _list_new(data_size, len);
    memcpy(list->data, data, data_size * len);
    list->len = len;
    return list;
}

void list_free(void *list) {
    free(((_List *)list)->data);
    free((_List *)list);
    return;
}

/*------------------------------------------------------------*/
/* MARK: getとset
/*------------------------------------------------------------*/

size_t _list_get(const _List *list, size_t idx) {
    _must_in_len(list, idx);
    return idx;
}

size_t _list_get_mut(_List *list, size_t idx) {
    _must_in_len(list, idx);
    return idx;
}

/*------------------------------------------------------------*/
/* MARK: pushとpop
/*------------------------------------------------------------*/

size_t _list_push(_List *list) {
    if (list->len == list->size) {
        _list_resize(list, list->len + 1);
    }
    return list->len++;
}

size_t _list_pop(_List *list) {
    if (list->len == 0) {
        fprintf(stderr, "try pop but list is empty\n");
        exit(EXIT_FAILURE);
    }
    return --list->len;
}

/*------------------------------------------------------------*/
/* MARK: resize関連 (expand, resize, shrink)
/*------------------------------------------------------------*/

/**
 * @fn
 * @param len: 追加するサイズ
 */
void _list_expand(_List *list, size_t len) {
    /* lenが入るもっとも小さなLIST_BLOCK_SIZEの倍数を探す */
    size_t size = (len & ~(LIST_BLOCK_SIZE - 1)) + LIST_BLOCK_SIZE;
    _Node *pool = malloc(size * sizeof(_Node));
    if (pool == NULL) {
        perror("malloc miss in _list_expand\n");
        exit(EXIT_FAILURE);
    }
    /* free listの先頭に追加 */
    for (size_t i = 0; i < size; ++i) {
        (pool + i)->next = list->free_head;
        list->free_head = (pool + i);
    }
    list->size += size;
    return;
}

/*------------------------------------------------------------*/
/* MARK: 基本演算関連 (copy, revese, concat)
/*------------------------------------------------------------*/

_List *_list_cpy(_List *list) {
    _List *new_list = _list_new(list->data_size, list->len);
    memcpy(new_list->data, list->data, list->data_size * list->len);
    return new_list;
}

/*------------------------------------------------------------*/
/* MARK: アルゴリズム関連 (bseach, sort, index_of)
/*------------------------------------------------------------*/

/*------------------------------------------------------------*/
/* MARK: 高階関数関連 (foreach, map, filter, reduce, all, any)
/*------------------------------------------------------------*/

/*------------------------------------------------------------*/
/* MARK: TODO sliice関連 (slice)
/*------------------------------------------------------------*/

/*------------------------------------------------------------*/
/* MARK: TODO insertとremove
/*------------------------------------------------------------*/
