#include <mem/pmm.h>
#include <stdint.h>

#define MMAP_TYPE_AVAILABLE   1
#define MMAP_TYPE_RESERVED    2
#define MMAP_TYPE_RECLAIMABLE 3
#define MMAP_TYPE_NONVOLATILE 4
#define MMAP_TYPE_BAD         5

typedef struct
{
    uint64_t addr;
    uint64_t size;
    uint32_t type;
    uint32_t attr;
} __attribute__((packed)) mmap_entry_t;

extern const uint64_t kernel_start;
extern const uint64_t kernel_data;
extern const uint64_t kernel_end;

void pmm_init(void *mmap)
{
    for(mmap_entry_t *entry = (mmap_entry_t *)mmap; entry->type; entry++)
    {
        if(entry->type > 1)
        {
            continue;
        }
    }
}
