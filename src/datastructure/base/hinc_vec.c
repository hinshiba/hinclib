#include "datastructure/base/hinc_vec.h"

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*------------------------------------------------------------*/
/* MARK: 汎用
/*------------------------------------------------------------*/

bool _is_in_len(const _Vec *vec, size_t idx) { return idx < vec->len; }
void _must_in_len(const _Vec *vec, size_t idx) {
    if (_is_in_len(vec, idx)) return;
    fprintf(stderr, "out of renge\n");
    exit(EXIT_FAILURE);
}

/*------------------------------------------------------------*/
/* MARK: 作成と消去 (new, from, free)
/*------------------------------------------------------------*/

_Vec *_vec_new(size_t data_size, size_t len) {
    _Vec *vec = malloc(sizeof(_Vec));
    if (vec == NULL) {
        perror("malloc miss in _vec_new\n");
        exit(EXIT_FAILURE);
    }
    size_t size = data_size * len;
    vec->data = malloc(size);
    vec->data_size = data_size;
    vec->len = len;
    vec->size = size;
    return vec;
}

_Vec *_vec_from(void *data, size_t data_size, size_t len) {
    _Vec *vec = _vec_new(data_size, len);
    memcpy(vec->data, data, data_size * len);
    return vec;
}

void _vec_free(_Vec *vec) {
    fprintf(stderr, "todo func\n");
    return;
}

/*------------------------------------------------------------*/
/* MARK: getとset
/*------------------------------------------------------------*/

size_t _vec_get(const _Vec *vec, size_t idx) {
    _must_in_len(vec, idx);
    return idx;
}
// size_t _vec_get_mut(_Vec *vec, size_t idx) { return idx; }

/*------------------------------------------------------------*/
/* MARK: pushとpop
/*------------------------------------------------------------*/

/*------------------------------------------------------------*/
/* MARK: resize関連 (resize, shrink)
/*------------------------------------------------------------*/

/*------------------------------------------------------------*/
/* MARK: sliice関連 (slice)
/*------------------------------------------------------------*/

/*------------------------------------------------------------*/
/* MARK: 基本演算関連 (copy, concat, )
/*------------------------------------------------------------*/

/*------------------------------------------------------------*/
/* MARK: TODO insertとremove
/*------------------------------------------------------------*/

/*------------------------------------------------------------*/
/* MARK: TODO revese
/*------------------------------------------------------------*/

/*------------------------------------------------------------*/
/* MARK: 高階関数関連 ()
/*------------------------------------------------------------*/

/*------------------------------------------------------------*/
/* MARK: マクロ
/*------------------------------------------------------------*/
