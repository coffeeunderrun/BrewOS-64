#include <stdlib.h>
#include <sys/mman.h>

#define MAGIC 0xCAFEBABE

// Align up to nearest 8 bytes
#define ALIGN(x) (((size_t)(x) + 7) & ~7ull)

typedef struct block_header
{
    uint64_t sign : 32;        // Block signature
    uint64_t free : 1;         // Set if block is available
    uint64_t rsvd : 31;        // Unused
    size_t size;               // Length of block in bytes
    struct block_header *next; // Next block header
    struct block_header *prev; // Previous block header
} block_header_t;

static block_header_t *get_first_block(void);
static block_header_t *find_best_fit(size_t);
static void defrag_blocks(block_header_t *);

static inline block_header_t *get_block_head(void *);
static inline void *get_block_data(block_header_t *);

extern uint64_t heap_start;
extern uint64_t heap_end;

void *calloc(size_t n, size_t sz)
{
    return NULL;
}

void free(void *p)
{
    block_header_t *blk = get_block_head(p);

    if(blk->sign != MAGIC)
    {
        // Invalid block signature
        return;
    }

    if(blk->free)
    {
        // Block is already free
        return;
    }

    blk->free = true;
    defrag_blocks(blk);
}

void *malloc(size_t sz)
{
    block_header_t *blk = find_best_fit(sz);

    if(blk == NULL)
    {
        // Unable to allocate space in heap
        return NULL;
    }

    blk->free = false;

    size_t needed = sizeof(block_header_t) + ALIGN(sz);
    size_t remain = blk->size - needed;

    // Split block if there is enough room for a header in a new block
    if(blk->size > needed && remain >= sizeof(block_header_t))
    {
        block_header_t *new_blk = (block_header_t *)((uint64_t)blk + needed);
        *new_blk = (block_header_t){ MAGIC, true, 0, remain, blk->next, blk };
        defrag_blocks(new_blk);

        blk->size = needed;
        blk->next = new_blk;
    }

    return get_block_data(blk);
}

void *realloc(void *p, size_t sz)
{
    return NULL;
}

static block_header_t *get_first_block(void)
{
    block_header_t *blk = (block_header_t *)&heap_start;

    if(blk->sign != MAGIC)
    {
        // One-time initialization of first block
        size_t size = (size_t)&heap_end - (size_t)&heap_start;
        *blk = (block_header_t){ MAGIC, true, 0, size, NULL, NULL };
    }

    return blk;
}

static block_header_t *find_best_fit(size_t sz)
{
    block_header_t *blk = get_first_block();
    block_header_t *best_blk = NULL;

    for( ; blk != NULL && blk->sign == MAGIC; blk = blk->next)
    {
        if(!blk->free)
        {
            // Ignore unavailable blocks
            continue;
        }

        size_t needed = sizeof(block_header_t) + ALIGN(sz);

        if(blk->size < needed)
        {
            // Current block doesn't have enough space
            continue;
        }

        if(best_blk == NULL || blk->size < best_blk->size)
        {
            // Found either the first best block or a better fit
            best_blk = blk;
        }
    }

    return best_blk;
}

static void defrag_blocks(block_header_t *blk)
{
    if(blk == NULL || blk->sign != MAGIC || !blk->free)
    {
        // Canot defrag an invalid or unavailable block
        return;
    }

    // Merge free blocks following current block
    while(blk->next != NULL && blk->next->sign == MAGIC && blk->next->free)
    {
        blk->size += blk->next->size;
        blk->next = blk->next->next;
    }

    // Merge free block preceding current block
    while(blk->prev != NULL && blk->prev->sign == MAGIC && blk->prev->free)
    {
        blk->size += blk->prev->size;
        blk->prev = blk->prev->prev;
    }
}

static inline block_header_t *get_block_head(void *p)
{
    return (block_header_t *)((uint64_t)p - sizeof(block_header_t));
}

static inline void *get_block_data(block_header_t *blk)
{
    return (void *)((uint64_t)blk + sizeof(block_header_t));
}
