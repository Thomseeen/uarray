/*
 *      uarray.c
 *
 *      uarray - unique array data structure implementation
 *
 *      Copyright (C) 2021, Thomas Wagner (thomas@the-wagner.de)
 */

#include "uarray.h"

#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct uarray_entry_s {
    volatile sig_atomic_t used;
    void* item;
} uarray_entry_st;

struct uarray_s {
    pthread_rwlock_t lock;
    int max_len;
    uarray_entry_st* entries;
};

int uarray_create(uarray_st** ua, int max_len) {
    uarray_st* uarray;
    uarray = calloc(1, sizeof(uarray_st));
    if (!uarray) {
        return -1;
    }

    *ua = uarray;

    uarray->max_len = max_len;
    uarray->entries = calloc(max_len, sizeof(uarray_entry_st));

    for (int ii = 0; ii < max_len; ii++) {
        uarray->entries[ii].used = 0;
        uarray->entries[ii].item = NULL;
    }

    if ((errno = pthread_rwlock_init(&uarray->lock, NULL))) {
        return -1;
    }

    return 0;
}

int uarray_destroy(uarray_st* ua) {
    int res = 0;
    for (int ii = 0; ii < ua->max_len; ii++) {
        if (ua->entries[ii].used) {
            free(ua->entries[ii].item);
        }
        ua->entries[ii].used = 0;
        ua->entries[ii].item = NULL;
    }
    if ((errno = pthread_rwlock_destroy(&ua->lock))) {
        res = -1;
    }
    free(ua->entries);
    free(ua);
    return res;
}

void uarray_clear_all(uarray_st* ua) {
    pthread_rwlock_wrlock(&ua->lock);
    for (int ii = 0; ii < ua->max_len; ii++) {
        if (ua->entries[ii].used) {
            free(ua->entries[ii].item);
        }
        ua->entries[ii].used = 0;
    }
    pthread_rwlock_unlock(&ua->lock);
}

int uarray_add(uarray_st* ua, void* item) {
    int res = 0;
    pthread_rwlock_wrlock(&ua->lock);
    for (int ii = 0; ii < ua->max_len; ii++) {
        if (!ua->entries[ii].used) {
            ua->entries[ii].used = 1;
            ua->entries[ii].item = item;
            res = ii;
            break;
        } else if (ii == ua->max_len - 1) {
            /* List full */
            res = -1;
            break;
        }
    }
    pthread_rwlock_unlock(&ua->lock);
    return res;
}

int uarray_edit(uarray_st* ua, int index, void* item) {
    int res = 0;
    pthread_rwlock_wrlock(&ua->lock);
    if (ua->entries[index].used) {
        free(ua->entries[index].item);
        ua->entries[index].item = item;
        res = index;
    } else {
        /* Not found */
        res = -1;
    }
    pthread_rwlock_unlock(&ua->lock);
    return res;
}

int uarray_delete(uarray_st* ua, int index) {
    int res = 0;
    pthread_rwlock_wrlock(&ua->lock);
    if (ua->entries[index].used) {
        free(ua->entries[index].item);
        ua->entries[index].used = 0;
        res = 1;
    }
    pthread_rwlock_unlock(&ua->lock);
    return res;
}

void* uarray_read(uarray_st* ua, int index) {
    void* res;
    pthread_rwlock_rdlock(&ua->lock);
    if (ua->entries[index].used) {
        res = ua->entries[index].item;
    } else {
        /* Not found */
        res = NULL;
    }
    pthread_rwlock_unlock(&ua->lock);
    return res;
}

int uarray_get_used(uarray_st* ua, void* items) {
    int res = 0;
    pthread_rwlock_rdlock(&ua->lock);
    for (int ii = 0; ii < ua->max_len; ii++) {
        if (ua->entries[ii].used) {
            void* item = items + res;
            item = &ua->entries[ii].item;
            res++;
        }
    }
    pthread_rwlock_unlock(&ua->lock);
    return res;
}

int uarray_get_used_len(uarray_st* ua) {
    int res = 0;
    pthread_rwlock_rdlock(&ua->lock);
    for (int ii = 0; ii < ua->max_len; ii++) {
        if (ua->entries[ii].used)
            res++;
    }
    pthread_rwlock_unlock(&ua->lock);
    return res;
}

int uarray_get_max_len(uarray_st* ua) {
    return ua->max_len;
}

void uarray_get_used_idxstr(uarray_st* ua, char* buffer) {
    int idx = 0;
    pthread_rwlock_rdlock(&ua->lock);
    for (int ii = 0; ii < ua->max_len; ii++) {
        if (ua->entries[ii].used) {
            sprintf(buffer + idx, "%d,", ii);
            idx = strlen(buffer);
        }
    }
    pthread_rwlock_unlock(&ua->lock);
    /* Remove last comma */
    buffer[idx - 1] = '\0';
}
