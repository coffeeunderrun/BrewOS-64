/*
 * Assumptions
 * - Loader passes higher half mmap address
 * - Kernel was loaded at 1 MiB (no available frames between 1 MiB and kernel)
 * - Memory map has last entry with a type of zero (temporary)
 */

#include <cpu.h>
#include <memory.h>
#include <status.h>
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
#define PAGE_PRESENT   (1ull << 0)    // Set if loaded into physical memory
#define PAGE_WRITE     (1ull << 1)    // Set if page can be written to, otherwise read-only
#define PAGE_USER      (1ull << 2)    // Set if page can be accessed by ring 3, otherwise only rings 0, 1, and 2
#define PAGE_PWT       (1ull << 3)    // Set for page-Level writethrough, otherwise writeback
#define PAGE_PCD       (1ull << 4)    // Set if page-Level cache is disabled, otherwise enabled
#define PAGE_ACCESSED  (1ull << 5)    // Set if page has been accessed
#define PAGE_DIRTY     (1ull << 6)    // Set if page has been written to (lowest level in hierarchy only)
#define PAGE_SIZE      (1ull << 7)    // Set if page is lowest level in hierarchy (P3E = 1 GiB, P2E = 2 MiB)
#define PAGE_GLOBAL    (1ull << 8)    // Set if page shouldn't invalidate on CR3 load (lowest level in hierarchy only)
#define PAGE_NOEXECUTE (1ull << 63)   // Set if page shouldn't allow code execution
#define PAGE_COUNT     (511ull << 52) // Number of pages present within a table (only in levels above lowest in hierarchy)

// Align up to nearest 4 KiB boundary
#define ALIGN(x) (((addr_t)(x) + 0xFFF) & 0xFFFFFFFFFFFFF000)

// Get lower half address of identity mapped virtual address
#define PADDR(x) ((addr_t)(x) & 0x7FFFFFFF)

typedef struct
{
    uint64_t base; // Memory region base address
    uint64_t size; // Size of memory region in bytes
    uint32_t type; // Memory region type
    uint32_t attr; // ACPI v3 attributes
} __attribute__((packed)) mmap_entry_t;

typedef uint64_t flag_t;
typedef uint64_t page_entry_t;

typedef struct
{
    page_entry_t entries[512];
} page_table_t;

static page_table_t *kernel_page_table_end;

static addr_t *frame_stack;

static mmap_entry_t *mmap_entry; // Pointer to current memory map entry
static uint64_t mmap_size;       // Bytes remaining to process in current entry
static addr_t mmap_addr;         // Physical address to process in current entry

static void push_stack_frame(addr_t);
static addr_t pop_stack_frame(void);
static addr_t pop_mmap_frame(void);

static status_t map_page(addr_t, flag_t, addr_t (*)(void));
static status_t unmap_page(addr_t, void (*)(addr_t));

static page_table_t *get_p4_table(addr_t);
static page_table_t *get_p3_table(addr_t);
static page_table_t *get_p2_table(addr_t);
static page_table_t *get_p1_table(addr_t);

static inline uint64_t get_p4e_index(addr_t);
static inline uint64_t get_p3e_index(addr_t);
static inline uint64_t get_p2e_index(addr_t);
static inline uint64_t get_p1e_index(addr_t);

extern const uint64_t kernel_start;
extern const uint64_t kernel_data;
extern const uint64_t kernel_end;

