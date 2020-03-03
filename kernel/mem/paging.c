#include <cpu.h>
#include <mem/paging.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define PRESENT     (1ull << 0)
#define WRITABLE    (1ull << 1)
#define USER_ACCESS (1ull << 2)
#define WRITE_THRU  (1ull << 3)
#define NO_CACHE    (1ull << 4)
#define HUGE        (1ull << 7)
#define GLOBAL      (1ull << 8)
#define NO_EXECUTE  (1ull << 63)

#define ALIGN(x) ((uint64_t)(x) & 0xFFFFFFFFFFFFF000)

#define LOWER_HALF(x) ((uint64_t)(x) & 0x7FFFFFFF)

extern const uint64_t kernel_start;
extern const uint64_t kernel_data;
extern const uint64_t kernel_end;

extern const uint64_t kernel_pml4;
extern const uint64_t kernel_pdp;
extern const uint64_t kernel_pd;
extern const uint64_t kernel_pt;

void paging_init()
{
    uint64_t *pml4 = (uint64_t *)&kernel_pml4;
    uint64_t *pdp = (uint64_t *)&kernel_pdp;
    uint64_t *pd = (uint64_t *)&kernel_pd;
    uint64_t *pt = (uint64_t *)&kernel_pt;

    memset(pml4, 0, 0x1000);
    memset(pdp, 0, 0x1000);
    memset(pd, 0, 0x1000);
    memset(pt, 0, 0x1000);

    // Last 512 GiB
    pml4[510] = LOWER_HALF(ALIGN(pml4)) | PRESENT | WRITABLE | GLOBAL;
    pml4[511] = LOWER_HALF(ALIGN(pdp)) | PRESENT | WRITABLE | GLOBAL;

    // Second to last 1 GiB
    pdp[510] = LOWER_HALF(ALIGN(pd)) | PRESENT | WRITABLE | GLOBAL;

    // First 2 MiB
    pd[0] = LOWER_HALF(ALIGN(pt)) | PRESENT | WRITABLE | GLOBAL;

    // Identity map individual pages within first 2 MiB
    for(uint64_t i = 0; i < 512; i++)
    {
        uint64_t address = i * 0x1000;
        uint64_t flags = PRESENT | GLOBAL;

        // Kernel code is read-only and executable, data is writable and not executable
        if(address < LOWER_HALF(&kernel_start) || address >= LOWER_HALF(&kernel_data))
        {
            flags |= WRITABLE | NO_EXECUTE;
        }

        pt[i] = address | flags;
    }

    write_cr3(LOWER_HALF(ALIGN(pml4)));
}
