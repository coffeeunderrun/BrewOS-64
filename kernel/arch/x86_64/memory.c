#include <cpu.h>
#include <memory.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

// Memory Map Types
#define MAP_AVAILABLE   1 // Available
#define MAP_RESERVED    2 // Reserved (DO NOT USE)
#define MAP_RECLAIMABLE 3 // ACPI Reclaimable (do NOT use until AFTER ACPI initialization)
#define MMT_NONVOLATILE 4 // ACPI Non-volatile (DO NOT USE)
#define MMT_BAD         5 // Bad (DO NOT USE)

// Page Translation Table Entry Field Bits
#define PAGE_PRESENT   (1ull << 0)  // Set if loaded into physical memory
#define PAGE_WRITE     (1ull << 1)  // Set if page can be written to, otherwise read-only
#define PAGE_USER      (1ull << 2)  // Set if page can be accessed by ring 3, otherwise only rings 0, 1, and 2
#define PAGE_PWT       (1ull << 3)  // Set for page-Level writethrough, otherwise writeback
#define PAGE_PCD       (1ull << 4)  // Set if page-Level cache is disabled, otherwise enabled
#define PAGE_ACCESSED  (1ull << 5)  // Set if page has been accessed
#define PAGE_DIRTY     (1ull << 6)  // Set if page has been written to (lowest level in hierarchy only)
#define PAGE_SIZE      (1ull << 7)  // Set if page is lowest level in hierarchy (P3E = 1 GiB, P2E = 2 MiB)
#define PAGE_GLOBAL    (1ull << 8)  // Set if page shouldn't invalidate on CR3 load (lowest level in hierarchy only)
#define PAGE_NOEXECUTE (1ull << 63) // Set if page shouldn't allow code execution

// Align up to nearest 4 KiB boundary
#define ALIGN(x) (((uint64_t)(x) + 0xFFF) & 0xFFFFFFFFFFFFF000)

// Get lower half address of identity mapped virtual address
#define PADDR(x) ((uint64_t)(x) & 0x7FFFFFFF)

typedef struct
{
    uint64_t addr; // Memory region base address
    uint64_t size; // Size of memory region in bytes
    uint32_t type; // Memory region type
    uint32_t attr; // ACPI v3 attributes
} __attribute__((packed)) mmap_entry_t;

typedef uint64_t frame_t;
typedef uint64_t page_t;
typedef uint64_t page_entry_t;
typedef uint64_t page_table_t;

extern const uint64_t kernel_start;
extern const uint64_t kernel_data;
extern const uint64_t kernel_end;

static frame_t *frame_stack;

static void push_frame(frame_t);
static frame_t pop_frame();

static void map_page(page_t *, uint64_t);
static void unmap_page(page_t *);

static inline page_table_t *get_p4(page_t *);
static inline page_table_t *get_p3(page_t *);
static inline page_table_t *get_p2(page_t *);
static inline page_table_t *get_p1(page_t *);

static inline page_entry_t *get_p4_entry(page_t *, page_table_t *);
static inline page_entry_t *get_p3_entry(page_t *, page_table_t *);
static inline page_entry_t *get_p2_entry(page_t *, page_table_t *);
static inline page_entry_t *get_p1_entry(page_t *, page_table_t *);

void init_memory(void *mmap)
{
    page_table_t *p4 = (uint64_t *)ALIGN(&kernel_end);
    page_table_t *p3 = p4 + 0x200;
    page_table_t *p2 = p3 + 0x200;
    page_table_t *pn = p2 + 0x200;
    page_table_t *p1 = NULL;

    memset(p4, 0, 0x3000);

    p4[510] = PADDR(p4) | PAGE_PRESENT | PAGE_WRITE; // PML4 recursion
    p4[511] = PADDR(p3) | PAGE_PRESENT | PAGE_WRITE; // Last 512 GiB
    p3[510] = PADDR(p2) | PAGE_PRESENT | PAGE_WRITE; // Last 1 GiB

    uint64_t pk_start = PADDR(ALIGN(&kernel_start));
    uint64_t pk_data = PADDR(ALIGN(&kernel_data));
    uint64_t pk_end = PADDR(ALIGN(&kernel_end));

    // Identity map individual pages
    for(uint64_t addr = 0; addr < pk_end; addr += 0x1000)
    {
        page_entry_t *p2e = get_p2_entry((page_t *)addr, p2);
        if(!(*p2e & PAGE_PRESENT))
        {
            p1 = pn;
            pn += 0x200;

            memset(p1, 0, 0x1000);

            *p2e = PADDR(p1) | PAGE_PRESENT | PAGE_WRITE;
        }

        page_entry_t *p1e = get_p1_entry((page_t *)addr, p1);
        if(addr >= pk_start && addr < pk_data)
        {
            // Kernel code is read-only and executable
            *p1e = addr | PAGE_PRESENT | PAGE_GLOBAL;
        }
        else
        {
            // Data is writable and not executable
            *p1e = addr | PAGE_PRESENT | PAGE_WRITE | PAGE_GLOBAL | PAGE_NOEXECUTE;
        }
    }

    // Load new page tables
    load_pml4(PADDR(p4));

    // Bottom of frame stack starts immediately after kernel
    frame_stack = (uint64_t *)ALIGN(&kernel_end);

    for(mmap_entry_t *entry = (mmap_entry_t *)mmap; entry->type; entry++)
    {
        if(entry->type != MAP_AVAILABLE)
        {
            // Ignore memory ranges that aren't available
            continue;
        }

        for(uint64_t addr = ALIGN(entry->addr), size = entry->size; size > 0xFFF; addr += 0x1000, size -= 0x1000)
        {
            if(addr < 0x100000)
            {
                // Ignore frames below 1 MiB
                continue;
            }

            if(addr >= PADDR(&kernel_start) && addr < PADDR(pn))
            {
                // Ignore frames used for kernel and page tables
                continue;
            }

            // Push physical address to frame stack
            push_frame(addr);
        }
    }
}

