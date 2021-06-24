#include "uarray.h"

#include <stdio.h>
#include <stdlib.h>

static int test();

int main() {
    if (test()) {
        return 1;
    }
    printf("test successful\n");
    return 0;
}

static int test() {
    uarray_st* uarray = NULL;
    int uarray_cap = 2;

    char buffer[uarray_cap * (2 + 1)];

    if (uarray_create(&uarray, uarray_cap) != 0) {
        printf("failed to init\n");
        return -1;
    }

    if (uarray_add(uarray, "item1") != 0) {
        printf("failed to add item 1\n");
        return -1;
    }

    if (uarray_add(uarray, "item2") != 0) {
        printf("failed to add item 2\n");
        return -1;
    }

    if (uarray_add(uarray, "item3") != -1) {
        printf("added item although full\n");
        return -1;
    }

    if (uarray_get_used_len(uarray) != 2) {
        printf("len is erroneous at 2 items\n");
        return -1;
    }

    uarray_get_used_idxstr(uarray, buffer);
    printf("current idexes: %s\n", buffer);

    if (uarray_delete(uarray, 1) != 1) {
        printf("failed to delete item\n");
        return -1;
    }

    if (uarray_delete(uarray, 1) != 0) {
        printf("deleted item although none there\n");
        return -1;
    }

    if (uarray_read(uarray, 0) != "item1") {
        printf("read wrong item, expected item1\n");
        return -1;
    }

    if (uarray_edit(uarray, 0, "item0") != 0) {
        printf("failed to edit item\n");
        return -1;
    }

    if (uarray_read(uarray, 0) != "item0") {
        printf("read wrong item, expected item0\n");
        return -1;
    }

    if (uarray_get_used_len(uarray) != 1) {
        printf("len is erroneous at 1 item\n");
        return -1;
    }

    uarray_get_used_idxstr(uarray, buffer);
    printf("current idexes: %s\n", buffer);

    if (uarray_destroy(uarray) != 0) {
        printf("failed to destroy\n");
        return -1;
    }
    return 0;
}
