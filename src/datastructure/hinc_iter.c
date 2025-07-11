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

bool _iter_has_next(_Iter *iter) {
    switch (iter->_con_type) {
        case _ITER_NO:
            fprintf(stderr, "this iter is not valid _ITER_NO\n");
            return false;
        case _ITER_VEC:
            return (char *)iter->_con.vec->data +
                       (iter->_con.vec->len * iter->_con.vec->data_size) >
                   (char *)iter->ref;
        case _ITER_LIST:
            return iter->_con.list_node && iter->_con.list_node->next;
        default:
            fprintf(stderr, "this iter is not valid\n");
            return false;
    }
}

void _iter_next(_Iter *iter) {
    if (iter->has_next = _iter_has_next(iter)) {
        switch (iter->_con_type) {
            case _ITER_NO:
                fprintf(stderr, "this iter is not valid _ITER_NO\n");
                break;
            case _ITER_VEC:
                iter->ref = (char *)iter->ref + iter->_con.vec->data_size;
                break;
            case _ITER_LIST:
                iter->_con.list_node = iter->_con.list_node->next;
                iter->ref = &iter->_con.list_node->data;
                break;
            default:
                fprintf(stderr, "this iter is not valid\n");
                break;
        }
    }
    return;
}

void iter_free(void *iter) {
    free(iter);
    return;
}