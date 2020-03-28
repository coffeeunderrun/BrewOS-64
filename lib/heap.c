#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#define MAGIC 0xCAFEBABE

#define ATTR_FREE (1 << 0) // Block is available

#define IS_FREE(blk)  (blk && (blk->attr & ATTR_FREE))
#define IS_VALID(blk) (blk && (blk->sign == MAGIC))

typedef struct block_header
{
    uint64_t sign : 32;        // Block signature
    uint64_t attr : 32;        // Block attributes
    size_t size;               // Length of block in bytes
    struct block_header *next; // Next block header
    struct block_header *prev; // Previous block header
} block_header_t;

static block_header_t *get_first_block(void);
static block_header_t *find_best_fit(size_t size);
static block_header_t *expand_block(block_header_t *blk, size_t size);
static block_header_t *shrink_block(block_header_t *blk, size_t size);
static void defrag_blocks(block_header_t *blk);

static inline block_header_t *get_block_head(void *addr);
static inline void *get_block_data(block_header_t *blk);

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

    if(!IS_VALID(blk))
    {
        // Invalid block
        return;
    }

    if(IS_FREE(blk))
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

    if(!IS_VALID(blk))
    {
        return NULL;
    }

    blk->attr &= ~ATTR_FREE;

    blk = shrink_block(blk, size);

    return get_block_data(blk);
}

void *realloc(void *addr, size_t size)
{
    if(!addr)
    {
        return malloc(size);
    }

    if(size == 0)
    {
        free(addr);
        return NULL;
    }

    block_header_t *blk = get_block_head(addr);

    if(!IS_VALID(blk))
    {
        return NULL;
    }

    if(blk->size > size + sizeof(block_header_t))
    {
        blk = shrink_block(blk, size);
    }
    else
    {
        blk = expand_block(blk, size);
    }

    return get_block_data(blk);
}

static block_header_t *expand_block(block_header_t *blk, size_t size)
{
    if(!IS_VALID(blk))
    {
        // Invalid block
        return NULL;
    }

    if(IS_FREE(blk))
    {
        // Cannot expand a free block
        return blk;
    }

    size_t old_size = blk->size;
    size_t new_size = sizeof(block_header_t) + size;

    // Attempt to fit new size by appending any following free blocks to the current block
    for(block_header_t *next = blk->next; IS_FREE(next) && new_size > blk->size; next = next->next)
    {
        if(!IS_VALID(next))
        {
            // Corrupt block
            return NULL;
        }

        blk->size += next->size;
        blk->next = next->next;
    }

    if(blk->size >= new_size)
    {
        // Shrink merged blocks to new size
        return shrink_block(blk, size);
    }

    // Could not expand current block so try finding a new block
    void *new_addr = malloc(size);

    if(!new_addr)
    {
        return NULL;
    }

    // Copy old block data to new block
    memmove(new_addr, get_block_data(blk), old_size);

    // Free up old block
    free(get_block_data(blk));

    return get_block_head(new_addr);
}

static block_header_t *shrink_block(block_header_t *blk, size_t size)
{
    if(!IS_VALID(blk))
    {
        // Invalid block
        return NULL;
    }

    if(IS_FREE(blk))
    {
        // Cannot shrink a free block
        return blk;
    }

    size_t new_size = sizeof(block_header_t) + size;
    size_t leftover = blk->size - new_size;

    if(new_size >= blk->size || leftover < sizeof(block_header_t))
    {
        return blk;
    }

    // Split block if there is enough room for a header in a new block
    block_header_t *new_blk = (block_header_t *)((uint64_t)blk + new_size);
    *new_blk = (block_header_t){ MAGIC, ATTR_FREE, leftover, blk->next, blk };

    defrag_blocks(new_blk);

    blk->size = new_size;
    blk->next = new_blk;

    return blk;
}

static block_header_t *get_first_block(void)
{
    block_header_t *blk = (block_header_t *)&heap_start;

    if(blk && !IS_VALID(blk))
    {
        // One-time initialization of first block
        size_t size = (size_t)&heap_end - (size_t)&heap_start;
        *blk = (block_header_t){ MAGIC, ATTR_FREE, size, NULL, NULL };
    }

    return blk;
}

static block_header_t *find_best_fit(size_t size)
{
    block_header_t *best = NULL;

    for(block_header_t *blk = get_first_block(); IS_VALID(blk); blk = blk->next)
    {
        if(!IS_FREE(blk))
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
    if(!IS_VALID(blk) || !IS_FREE(blk))
    {
        // Cannot defrag an invalid or unavailable block
        return;
    }

    // Merge free blocks following current block
    while(IS_VALID(blk->next) && IS_FREE(blk->next))
    {
        blk->size += blk->next->size;
        blk->next = blk->next->next;
    }

    // Merge free block preceding current block
    while(IS_VALID(blk->prev) && IS_FREE(blk->prev))
    {
        blk->size += blk->prev->size;
        blk->prev = blk->prev->prev;
    }
}

static inline block_header_t *get_block_head(void *addr)
{
    if(!addr)
    {
        return NULL;
    }

    return (block_header_t *)((uint64_t)addr - sizeof(block_header_t));
}

static inline void *get_block_data(block_header_t *blk)
{
    if(!IS_VALID(blk))
    {
        return NULL;
    }

    return (void *)((uint64_t)blk + sizeof(block_header_t));
}
