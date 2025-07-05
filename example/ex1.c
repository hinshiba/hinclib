#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "datastructure/base/hinc_vec.h"

vec_def(int);

int main(void) {
    vec_int *vec = vec_int_from((int[]){1, 2, 3}, 3);
    printf("%zu\n", vec->data_size);
    printf("%zu\n", vec->len);
    printf("%zu\n", vec->size);

    printf("%d\n", *vec_int_get(vec, 0));
    printf("%d\n", *vec_int_get(vec, 1));
    printf("%d\n", *vec_int_get(vec, 2));
    printf("%d\n", *vec_int_get(vec, 3));  // out of range
}