/*
 *      uarray.h
 *
 *      uarray - unique array data structure implementation
 *
 *      Copyright (C) 2021, Thomas Wagner (thomas@the-wagner.de)
 */

#ifndef UARRAY_H
#define UARRAY_H

typedef struct uarray_t uarray_t;

int uarray_init(uarray_t** ua, int max_len);
int uarray_deinit(uarray_t* ua);

int uarray_clear_all(uarray_t* ua);

int uarray_add(uarray_t* ua, void* item);
int uarray_edit(uarray_t* ua, int index, void* item);
int uarray_delete(uarray_t* ua, int index);
void* uarray_read(uarray_t* ua, int index);

int uarray_used_len(uarray_t* ua);
int uarray_max_len(uarray_t* ua);
void uarray_used_idxstr(uarray_t* ua, char* buffer);

#endif /* UARRAY_H */