static void push_frame(frame_t frame)
{
    // Next 1 GiB if needed
    page_table_t *p3 = get_p3(frame_stack);
    page_entry_t *p3e = get_p3_entry(frame_stack, p3);
    if(!(*p3e & PAGE_PRESENT))
    {
        *p3e = frame | PAGE_PRESENT | PAGE_WRITE;
        flush_page(p3);
        return;
    }

    // Next 2 MiB if needed
    page_table_t *p2 = get_p2(frame_stack);
    page_entry_t *p2e = get_p2_entry(frame_stack, p2);
    if(!(*p2e & PAGE_PRESENT))
    {
        *p2e = frame | PAGE_PRESENT | PAGE_WRITE;
        flush_page(p2);
        return;
    }

    // Next 4 KiB if needed
    page_table_t *p1 = get_p1(frame_stack);
    page_entry_t *p1e = get_p1_entry(frame_stack, p1);
    if(!(*p1e & PAGE_PRESENT))
    {
        *p1e = frame | PAGE_PRESENT | PAGE_WRITE | PAGE_GLOBAL;
        flush_page(p1);
        return;
    }

    // Fetch physical address and move stack frame pointer up
    *frame_stack++ = frame;
}

static frame_t pop_frame()
{
    if(frame_stack > (uint64_t *)ALIGN(&kernel_end))
    {
        // Move frame stack pointer down and fetch physical address
        return *--frame_stack;
    }

    // No available frames
    return 0;
}

static void map_page(page_t *page, uint64_t flags)
{
    // Next 512 GiB if needed
    page_table_t *p4 = get_p4(page);
    page_entry_t *p4e = get_p4_entry(page, p4);
    if(!(*p4e & PAGE_PRESENT))
    {
        *p4e = pop_frame() | PAGE_PRESENT | (flags & ~PAGE_SIZE & ~PAGE_GLOBAL);
        flush_page(p4);
    }

    // Next 1 GiB if needed
    page_table_t *p3 = get_p3(page);
    page_entry_t *p3e = get_p3_entry(page, p3);
    if(!(*p3e & PAGE_PRESENT))
    {
        *p3e = pop_frame() | PAGE_PRESENT | (flags & ~PAGE_SIZE);
        flush_page(p3);
    }

    // Next 2 MiB if needed
    page_table_t *p2 = get_p2(page);
    page_entry_t *p2e = get_p2_entry(page, p2);
    if(!(*p2e & PAGE_PRESENT))
    {
        *p2e = pop_frame() | PAGE_PRESENT | (flags & ~PAGE_SIZE);
        flush_page(p2);
    }

    // Next 4 KiB if needed
    page_table_t *p1 = get_p1(page);
    page_entry_t *p1e = get_p1_entry(page, p1);
    if(!(*p1e & PAGE_PRESENT))
    {
        *p1e = pop_frame() | PAGE_PRESENT | flags;
        flush_page(p1);
    }
}

static void unmap_page(page_t *page)
{
    page_table_t *p1 = get_p1(page);
    page_entry_t *p1e = get_p1_entry(page, p1);
    frame_t frame = *p1e & 0x7FFFF000;
    if(*p1e & PAGE_PRESENT)
    {
        *p1e = ~PAGE_PRESENT;
        flush_page(p1);
        push_frame(frame);
    }
}

static inline page_table_t *get_p4(page_t *page)
{
    return (page_table_t *)(((uint64_t)page & 0xFFFF000000000000) | 0xFF7FBFDFE000);
}

static inline page_table_t *get_p3(page_t *page)
{
    return (page_table_t *)(((uint64_t)page & 0xFFFF000000000000) | 0xFF7FBFC00000 | ((uint64_t)page >> 27 & 0x1FF000));
}

static inline page_table_t *get_p2(page_t *page)
{
    return (page_table_t *)(((uint64_t)page & 0xFFFF000000000000) | 0xFF7F80000000 | ((uint64_t)page >> 18 & 0x3FFFF000));
}

static inline page_table_t *get_p1(page_t *page)
{
    return (page_table_t *)(((uint64_t)page & 0xFFFF000000000000) | 0xFF0000000000 | ((uint64_t)page >> 9 & 0x7FFFFFF000));
}

static inline page_entry_t *get_p4_entry(page_t *page, page_table_t *table)
{
    return (page_entry_t *)&table[((uint64_t)page >> 39) & 0x1FF];
}

static inline page_entry_t *get_p3_entry(page_t *page, page_table_t *table)
{
    return (page_entry_t *)&table[((uint64_t)page >> 30) & 0x1FF];
}

static inline page_entry_t *get_p2_entry(page_t *page, page_table_t *table)
{
    return (page_entry_t *)&table[((uint64_t)page >> 21) & 0x1FF];
}

static inline page_entry_t *get_p1_entry(page_t *page, page_table_t *table)
{
    return (page_entry_t *)&table[((uint64_t)page >> 12) & 0x1FF];
}
