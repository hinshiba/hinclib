#include "datastructure/base/hinc_list.h"

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LIST_BLOCK_SIZE 32

/*------------------------------------------------------------
 * MARK: 汎用
 *------------------------------------------------------------*/

bool _is_in_list_len(const _List *list, size_t idx) { return idx < list->len; }
void _must_in_list_len(const _List *list, size_t idx) {
    if (_is_in_list_len(list, idx)) return;
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
        _list_expand(list, 1);
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

/*------------------------------------------------------------
 * MARK: 作成と消去 (new, from, free)
 *------------------------------------------------------------*/

_List *_list_new(size_t data_size, size_t size) {
    _List *list = malloc(sizeof(_List));
    if (list == NULL) {
        perror("malloc miss in _list_new list\n");
        exit(EXIT_FAILURE);
    }
    list->head = NULL;
    list->tail = NULL;
    list->data_size = data_size;
    list->len = 0;
    list->size = 0;
    list->_free_head = NULL;
    list->_block_head = NULL;
    _list_expand(list, size);
    return list;
}

_List *_list_from(void *data, size_t data_size, size_t len) {
    _List *list = _list_new(data_size, len);
    /* 実質的にpushの繰り返し，データサイズの問題で外で書き込む */
    list->len = len;
    return list;
}

void list_free(void *list) {
    /* 最初に設定した nextがNULLのblockまでfree */
    _Node *tmp;
    for (_Node *block_node = ((_List *)list)->_block_head; block_node;) {
        tmp = block_node->next;
        free(block_node);
        block_node = tmp;
    }

    free((_List *)list);
    return;
}

/*------------------------------------------------------------
 * MARK: getとset
 *------------------------------------------------------------*/

_Node *_list_get(const _List *list, size_t idx) {
    _Node *node;
    _must_in_list_len(list, idx);
    if (idx * 2 < list->len) {
        /* 前からたどったほうが早い */
        node = list->head;
        for (size_t i = 0; i < idx; ++i) {
            node = node->next;
        }
    } else {
        node = list->tail;
        for (size_t i = list->len - 1; i != idx; --i) {
            node = node->prev;
        }
    }
    return node;
}

size_t _list_get_mut(_List *list, size_t idx) {
    _must_in_list_len(list, idx);
    return idx;
}

/*------------------------------------------------------------
 * MARK: pushとpop
 *------------------------------------------------------------*/

/**
 * @fn
 * @brief 確保した書き込み先を返す
 */
_Node *_list_push_back(_List *list) {
    _Node *node = _get_free_node(list);
    /* 接続操作 */
    node->prev = list->tail;
    if (list->head) {
        list->tail->next = node;
    } else {
        list->head = node;
    }
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
    if (list->tail) {
        list->tail->next = NULL;
    } else {
        list->head = NULL;
    }
    _ret_free_node(list, tail);
    return tail;
}

/*------------------------------------------------------------
 * MARK: resize関連 (expand, resize, shrink)
 *------------------------------------------------------------*/

/**
 * @fn
 * @param len: 追加するサイズ
 */
void _list_expand(_List *list, size_t len) {
    /* lenが入るもっとも小さなLIST_BLOCK_SIZEの倍数を探す */
    size_t size = (len & ~(LIST_BLOCK_SIZE - 1)) + LIST_BLOCK_SIZE;
    _Node *pool =
        malloc((list->data_size + sizeof(_Node)) * size + sizeof(_Node));
    if (pool == NULL) {
        perror("malloc miss in _list_expand\n");
        exit(EXIT_FAILURE);
    }
    /* block listに追加 */
    pool->next = list->_block_head;
    list->_block_head = pool++;

    /* free listの先頭に追加 */
    /* 一回目は先頭NULLかもしれないので確認 */
    pool->next = list->_free_head;
    if (list->_free_head) {
        list->_free_head->prev = pool;
    }
    list->_free_head = pool++;
    /* dataのポインタを飛ばす */
    pool = (_Node *)((char *)pool + list->data_size);

    for (size_t i = 1; i < size; ++i) {
        pool->next = list->_free_head;
        list->_free_head->prev = pool;
        list->_free_head = pool++;
        /* dataの領域を飛ばす */
        pool = (_Node *)((char *)pool + list->data_size);
    }
    list->size += size;
    return;
}

/*------------------------------------------------------------
 * MARK: 基本演算関連 (copy, revese, concat)
 *------------------------------------------------------------*/

_List *_list_cpy(_List *list) {
    _List *new_list = _list_new(list->data_size, list->len);
    /* 実質的にpushの繰り返し */
    return new_list;
}

/*------------------------------------------------------------
 * MARK: アルゴリズム関連 (bseach, sort, index_of)
 *------------------------------------------------------------*/

_Node *_list_marge(_Node *left, _Node *right,
                   int (*compar)(const void *, const void *)) {
    if (!left) return right;
    if (!right) return left;

    _Node *head;
    _Node *tail;

    /* 先頭を決定 */
    if (compar(left->data, right->data) <= 0) {
        head = tail = left;
        left = left->next;
    } else {
        head = tail = right;
        right = right->next;
    }
    head->prev = NULL;

    /* 残りのノードをマージ */
    while (left && right) {
        if (compar(left->data, right->data) <= 0) {
            tail->next = left;
            left->prev = tail;
            tail = left;
            left = left->next;
        } else {
            tail->next = right;
            right->prev = tail;
            tail = right;
            right = right->next;
        }
    }

    /* 残っている場合の処理 */
    if (left) {
        tail->next = left;
        left->prev = tail;
    } else if (right) {
        tail->next = right;
        right->prev = tail;
    }

    return head;
}

void _list_sort(_List *list, int (*compar)(const void *, const void *)) {
    _Node *head = list->head;
    if (list->len < 1) {
        return; /* ソート不要 */
    }

    _Node *left;
    _Node *right;
    _Node *tail;
    _Node *current;
    size_t size = 1;

    while (1) {
        current = head;
        head = NULL;
        tail = NULL;

        size_t processed = 0;

        while (current) {
            ++processed;
            left = current;

            /* rightを探す */
            right = current;
            for (int i = 0; i < size && right; i++) {
                right = right->next;
            }
            if (right) {
                /* rightが有りそうなら分割 */
                _Node *prev_right = right->prev;
                prev_right->next = NULL;

                /* 次のループの開始点を保存 */
                current = right;
                for (int i = 0; i < size && current; i++) {
                    current = current->next;
                }

                /* rightもNULL終端に */
                if (current) {
                    _Node *prev_current = current->prev;
                    prev_current->next = NULL;
                }
            } else {
                current = NULL;
            }

            _Node *merged = _list_marge(left, right, compar);

            /* for dbg
            for (_Node *tmp = merged; tmp; tmp = tmp->next) {
                printf("%d, ", *(int *)tmp->data);
            }
            printf("\n");
            */

            /* 全体のリストに連結 */
            if (!head) {
                head = merged;
                tail = merged;
            } else {
                tail->next = merged;
                merged->prev = tail;
            }

            /* tailをマージ後リストの末尾に移動 */
            while (tail && tail->next) {
                tail = tail->next;
            }
        }

        /* マージが1回しか行われなかった場合、ソート完了 */
        if (processed <= 1) {
            break;
        }
        size *= 2;
    }

    list->head = head;
    list->tail = tail;
}

/*------------------------------------------------------------
 * MARK: 高階関数関連 (foreach, map, filter, reduce, all, any)
 *------------------------------------------------------------*/

/*------------------------------------------------------------
 * MARK: TODO sliice関連 (slice)
 *------------------------------------------------------------*/

/*------------------------------------------------------------
 * MARK: TODO insertとremove
 *------------------------------------------------------------*/
