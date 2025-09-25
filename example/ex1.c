#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "datastructure/base/hinc_vec.h"

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

/* 新しいvecの定義 */
vec_def(int);

int main(void) {
    /* 新しいvecの宣言と初期化 */
    vec_int *vec = vec_int_from((int[]){1, 2, 3}, 3);
    printf("data_size: %zu\n", vec->data_size);
    printf("len: %zu\n", vec->len);
    printf("size: %zu\n", vec->size);
    printf("\n");

    /* 安全な vec[i] */
    printf("vec[0]: %d\n", vec_int_get(vec, 0));
    printf("vec[1]: %d\n", vec_int_get(vec, 1));
    printf("vec[2]: %d\n", vec_int_get(vec, 2));
    printf("\n");

    /* 実行時に out of range Error */
    // printf("vec[2]: %d\n", *vec_int_get(vec, 3));

    /* 変更できるポインタの取得 */
    *vec_int_get_mut(vec, 1) = 10;

    /* pushとpop */
    printf("pop: %d\n", vec_int_pop(vec));
    vec_int_push(vec, 100);
    printf("\n");

    /* foreachの利用 */
    vec_int_foreach(vec, duble_int);
    vec_int_foreach(vec, print_int);
    printf("\n");

    /* 条件を満たす要素だけの新しいvecを作成 */
    vec_int *new_vec = vec_int_filter(vec, is_up15);
    vec_int_foreach(new_vec, print_int);
    printf("\n");

    /* 累積的な計算 */
    printf("sum: %d\n", vec_int_reduce(new_vec, sum_int, 0));
    printf("\n");

    vec_free(vec);
    vec_free(new_vec);
}