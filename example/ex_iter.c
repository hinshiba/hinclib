#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "datastructure/base/hinc_list.h"
#include "datastructure/hinc_iter.h"

int compint(const int *l, const int *r) {
    printf("%d-%d\n", *l, *r);
    return *l - *r;
}

/* 新しいlistの定義 */
list_def(int);
iter_def(int);
iter_def_for_list(int);

int main(void) {
    /* 新しいlistの宣言と初期化 */
    list_int *list =
        list_int_from((int[]){54, 82, 37, 14, 78, 87, 32, 71, 91, 90, 12,
                              61, 76, 69, 58, 87, 65, 64, 40, 45, 35},
                      21);
    printf("data_size: %zu\n", list->data_size);
    printf("len: %zu\n", list->len);
    printf("size: %zu\n", list->size);
    printf("\n");

    /* pushとpop */
    printf("pop: %d\n", list_int_pop_back(list));
    list_int_push_back(list, 100);

    list_int_sort(list, compint);

    printf("pop: %d\n", *list_int_get(list, 0));
    printf("pop: %d\n", list_int_pop_back(list));
    printf("pop: %d\n", list_int_pop_back(list));
    printf("pop: %d\n", list_int_pop_back(list));
    node_int *node = list->head;
    node_int_next(&node);
    printf("next: %d", node->data);
    printf("\n");
    for (iter_int iter = iter_int_new_from_list(list);; iter_next(iter)) {
        printf("iter: %d\n", *iter.ref);
    }

    list_free(list);
}