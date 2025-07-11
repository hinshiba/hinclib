#include "datastructure/hinc_iter.h"

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "datastructure/base/hinc_list.h"
#include "datastructure/base/hinc_vec.h"

bool _iter_next(_Iter *iter) {
    switch (iter->_con_type) {
        case _ITER_NO:
            fprintf(stderr, "this iter is not valid _ITER_NO\n");
            return false;
        case _ITER_VEC:
            if ((char *)iter->_con.vec->data +
                    (iter->_con.vec->len * iter->_con.vec->data_size) >
                (char *)iter->ref) {
                iter->ref = (char *)iter->ref + iter->_con.vec->data_size;
                return true;
            }
            return false;
        case _ITER_LIST:
            if (iter->_con.list_node->next) {
                iter->_con.list_node = iter->_con.list_node->next;
                iter->ref = &iter->_con.list_node->data;
                return true;
            }
            return false;
        default:
            fprintf(stderr, "this iter is not valid\n");
            return false;
    }
}

_Iter *_iter_new(void) {
    _Iter *iter = malloc(sizeof(_Iter));
    if (iter == NULL) {
        perror("malloc miss in _iter_new\n");
        exit(EXIT_FAILURE);
    }
    return iter;
}

void iter_free(void *iter) {
    free(iter);
    return;
}