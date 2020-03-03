#include <mem/paging.h>
#include <mem/pmm.h>
#include <stdint.h>

void main(void *mmap)
{
    paging_init();

    pmm_init(mmap);

    while(1);
}
