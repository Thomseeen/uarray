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

typedef struct {
    volatile sig_atomic_t used;
    void* item;
} uarray_entry_st;

struct uarray_s {
    pthread_rwlock_t lock;
    int maxlen;
    uarray_entry_st* data;
};

int uarray_init(uarray_st** ua, int max_len) {
    uarray_st* uarray;
    uarray = calloc(1, sizeof(uarray_st));
    if (!uarray) {
        return -1;
    }

    *ua = uarray;

    uarray->maxlen = max_len;
    uarray->data = calloc(max_len, sizeof(uarray_entry_st));

    for (int ii = 0; ii < max_len; ii++) {
        uarray->data[ii].used = 0;
        uarray->data[ii].item = NULL;
    }

    if ((errno = pthread_rwlock_init(&uarray->lock, NULL))) {
        return -1;
    }
    return 0;
}

int uarray_deinit(uarray_st* ua) {
    int res = 0;
    for (int ii = 0; ii < ua->maxlen; ii++) {
        ua->data[ii].used = 0;
        ua->data[ii].item = NULL;
    }
    if ((errno = pthread_rwlock_destroy(&ua->lock))) {
        res = -1;
    }
    free(ua->data);
    free(ua);
    return res;
}

int uarray_clear_all(uarray_st* ua) {
    pthread_rwlock_wrlock(&ua->lock);
    for (int ii = 0; ii < ua->maxlen; ii++) {
        ua->data[ii].used = 0;
    }
    pthread_rwlock_unlock(&ua->lock);
}

int uarray_add(uarray_st* ua, void* item) {
    int res = 0;
    pthread_rwlock_wrlock(&ua->lock);
    for (int ii = 0; ii < ua->maxlen; ii++) {
        if (!ua->data[ii].used) {
            ua->data[ii].used = 1;
            ua->data[ii].item = item;
            break;
        } else if (ii == ua->maxlen - 1) {
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
    if (ua->data[index].used) {
        ua->data[index].item = item;
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
    if (ua->data[index].used) {
        ua->data[index].used = 0;
        res = 1;
    }
    pthread_rwlock_unlock(&ua->lock);
    return res;
}

void* uarray_read(uarray_st* ua, int index) {
    void* res;
    pthread_rwlock_rdlock(&ua->lock);
    if (ua->data[index].used) {
        res = ua->data[index].item;
    } else {
        /* Not found */
        res = NULL;
    }
    pthread_rwlock_unlock(&ua->lock);
    return res;
}

int uarray_used_len(uarray_st* ua) {
    int res = 0;
    pthread_rwlock_rdlock(&ua->lock);
    for (int ii = 0; ii < ua->maxlen; ii++) {
        if (ua->data[ii].used)
            res++;
    }
    pthread_rwlock_unlock(&ua->lock);
    return res;
}

int uarray_max_len(uarray_st* ua) {
    return ua->maxlen;
}

void uarray_used_idxstr(uarray_st* ua, char* buffer) {
    int idx = 0;
    pthread_rwlock_rdlock(&ua->lock);
    for (int ii = 0; ii < ua->maxlen; ii++) {
        if (ua->data[ii].used) {
            sprintf(buffer + idx, "%d,", ii);
            idx = strlen(buffer);
        }
    }
    pthread_rwlock_unlock(&ua->lock);
    /* Remove last comma */
    buffer[idx - 1] = '\0';
}
