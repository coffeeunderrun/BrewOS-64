#include <kernel/memory.h>
#include <sys/mman.h>

void *mmap(void *addr, size_t len, int prot, int flags, int fildes, off_t off)
{
    if(!addr || len == 0)
    {
        return MAP_FAILED;
    }

    if(prot == PROT_NONE)
    {
        // Not currently supported
        return MAP_FAILED;
    }

    bool exec = prot & PROT_EXEC;
    bool rw = prot & PROT_WRITE;
    bool usr = false;

    // Decrease size by 4 KiB (0x1000)
    // Increase pointer by 4 KiB (8 x 0x200 = 0x1000)
    for(void *p = addr; len; len -= 0x1000, p += 0x200)
    {
        mem_alloc(p, exec, rw, usr);
    }

    return addr;
}