void init_memory(void *mmap)
{
    kernel_page_table_end = (page_table_t *)ALIGN(&kernel_end);

    page_table_t *p4 = kernel_page_table_end++;
    page_table_t *p3 = kernel_page_table_end++;
    page_table_t *p2 = kernel_page_table_end++;
    page_table_t *p1 = NULL;

    // Zero out P4, P3, and P2
    memset(p4, 0, 0x3000);

    p4->entries[510] = PADDR(p4) | PAGE_PRESENT | PAGE_WRITE; // P4 self reference
    p4->entries[511] = PADDR(p3) | PAGE_PRESENT | PAGE_WRITE; // Last 512 GiB
    p3->entries[510] = PADDR(p2) | PAGE_PRESENT | PAGE_WRITE; // Last 1 GiB

    // Identity map individual pages
    for(addr_t addr = 0; addr < PADDR(ALIGN(&kernel_end)); addr += 0x1000)
    {
        uint64_t p2e_idx = get_p2e_index(addr);
        if(!(p2->entries[p2e_idx] & PAGE_PRESENT))
        {
            p1 = kernel_page_table_end++;
            p2->entries[p2e_idx] = PADDR(p1) | PAGE_PRESENT | PAGE_WRITE;
            memset(p1, 0, 0x1000);
        }

        uint64_t p1e_idx = get_p1e_index(addr);
        if(addr >= PADDR(ALIGN(&kernel_start)) && addr < PADDR(ALIGN(&kernel_data)))
        {
            // Kernel code is read-only and executable
            p1->entries[p1e_idx] = addr | PAGE_PRESENT | PAGE_GLOBAL;
        }
        else
        {
            // Data is writable and not executable
            p1->entries[p1e_idx] = addr | PAGE_PRESENT | PAGE_WRITE | PAGE_GLOBAL | PAGE_NOEXECUTE;
        }
    }

    // Load new page tables
    load_pml4(PADDR(p4));

    // Bottom of frame stack starts immediately after kernel
    frame_stack = (addr_t *)ALIGN(&kernel_end);

    // Initialize memory map pop frame function
    mmap_entry = (mmap_entry_t *)mmap;
    mmap_addr = ALIGN(mmap_entry->base);
    mmap_size = mmap_entry->size;

    // Add available memory map frames to stack
    addr_t addr;
    while(mmap_entry != NULL && (addr = pop_mmap_frame()) > 0)
    {
        push_stack_frame(addr);
    }
}

static void push_stack_frame(addr_t paddr)
{
    status_t status = map_page((addr_t)frame_stack, PAGE_WRITE | PAGE_GLOBAL, pop_mmap_frame);
    if(status != STATUS_OKAY)
    {
        return;
    }

    // Fetch physical address and move stack frame pointer up
    *frame_stack++ = paddr;
}

static addr_t pop_stack_frame(void)
{
    if(frame_stack > (addr_t *)ALIGN(&kernel_end))
    {
        // Move frame stack pointer down and fetch physical address
        return *--frame_stack;
    }

    // No available frames
    return 0;
}

static addr_t pop_mmap_frame(void)
{
    while(mmap_entry != NULL && mmap_entry->type)
    {
        if(mmap_entry->type != MAP_AVAILABLE || mmap_size < 0x1000)
        {
            mmap_entry++;
            mmap_size = mmap_entry->size;
            continue;
        }

        addr_t addr = mmap_addr;
        mmap_addr += 0x1000; // Advance to next 4 KiB
        mmap_size -= 0x1000; // Reduce remaining region size by 4 KiB

        if(addr >= PADDR(&kernel_start) && addr < PADDR(kernel_page_table_end))
        {
            // Only claim frames above kernel and initial page tables
            continue;
        }

        return addr;
    }

    mmap_entry = NULL;
    return 0;
}

static status_t map_page(addr_t vaddr, flag_t flags, addr_t (*pop_frame_func)(void))
{
    page_table_t *p4 = get_p4_table(vaddr);
    page_table_t *p3 = get_p3_table(vaddr);
    page_table_t *p2 = get_p2_table(vaddr);
    page_table_t *p1 = get_p1_table(vaddr);

    uint64_t p4e_idx = get_p4e_index(vaddr);
    if(!(p4->entries[p4e_idx] & PAGE_PRESENT))
    {
        // Add P3 table for requested address
        addr_t paddr = (*pop_frame_func)();
        if(paddr == 0)
        {
            // No frames available
            return ERROR_OUT_OF_MEMORY;
        }

        // Page size and global flags must be 0 for P4 entries
        p4->entries[p4e_idx] = paddr | PAGE_PRESENT | (flags & ~PAGE_SIZE & ~PAGE_GLOBAL);
        flush_page(p4);
        memset(p3, 0, 0x1000);
    }

    uint64_t p3e_idx = get_p3e_index(vaddr);
    if(!(p3->entries[p3e_idx] & PAGE_PRESENT))
    {
        // Add P2 table for requested address
        addr_t paddr = (*pop_frame_func)();
        if(paddr == 0)
        {
            // No frames available
            return ERROR_OUT_OF_MEMORY;
        }

        // Do not allow 1 GiB pages
        p3->entries[p3e_idx] = paddr | PAGE_PRESENT | (flags & ~PAGE_SIZE);
        flush_page(p3);
        memset(p2, 0, 0x1000);
    }

    uint64_t p2e_idx = get_p2e_index(vaddr);
    if(!(p2->entries[p2e_idx] & PAGE_PRESENT))
    {
        // Add P1 table for requested address
        addr_t paddr = (*pop_frame_func)();
        if(paddr == 0)
        {
            // No frames available
            return ERROR_OUT_OF_MEMORY;
        }

        // Do not allow 2 MiB pages (this may change in the future)
        p2->entries[p2e_idx] = paddr | PAGE_PRESENT | (flags & ~PAGE_SIZE);
        flush_page(p2);
        memset(p1, 0, 0x1000);
    }

    uint64_t p1e_idx = get_p1e_index(vaddr);
    if(!(p1->entries[p1e_idx] & PAGE_PRESENT))
    {
        // Assign frame to page for requested address
        addr_t paddr = (*pop_frame_func)();
        if(paddr == 0)
        {
            // No frames available
            return ERROR_OUT_OF_MEMORY;
        }

        p1->entries[p1e_idx] = paddr | PAGE_PRESENT | flags;
        flush_page(p1);
    }

    return STATUS_OKAY;
}

