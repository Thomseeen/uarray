/*
 *      uarray.h
 *
 *      uarray - unique array data structure implementation
 *
 *      Copyright (C) 2021, Thomas Wagner (thomas@the-wagner.de)
 */

#ifndef UARRAY_H
#define UARRAY_H

typedef struct uarray_s uarray_st;

int uarray_init(uarray_st** ua, int max_len);
int uarray_deinit(uarray_st* ua);

int uarray_clear_all(uarray_st* ua);

int uarray_add(uarray_st* ua, void* item);
int uarray_edit(uarray_st* ua, int index, void* item);
int uarray_delete(uarray_st* ua, int index);
void* uarray_read(uarray_st* ua, int index);

int uarray_used_len(uarray_st* ua);
int uarray_max_len(uarray_st* ua);
void uarray_used_idxstr(uarray_st* ua, char* buffer);

#endif /* UARRAY_H */
