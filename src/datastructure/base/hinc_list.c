#include "datastructure/base/hinc_list.h"

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LIST_BLOCK_SIZE 32

/*------------------------------------------------------------*/
/* MARK: 汎用
/*------------------------------------------------------------*/

bool _is_in_len(const _List *list, size_t idx) { return idx < list->len; }
void _must_in_len(const _List *list, size_t idx) {
    if (_is_in_len(list, idx)) return;
    fprintf(stderr, "out of renge\n");
    exit(EXIT_FAILURE);
}

/**
 * @fn
 * @brief _free_headから新しいnodeの場所を確保する
 *
 * @details 足りなければexpandする
 */
_Node *_get_free_node(_List *list) {
    if (list->len == list->size) {
        _list_resize(list, 1);
    }
    _Node *new = list->_free_head;
    list->_free_head = list->_free_head->next;

    new->next = NULL;
    new->prev = NULL;

    ++list->len;
    return new;
}

/**
 * @fn
 * @brief _free_headにnodeを返却する
 *
 * @details
 */
void _ret_free_node(_List *list, _Node *node) {
    node->next = list->_free_head;
    list->_free_head = node;

    --list->len;
    return;
}

/*------------------------------------------------------------*/
/* MARK: 作成と消去 (new, from, free)
/*------------------------------------------------------------*/

_List *_list_new(size_t node_size, size_t size) {
    _List *list = malloc(sizeof(_List));
    if (list == NULL) {
        perror("malloc miss in _list_new list\n");
        exit(EXIT_FAILURE);
    }
    list->tail = &list->head;
    list->node_size = node_size;
    list->len = 0;
    list->_free_head = NULL;
    list->_block_head = NULL;
    _list_resize(list, size);
    return list;
}

_List *_list_from(void *data, size_t node_size, size_t len) {
    _List *list = _list_new(node_size, len);
    /* 実質的にpushの繰り返し，データサイズの問題で外で書き込む */
    list->len = len;
    return list;
}

void list_free(void *list) {
    /* 最初に設定した nextがNULLのblockまでfree */
    for (_Node *block_node = ((_List *)list)->_block_head; block_node;
         block_node = block_node->next) {
        free(block_node);
    }

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

/**
 * @fn
 * @brief 確保した書き込み先を返す
 */
_Node *_list_push_back(_List *list) {
    _Node *node = _get_free_node(list);
    /* 接続操作 */
    node->prev = list->tail;
    list->tail->next = node;
    list->tail = node;
    return node;
}

/**
 * @fn
 * @brief tailのNodeを返す
 */
_Node *_list_pop_back(_List *list) {
    if (list->len == 0) {
        fprintf(stderr, "try pop but list is empty\n");
        exit(EXIT_FAILURE);
    }
    _Node *tail = list->tail;
    list->tail = tail->prev;
    list->tail->next = NULL;
    _ret_free_node(list, tail);
    return tail;
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
    _Node *pool = malloc((size + 1) * list->node_size);
    if (pool == NULL) {
        perror("malloc miss in _list_expand\n");
        exit(EXIT_FAILURE);
    }
    /* block listに追加 */
    pool->next = list->_block_head;
    list->_block_head = pool;

    /* free listの先頭に追加 */
    for (size_t i = 1; i < size + 1; ++i) {
        (pool + i)->next = list->_free_head;
        list->_free_head = (pool + i);
    }
    list->size += size;
    return;
}

/*------------------------------------------------------------*/
/* MARK: 基本演算関連 (copy, revese, concat)
/*------------------------------------------------------------*/

_List *_list_cpy(_List *list) {
    _List *new_list = _list_new(list->node_size, list->len);
    /* 実質的にpushの繰り返し */
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
