#include <stdlib.h>
#include <string.h>
#include <vector.h>

static inline void resize(vector_t *vec, size_t size);

void vec_init(vector_t *vec, unsigned int cap)
{
    vec->data = malloc(cap * sizeof(void *));
    vec->cap = cap;
    vec->cnt = 0;
}

void vec_push(vector_t *vec, void *item)
{
    if(!vec || !vec->data)
    {
        return;
    }

    if(vec->cap == vec->cnt)
    {
        resize(vec, vec->cap * 2);
    }

    vec->data[vec->cnt++] = item;
}

void *vec_pop(vector_t *vec)
{
    if(!vec || !vec->data || !vec->cnt)
    {
        return NULL;
    }

    void *item = vec->data[--vec->cnt];

    if(vec->cnt && vec->cnt == vec->cap / 4)
    {
        resize(vec, vec->cap / 2);
    }

    return item;
}

void vec_ins(vector_t *vec, unsigned int idx, void *item)
{
    if(!vec || !vec->data || idx > vec->cnt)
    {
        return;
    }

    if(vec->cap == vec->cnt)
    {
        resize(vec, vec->cap * 2);
    }

    size_t move_size = (vec->cnt - idx) * sizeof(void *);
    memmove(&vec->data[idx + 1], &vec->data[idx], move_size);

    vec->data[idx] = item;
    vec->cnt++;
}

void vec_del(vector_t *vec, unsigned int idx)
{
    if(!vec || !vec->data || idx >= vec->cnt)
    {
        return;
    }

    size_t move_size = (vec->cnt - idx - 1) * sizeof(void *);
    memmove(&vec->data[idx], &vec->data[idx + 1], move_size);

    vec->cnt--;

    if(vec->cnt && vec->cnt == vec->cap / 4)
    {
        resize(vec, vec->cap / 2);
    }
}

void vec_set(vector_t *vec, unsigned int idx, void *item)
{
    if(!vec || !vec->data || idx >= vec->cnt)
    {
        return;
    }

    vec->data[idx] = item;
}

void *vec_get(vector_t *vec, unsigned int idx)
{
    if(!vec || !vec->data || idx >= vec->cnt)
    {
        return NULL;
    }

    return vec->data[idx];
}

unsigned int vec_cnt(vector_t *vec)
{
    if(!vec)
    {
        return 0;
    }

    return vec->cnt;
}

unsigned int vec_cap(vector_t *vec)
{
    if(!vec)
    {
        return 0;
    }

    return vec->cap;
}

void vec_free(vector_t *vec)
{
    if(!vec || !vec->data)
    {
        return;
    }

    free(vec->data);
}

static inline void resize(vector_t *vec, size_t size)
{
    void **data = realloc(vec->data, size * sizeof(void *));

    if(!data)
    {
        return;
    }

    vec->data = data;
    vec->cap = size;
}
