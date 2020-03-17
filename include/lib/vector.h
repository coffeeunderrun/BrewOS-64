#ifndef VECTOR_H
#define VECTOR_H

#include <sys/types.h>

typedef struct vector
{
    void **items; // Pointer to list of pointers
    size_t cap;   // Max number of items for currently allocated space
    size_t cnt;   // Number of items
} vector_t;

/*
 * Allocate memory for initial list using a default capacity
 */
void vec_init(vector_t *vec);

/*
 * Append item to end of list
 */
void vec_push(vector_t *vec, void *item);

/*
 * Remove and return item from end of list
 */
void *vec_pop(vector_t *vec);

/*
 * Add item to list at specific index
 */
void vec_ins(vector_t *vec, unsigned int idx, void *item);

/*
 * Remove item from list at specific index
 */
void vec_del(vector_t *vec, unsigned int idx);

/*
 * Change item in list at specific index
 */
void vec_set(vector_t *vec, unsigned int idx, void *item);

/*
 * Return item from list at specific index
 */
void *vec_get(vector_t *vec, unsigned int idx);

/*
 * Return number of items in list
 */
unsigned int vec_cnt(vector_t *vec);

/*
 * Return current capacity of list
 */
unsigned int vec_cap(vector_t *vec);

/*
 * Free memory allocated for the list
 */
void vec_free(vector_t *vec);

#endif // VECTOR_H
