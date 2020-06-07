/*
 * Assumptions
 * - Loader passes higher half mmap address
 * - Kernel was loaded at 1 MiB (no available frames between 1 MiB and kernel)
 * - Memory map has last entry with a type of zero (temporary)
 */

#include <arch/x86_64/arch.hpp>
#include <cerrno>
#include <cstdint>
#include <cstring>
#include <memory.hpp>

extern const uintptr_t _kernel_start;
extern const uintptr_t _kernel_data;
extern const uintptr_t _kernel_end;

namespace BrewOS {
namespace Memory {

// Memory Map Types
const uint64_t MAP_AVAILABLE = 1;   // Available
const uint64_t MAP_RESERVED = 2;    // Reserved (DO NOT USE)
const uint64_t MAP_RECLAIMABLE = 3; // ACPI Reclaimable (do NOT use until AFTER ACPI initialization)
const uint64_t MAP_NONVOLATILE = 4; // ACPI Non-volatile (DO NOT USE)
const uint64_t MAP_BAD = 5;         // Bad (DO NOT USE)

// Page Translation Table Entry Field Bits
const uint64_t PAGE_PRESENT = 1ull << 0;    // Set if loaded into physical memory
const uint64_t PAGE_WRITE = 1ull << 1;      // Set if page can be written to, otherwise read-only
const uint64_t PAGE_USER = 1ull << 2;       // Set if page can be accessed by ring 3, otherwise only rings 0, 1, and 2
const uint64_t PAGE_PWT = 1ull << 3;        // Set for page-Level writethrough, otherwise writeback
const uint64_t PAGE_PCD = 1ull << 4;        // Set if page-Level cache is disabled, otherwise enabled
const uint64_t PAGE_ACCESSED = 1ull << 5;   // Set if page has been accessed
const uint64_t PAGE_DIRTY = 1ull << 6;      // Set if page has been written to (lowest level in hierarchy only)
const uint64_t PAGE_SIZE = 1ull << 7;       // Set if page is lowest level in hierarchy (P3E = 1 GiB, P2E = 2 MiB)
const uint64_t PAGE_GLOBAL = 1ull << 8;     // Set if page shouldn't invalidate on CR3 load (lowest level in hierarchy only)
const uint64_t PAGE_NOEXECUTE = 1ull << 63; // Set if page shouldn't allow code execution

// Align up to nearest 4 KiB boundary
#define ALIGN(x) (((uintptr_t)(x) + 0xFFF) & 0xFFFFFFFFFFFFF000)

// Get lower half address of identity mapped virtual address
#define PADDR(x) ((uintptr_t)(x)&0x7FFFFFFF)

typedef struct {
    uint64_t base; // Memory region base address
    uint64_t size; // Size of memory region in bytes
    uint32_t type; // Memory region type
    uint32_t attr; // ACPI v3 attributes
} __attribute__((packed)) MemMapEntry;

typedef struct {
    uint64_t entries[512];
} PageTable;

static PageTable* kernel_page_table_end;

static uintptr_t* frame_stack;

static MemMapEntry* mmap_entry; // Pointer to current memory map entry
static uintptr_t mmap_addr;     // Physical address to process in current entry
static size_t mmap_size;        // Bytes remaining to process in current entry

static void
PushStackFrame(uintptr_t addr);

static uintptr_t
PopStackFrame(void);

static uintptr_t
PopMemMapFrame(void);

static err_t
MapPage(uintptr_t addr, int flags, uintptr_t (*pop)(void));

static err_t
UnmapPage(uintptr_t addr, void (*push)(uintptr_t));

static PageTable*
GetP4Table(uintptr_t addr);

static PageTable*
GetP3Table(uintptr_t addr);

static PageTable*
GetP2Table(uintptr_t addr);

static PageTable*
GetP1Table(uintptr_t addr);

static inline uint64_t
GetP4EntryIndex(uintptr_t addr);

static inline uint64_t
GetP3EntryIndex(uintptr_t addr);

static inline uint64_t
GetP2EntryIndex(uintptr_t addr);

static inline uint64_t
GetP1EntryIndex(uintptr_t addr);

static inline void
IncEntryCount(uint64_t* entry);

static inline void
DecEntryCount(uint64_t* entry);

static inline bool
HasEntries(uint64_t entry);

static const uintptr_t kernel_start = (uintptr_t)&_kernel_start;
static const uintptr_t kernel_data = (uintptr_t)&_kernel_data;
static const uintptr_t kernel_end = (uintptr_t)&_kernel_end;

void
Initialize(void* mmap)
{
    kernel_page_table_end = (PageTable*)ALIGN(kernel_end);

    PageTable* p4 = kernel_page_table_end++;
    PageTable* p3 = kernel_page_table_end++;
    PageTable* p2 = kernel_page_table_end++;
    PageTable* p1 = nullptr;

    // Zero out P4, P3, and P2
    memset(p4, 0, 0x3000);

    p4->entries[510] = PADDR(p4) | PAGE_PRESENT | PAGE_WRITE; // P4 self reference
    p4->entries[511] = PADDR(p3) | PAGE_PRESENT | PAGE_WRITE; // Last 512 GiB
    p3->entries[510] = PADDR(p2) | PAGE_PRESENT | PAGE_WRITE; // Last 1 GiB

    // Increase P3 present entry count in P4 entry
    IncEntryCount(&p4->entries[511]);

    // Identity map individual pages
    for(uintptr_t addr = 0; addr < PADDR(ALIGN(kernel_end)); addr += 0x1000) {
        uint64_t p2e_idx = GetP2EntryIndex(addr);
        if(!(p2->entries[p2e_idx] & PAGE_PRESENT)) {
            p1 = kernel_page_table_end++;
            p2->entries[p2e_idx] = PADDR(p1) | PAGE_PRESENT | PAGE_WRITE;
            memset(p1, 0, 0x1000);

            // Increase present P2 entry count in P3 entry
            IncEntryCount(&p3->entries[510]);
        }

        uint64_t p1e_idx = GetP1EntryIndex(addr);
        if(addr >= PADDR(ALIGN(kernel_start)) && addr < PADDR(ALIGN(kernel_data))) {
            // Kernel code is read-only and executable
            p1->entries[p1e_idx] = addr | PAGE_PRESENT | PAGE_GLOBAL;
        }
        else {
            // Data is writable and not executable
            p1->entries[p1e_idx] = addr | PAGE_PRESENT | PAGE_WRITE | PAGE_GLOBAL | PAGE_NOEXECUTE;
        }

        // Increase P1 present entry count in P2 entry
        IncEntryCount(&p2->entries[p2e_idx]);
    }

    // Load new page tables
    load_pml4(PADDR(p4));

    // Bottom of frame stack starts immediately after kernel
    frame_stack = (uintptr_t*)ALIGN(kernel_end);

    // Initialize memory map pop frame function
    mmap_entry = (MemMapEntry*)mmap;
    mmap_addr = ALIGN(mmap_entry->base);
    mmap_size = mmap_entry->size;

    // Add available memory map frames to stack
    uintptr_t addr;
    while(mmap_entry && (addr = PopMemMapFrame()) > 0) {
        PushStackFrame(addr);
    }
}

err_t
Allocate(void* addr, bool exec, bool write, bool user)
{
    int flags = 0;

    if(!exec) {
        flags |= PAGE_NOEXECUTE;
    }

    if(write) {
        flags |= PAGE_WRITE;
    }

    if(user) {
        flags |= PAGE_USER;
    }

    return MapPage((uintptr_t)addr, flags, PopStackFrame);
}

err_t
Free(void* addr)
{
    return UnmapPage((uintptr_t)addr, PushStackFrame);
}

static void
PushStackFrame(uintptr_t paddr)
{
    err_t err = MapPage((uintptr_t)frame_stack, PAGE_WRITE | PAGE_GLOBAL, PopMemMapFrame);
    if(err != OK) {
        return;
    }

    // Fetch physical address and move stack frame pointer up
    *frame_stack++ = paddr;
}

static uintptr_t
PopStackFrame(void)
{
    if(frame_stack > (uintptr_t*)ALIGN(kernel_end)) {
        // Move frame stack pointer down and fetch physical address
        return *--frame_stack;
    }

    // No available frames
    return 0;
}

static uintptr_t
PopMemMapFrame(void)
{
    while(mmap_entry && mmap_entry->type) {
        if(mmap_entry->type != MAP_AVAILABLE || mmap_size < 0x1000) {
            mmap_entry++;
            mmap_size = mmap_entry->size;
            continue;
        }

        uintptr_t addr = mmap_addr;
        mmap_addr += 0x1000; // Advance to next 4 KiB
        mmap_size -= 0x1000; // Reduce remaining region size by 4 KiB

        if(addr < PADDR(kernel_page_table_end)) {
            // Only claim frames above kernel and initial page tables
            continue;
        }

        return addr;
    }

    mmap_entry = nullptr;

    return 0;
}

static err_t
MapPage(uintptr_t vaddr, int flags, uintptr_t (*pop)(void))
{
    PageTable* p4 = GetP4Table(vaddr);
    PageTable* p3 = GetP3Table(vaddr);
    PageTable* p2 = GetP2Table(vaddr);
    PageTable* p1 = GetP1Table(vaddr);

    uint64_t p4e_idx = GetP4EntryIndex(vaddr);
    if(!(p4->entries[p4e_idx] & PAGE_PRESENT)) {
        // Add P3 table for requested address
        uintptr_t paddr = (*pop)();
        if(paddr == 0) {
            // No frames available
            return ENOMEM;
        }

        // Page size and global flags must be 0 for P4 entries
        p4->entries[p4e_idx] = paddr | PAGE_PRESENT | (flags & ~PAGE_SIZE & ~PAGE_GLOBAL);
        flush_page(p4);
        memset(p3, 0, 0x1000);
    }

    uint64_t p3e_idx = GetP3EntryIndex(vaddr);
    if(!(p3->entries[p3e_idx] & PAGE_PRESENT)) {
        // Add P2 table for requested address
        uintptr_t paddr = (*pop)();
        if(paddr == 0) {
            // No frames available
            return ENOMEM;
        }

        // Do not allow 1 GiB pages
        p3->entries[p3e_idx] = paddr | PAGE_PRESENT | (flags & ~PAGE_SIZE);
        flush_page(p3);
        memset(p2, 0, 0x1000);

        // Increase P3 present entry count in P4 entry
        IncEntryCount(&p4->entries[p4e_idx]);
    }

    uint64_t p2e_idx = GetP2EntryIndex(vaddr);
    if(!(p2->entries[p2e_idx] & PAGE_PRESENT)) {
        // Add P1 table for requested address
        uintptr_t paddr = (*pop)();
        if(paddr == 0) {
            // No frames available
            return ENOMEM;
        }

        // Do not allow 2 MiB pages (this may change in the future)
        p2->entries[p2e_idx] = paddr | PAGE_PRESENT | (flags & ~PAGE_SIZE);
        flush_page(p2);
        memset(p1, 0, 0x1000);

        // Increase P2 present entry count in P3 entry
        IncEntryCount(&p3->entries[p3e_idx]);
    }

    uint64_t p1e_idx = GetP1EntryIndex(vaddr);
    if(!(p1->entries[p1e_idx] & PAGE_PRESENT)) {
        // Assign frame to page for requested address
        uintptr_t paddr = (*pop)();
        if(paddr == 0) {
            // No frames available
            return ENOMEM;
        }

        p1->entries[p1e_idx] = paddr | PAGE_PRESENT | flags;
        flush_page(p1);

        // Increase P1 present entry count in P2 entry
        IncEntryCount(&p2->entries[p2e_idx]);
    }

    return OK;
}

static err_t
UnmapPage(uintptr_t vaddr, void (*push)(uintptr_t))
{
    // Unmap page
    PageTable* p1 = GetP1Table(vaddr);
    uint64_t p1e_idx = GetP1EntryIndex(vaddr);
    if(!(p1->entries[p1e_idx] & PAGE_PRESENT)) {
        // P1 entry is not present
        return OK;
    }

    // Get physical address from P1 entry
    uintptr_t paddr = p1->entries[p1e_idx] & 0x7FFFF000;

    // Update P1 table
    p1->entries[p1e_idx] = ~PAGE_PRESENT;
    flush_page(p1);

    // Put frame back on stack
    (*push)(paddr);

    // Unmap P1 table if no more entries present
    PageTable* p2 = GetP2Table(vaddr);
    uint64_t p2e_idx = GetP2EntryIndex(vaddr);

    // Decrease P1 present entry count in P2 entry
    DecEntryCount(&p2->entries[p2e_idx]);

    if(HasEntries(p2->entries[p2e_idx])) {
        // There are still P1 entries present
        return OK;
    }

    // Get physical address from P2 entry
    paddr = p2->entries[p2e_idx] & 0x7FFFF000;

    // Update P2 table
    p2->entries[p2e_idx] = ~PAGE_PRESENT;
    flush_page(p2);

    // Put frame back on stack
    (*push)(paddr);

    // Unmap P2 table if no more entries present
    PageTable* p3 = GetP3Table(vaddr);
    uint64_t p3e_idx = GetP3EntryIndex(vaddr);

    // Decrease P2 present entry count in P3 entry
    DecEntryCount(&p3->entries[p3e_idx]);

    if(HasEntries(p3->entries[p3e_idx])) {
        // There are still P2 entries present
        return OK;
    }

    // Get physical address from P3 entry
    paddr = p3->entries[p3e_idx] & 0x7FFFF000;

    // Update P3 table
    p3->entries[p3e_idx] = ~PAGE_PRESENT;
    flush_page(p3);

    // Put frame back on stack
    (*push)(paddr);

    // Unmap P3 table if no more entries present
    PageTable* p4 = GetP4Table(vaddr);
    uint64_t p4e_idx = GetP4EntryIndex(vaddr);

    // Decrease P3 present entry count in P4 entry
    DecEntryCount(&p4->entries[p4e_idx]);

    if(HasEntries(p4->entries[p4e_idx])) {
        // There are still P2 entries present
        return OK;
    }

    // Get physical address from P3 entry
    paddr = p4->entries[p4e_idx] & 0x7FFFF000;

    // Update P3 table
    p4->entries[p4e_idx] = ~PAGE_PRESENT;
    flush_page(p4);

    // Put frame back on stack
    (*push)(paddr);

    return OK;
}

void
PageFaultHandler(Registers* regs)
{
}

static PageTable*
GetP4Table(uintptr_t vaddr)
{
    // Keep sign extend
    uint64_t p4 = vaddr & 0xFFFF000000000000;

    // Reference P4 table recursively
    p4 |= 510ull << 39;
    p4 |= 510ull << 30;
    p4 |= 510ull << 21;
    p4 |= 510ull << 12;

    return (PageTable*)p4;
}

static PageTable*
GetP3Table(uintptr_t vaddr)
{
    // Keep sign extend
    uint64_t p3 = vaddr & 0xFFFF000000000000;

    // Reference P4 table recursively
    p3 |= 510ull << 39;
    p3 |= 510ull << 30;
    p3 |= 510ull << 21;

    // Get index of P3 table
    p3 |= vaddr >> 27 & 0x1FF000;

    return (PageTable*)p3;
}

static PageTable*
GetP2Table(uintptr_t vaddr)
{
    // Keep sign extend
    uint64_t p3 = vaddr & 0xFFFF000000000000;

    // Reference P4 table recursively
    p3 |= 510ull << 39;
    p3 |= 510ull << 30;

    // Get index of P3 and P2 tables
    p3 |= vaddr >> 18 & 0x3FFFF000;

    return (PageTable*)p3;
}

static PageTable*
GetP1Table(uintptr_t vaddr)
{
    // Keep sign extend
    uint64_t p3 = vaddr & 0xFFFF000000000000;

    // Reference P4 table recursively
    p3 |= 510ull << 39;

    // Get index of P3, P2, and P1 tables
    p3 |= vaddr >> 9 & 0x7FFFFFF000;

    return (PageTable*)p3;
}

static inline uint64_t
GetP4EntryIndex(uintptr_t vaddr)
{
    return (uint64_t)(vaddr >> 39 & 0x1FF);
}

static inline uint64_t
GetP3EntryIndex(uintptr_t vaddr)
{
    return (uint64_t)(vaddr >> 30 & 0x1FF);
}

static inline uint64_t
GetP2EntryIndex(uintptr_t vaddr)
{
    return (uint64_t)(vaddr >> 21 & 0x1FF);
}

static inline uint64_t
GetP1EntryIndex(uintptr_t vaddr)
{
    return (uint64_t)(vaddr >> 12 & 0x1FF);
}

static inline void
IncEntryCount(uint64_t* entry)
{
    uint64_t count = (*entry >> 52 & 0x1FF) + 1;

    // Zero out current count;
    *entry &= ~(511ull << 52);

    // Add new count
    *entry |= count << 52;
}

static inline void
DecEntryCount(uint64_t* entry)
{
    uint64_t count = (*entry >> 52 & 0x1FF) - 1;

    // Zero out current count;
    *entry &= ~(511ull << 52);

    // Add new count
    *entry |= count << 52;
}

static inline bool
HasEntries(uint64_t entry)
{
    return (entry & (511ull << 52)) != 0;
}

} // Memory
} // BrewOS
