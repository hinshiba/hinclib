#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "datastructure/base/hinc_list.h"

void print_int(int *n) {
    printf("%d ", *n);
    return;
}

void duble_int(int *n) {
    *n = *n * 2;
    return;
}

int sum_int(const int *a, const int *b) { return *a + *b; }

bool is_up15(const int *n) { return *n > 15; }

/* 新しいlistの定義 */
list_def(int);

int main(void) {
    /* 新しいlistの宣言と初期化 */
    list_int *list = list_int_from((int[]){1, 2, 3}, 3);
    printf("data_size: %zu\n", list->node_size);
    printf("len: %zu\n", list->len);
    printf("size: %zu\n", list->size);
    printf("\n");

    /* pushとpop */
    printf("pop: %d\n", list_int_pop_back(list));
    list_int_push_back(list, 100);

    printf("pop: %d\n", list_int_pop_back(list));
    printf("pop: %d\n", list_int_pop_back(list));
    printf("pop: %d\n", list_int_pop_back(list));
    printf("pop: %d\n", list_int_pop_back(list));

    printf("\n");

    list_free(list);
}