static status_t unmap_page(addr_t vaddr, void (*push_frame_func)(addr_t))
{
    page_table_t *p1 = get_p1_table(vaddr);
    uint64_t p1e_idx = get_p1e_index(vaddr);

    if(!(p1->entries[p1e_idx] & PAGE_PRESENT))
    {
        // P1 entry is not present
        return STATUS_OKAY;
    }

    // Get physical address from P1 entry
    addr_t paddr = p1->entries[p1e_idx] & 0x7FFFF000;

    // Update P1 table
    p1->entries[p1e_idx] = ~PAGE_PRESENT;
    flush_page(p1);

    // Put frame back on stack
    (*push_frame_func)(paddr);

    // TODO: Keep track of present entries in page tables
    // TODO: Unmap page tables when present entry count reaches zero

    return STATUS_OKAY;
}

static page_table_t *get_p4_table(addr_t vaddr)
{
    // Keep sign extend
    uint64_t p4 = vaddr & 0xFFFF000000000000;

    // Reference P4 table recursively
    p4 |= 510ull << 39;
    p4 |= 510ull << 30;
    p4 |= 510ull << 21;
    p4 |= 510ull << 12;

    return (page_table_t *)p4;
}

static page_table_t *get_p3_table(addr_t vaddr)
{
    // Keep sign extend
    uint64_t p3 = vaddr & 0xFFFF000000000000;

    // Reference P4 table recursively
    p3 |= 510ull << 39;
    p3 |= 510ull << 30;
    p3 |= 510ull << 21;

    // Get index of P3 table
    p3 |= vaddr >> 27 & 0x1FF000;

    return (page_table_t *)p3;
}

static page_table_t *get_p2_table(addr_t vaddr)
{
    // Keep sign extend
    uint64_t p3 = vaddr & 0xFFFF000000000000;

    // Reference P4 table recursively
    p3 |= 510ull << 39;
    p3 |= 510ull << 30;

    // Get index of P3 and P2 tables
    p3 |= vaddr >> 18 & 0x3FFFF000;

    return (page_table_t *)p3;
}

static page_table_t *get_p1_table(addr_t vaddr)
{
    // Keep sign extend
    uint64_t p3 = vaddr & 0xFFFF000000000000;

    // Reference P4 table recursively
    p3 |= 510ull << 39;

    // Get index of P3, P2, and P1 tables
    p3 |= vaddr >> 9 & 0x7FFFFFF000;

    return (page_table_t *)p3;
}

static inline uint64_t get_p4e_index(addr_t vaddr)
{
    return (uint64_t)(vaddr >> 39 & 0x1FF);
}

static inline uint64_t get_p3e_index(addr_t vaddr)
{
    return (uint64_t)(vaddr >> 30 & 0x1FF);
}

static inline uint64_t get_p2e_index(addr_t vaddr)
{
    return (uint64_t)(vaddr >> 21 & 0x1FF);
}

static inline uint64_t get_p1e_index(addr_t vaddr)
{
    return (uint64_t)(vaddr >> 12 & 0x1FF);
}
