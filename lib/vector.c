#include <stdlib.h>
#include <vector.h>

#define INITIAL_CAP 10

static void vec_resize(vector_t *, size_t);

void vec_init(vector_t *vec)
{
    vec->cap = INITIAL_CAP;
    vec->cnt = 0;
    vec->items = malloc(sizeof(void *) * vec->cap);
}

void vec_push(vector_t *vec, void *item)
{
    if(vec->cap == vec->cnt)
    {
        vec_resize(vec, vec->cap * 2);
    }

    vec->items[vec->cnt++] = item;
}

void *vec_pop(vector_t *vec)
{
    if(vec->cnt == 0)
    {
        return NULL;
    }

    void *item = vec->items[--vec->cnt];

    if(vec->cnt > 0 && vec->cnt == vec->cap / 4)
    {
        vec_resize(vec, vec->cap / 2);
    }

    return item;
}

void vec_ins(vector_t *vec, unsigned int idx, void *item)
{
    if(idx > vec->cnt)
    {
        return;
    }

    if(vec->cap == vec->cnt + 1)
    {
        vec_resize(vec, vec->cap * 2);
    }

    for(unsigned int i = vec->cnt; i > idx; i--)
    {
        vec->items[i] = vec->items[i - 1];
    }

    vec->items[idx] = item;
    vec->cnt++;
}

void vec_del(vector_t *vec, unsigned int idx)
{
    if(vec->cnt < idx)
    {
        return;
    }

    vec->items[idx] = NULL;

    for(unsigned int i = idx; i < vec->cnt - 1; i++)
    {
        vec->items[i] = vec->items[i + 1];
        vec->items[i + 1] = NULL;
    }

    vec->cnt--;

    if(vec->cnt > 0 && vec->cnt == vec->cap / 4)
    {
        vec_resize(vec, vec->cap / 2);
    }
}

void vec_set(vector_t *vec, unsigned int idx, void *item)
{
    if(idx > vec->cnt)
    {
        vec->items[idx] = item;
    }
}

void *vec_get(vector_t *vec, unsigned int idx)
{
    if(idx < vec->cnt)
    {
        return vec->items[idx];
    }

    return NULL;
}

unsigned int vec_cnt(vector_t *vec)
{
    return vec->cnt;
}

unsigned int vec_cap(vector_t *vec)
{
    return vec->cap;
}

void vec_free(vector_t *vec)
{
    free(vec->items);
}

static void vec_resize(vector_t *vec, size_t size)
{
    void **items = realloc(vec->items, sizeof(void *) * size);

    if(items)
    {
        vec->items = items;
        vec->cap = size;
    }
}
