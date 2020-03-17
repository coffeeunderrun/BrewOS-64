#include <stdlib.h>
#include <sys/mman.h>

#define MAGIC 0xCAFEBABE

#define ATTR_FREE (1 << 0) // Block is available

typedef struct block_header
{
    uint64_t sign : 32;        // Block signature
    uint64_t attr : 32;        // Block attributes
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

void *calloc(size_t el_cnt, size_t el_size)
{
    if(el_cnt == 0 || el_size == 0)
    {
        // Invalid element count or size requested
        return NULL;
    }

    return NULL;
}

void free(void *addr)
{
    if(!addr)
    {
        // Invalid memory address
        return;
    }

    block_header_t *blk = get_block_head(addr);

    if(!blk || blk->sign != MAGIC)
    {
        // Invalid block
        return;
    }

    if(blk->attr & ATTR_FREE)
    {
        // Block is already free
        return;
    }

    blk->attr |= ATTR_FREE;

    defrag_blocks(blk);
}

void *malloc(size_t size)
{
    if(size == 0)
    {
        // Invalid size requested
        return NULL;
    }

    block_header_t *blk = find_best_fit(size);

    if(!blk)
    {
        // Unable to allocate space in heap
        return NULL;
    }

    blk->attr &= ~ATTR_FREE;

    size_t needed = sizeof(block_header_t) + size;
    size_t remain = blk->size - needed;

    // Split block if there is enough room for a header in a new block
    if(blk->size > needed && remain >= sizeof(block_header_t))
    {
        block_header_t *new_blk = (block_header_t *)((uint64_t)blk + needed);
        *new_blk = (block_header_t){ MAGIC, ATTR_FREE, remain, blk->next, blk };

        defrag_blocks(new_blk);

        blk->size = needed;
        blk->next = new_blk;
    }

    return get_block_data(blk);
}

void *realloc(void *addr, size_t size)
{
    if(!addr)
    {
        // Invalid memory address
        return NULL;
    }

    if(size == 0)
    {
        // Invalid size requested
        return NULL;
    }

    return NULL;
}

static block_header_t *get_first_block(void)
{
    block_header_t *blk = (block_header_t *)&heap_start;

    if(blk && blk->sign != MAGIC)
    {
        // One-time initialization of first block
        size_t size = (size_t)&heap_end - (size_t)&heap_start;
        *blk = (block_header_t){ MAGIC, ATTR_FREE, size, NULL, NULL };
    }

    return blk;
}

static block_header_t *find_best_fit(size_t size)
{
    block_header_t *blk = get_first_block();
    block_header_t *best = NULL;

    for( ; blk && blk->sign == MAGIC; blk = blk->next)
    {
        if(!(blk->attr & ATTR_FREE))
        {
            // Ignore unavailable blocks
            continue;
        }

        size_t needed = sizeof(block_header_t) + size;

        if(blk->size < needed)
        {
            // Current block doesn't have enough space
            continue;
        }

        if(!best || blk->size < best->size)
        {
            // Found either the first best block or a better fit
            best = blk;
        }
    }

    return best;
}

static void defrag_blocks(block_header_t *blk)
{
    if(!blk || blk->sign != MAGIC || !(blk->attr & ATTR_FREE))
    {
        // Canot defrag an invalid or unavailable block
        return;
    }

    // Merge free blocks following current block
    while(blk->next && blk->next->sign == MAGIC && blk->next->attr & ATTR_FREE)
    {
        blk->size += blk->next->size;
        blk->next = blk->next->next;
    }

    // Merge free block preceding current block
    while(blk->prev && blk->prev->sign == MAGIC && blk->prev->attr & ATTR_FREE)
    {
        blk->size += blk->prev->size;
        blk->prev = blk->prev->prev;
    }
}

static inline block_header_t *get_block_head(void *addr)
{
    return (block_header_t *)((uint64_t)addr - sizeof(block_header_t));
}

static inline void *get_block_data(block_header_t *blk)
{
    return (void *)((uint64_t)blk + sizeof(block_header_t));
}
