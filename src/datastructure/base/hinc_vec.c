#include "datastructure/base/hinc_vec.h"

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VEC_BLOCK_SIZE 128

/*------------------------------------------------------------
 * MARK: 汎用
 *------------------------------------------------------------*/

bool _is_in_len(const _Vec *vec, size_t idx) { return idx < vec->len; }
void _must_in_len(const _Vec *vec, size_t idx) {
    if (_is_in_len(vec, idx)) return;
    fprintf(stderr, "out of renge\n");
    exit(EXIT_FAILURE);
}

/*------------------------------------------------------------
 * MARK: 作成と消去 (new, from, free)
 *------------------------------------------------------------*/

_Vec *_vec_new(size_t data_size, size_t size) {
    _Vec *vec = malloc(sizeof(_Vec));
    if (vec == NULL) {
        perror("malloc miss in _vec_new vec\n");
        exit(EXIT_FAILURE);
    }
    vec->data = NULL;
    vec->data_size = data_size;
    vec->len = 0;
    _vec_resize(vec, size);
    return vec;
}

_Vec *_vec_from(void *data, size_t data_size, size_t len) {
    _Vec *vec = _vec_new(data_size, len);
    memcpy(vec->data, data, data_size * len);
    vec->len = len;
    return vec;
}

void vec_free(void *vec) {
    free(((_Vec *)vec)->data);
    free((_Vec *)vec);
    return;
}

/*------------------------------------------------------------
 * MARK: getとset
 *------------------------------------------------------------*/

size_t _vec_get(const _Vec *vec, size_t idx) {
    _must_in_len(vec, idx);
    return idx;
}

size_t _vec_get_mut(_Vec *vec, size_t idx) {
    _must_in_len(vec, idx);
    return idx;
}

/*------------------------------------------------------------
 * MARK: pushとpop
 *------------------------------------------------------------*/

size_t _vec_push(_Vec *vec) {
    if (vec->len == vec->size) {
        _vec_resize(vec, vec->len + 1);
    }
    return vec->len++;
}

size_t _vec_pop(_Vec *vec) {
    if (vec->len == 0) {
        fprintf(stderr, "try pop but vec is empty\n");
        exit(EXIT_FAILURE);
    }
    return --vec->len;
}

/*------------------------------------------------------------
 * MARK: resize関連 (resize, shrink)
 *------------------------------------------------------------*/

void _vec_resize(_Vec *vec, size_t len) {
    /* lenが入るもっとも小さなVEC_BLOCK_SIZEの倍数を探す */
    size_t size = (len & ~(VEC_BLOCK_SIZE - 1)) + VEC_BLOCK_SIZE;
    vec->data = realloc(vec->data, size * vec->data_size);
    if (vec->data == NULL) {
        perror("realloc miss in _vec_resize\n");
        exit(EXIT_FAILURE);
    }
    vec->size = size;
    return;
}

/*------------------------------------------------------------
 * MARK: 基本演算関連 (copy, revese, concat)
 *------------------------------------------------------------*/

_Vec *_vec_cpy(_Vec *vec) {
    _Vec *new_vec = _vec_new(vec->data_size, vec->len);
    memcpy(new_vec->data, vec->data, vec->data_size * vec->len);
    return new_vec;
}

/*------------------------------------------------------------
 * MARK: アルゴリズム関連 (bseach, sort, index_of)
 *------------------------------------------------------------*/

/*------------------------------------------------------------
 * MARK: 高階関数関連 (foreach, map, filter, reduce, all, any)
 *------------------------------------------------------------*/

/*------------------------------------------------------------
 * MARK: TODO sliice関連 (slice)
 *------------------------------------------------------------*/

/*------------------------------------------------------------
 * MARK: TODO insertとremove
 *------------------------------------------------------------*/
