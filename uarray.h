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

/*
 * Creates a new uarray.
 *
 * Returns 0 on succes and -1 on failure.
 */
int uarray_create(uarray_st** ua, int max_len);

/*
 * Destroys a uarray or frees it's resources.
 *
 * Any stored data pointers should be retrieved beforehand
 * and be freed as needed.
 *
 * Returns 0 on succes or -1 on failure.
 */
int uarray_destroy(uarray_st* ua);

/*
 * Markes all entries as unused.
 *
 * Any stored data pointers should be retrieved beforehand
 * and be freed as needed.
 */
void uarray_clear_all(uarray_st* ua);

/*
 * Adds an entry to the uarray.
 *
 * Returns the index of the new element or -1 on failure.
 */
int uarray_add(uarray_st* ua, void* item);

/*
 * Edits an entry in the uarray.
 *
 * Returns the index of the edited element or -1 on failure.
 */
int uarray_edit(uarray_st* ua, int index, void* item);

/*
 * Deletes an entry from the uarray.
 *
 * Returns the amount of elements that have been actually deleted.
 */
int uarray_delete(uarray_st* ua, int index);

/*
 * Reads data at the given index from the uarray.
 *
 * Returns the pointer to the data.
 */
void* uarray_read(uarray_st* ua, int index);

/*
 * Returns the amount of used elements in the uarray.
 */
int uarray_used_len(uarray_st* ua);

/*
 * Returns the maximum amount of elements allowed in the uarray.
 */
int uarray_max_len(uarray_st* ua);

/*
 * Returns a comma seperated string of indices describing the used entries.
 */
void uarray_used_idxstr(uarray_st* ua, char* buffer);

#endif /* UARRAY_H */
