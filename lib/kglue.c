#include <memory.h>
#include <sys/mman.h>

void *mmap(void *p, size_t sz, int pr, int fl, int fd, off_t of)
{
    if(pr == PROT_NONE)
    {
        return MAP_FAILED;
    }

    if(sz == 0)
    {
        return MAP_FAILED;
    }

    bool ex = pr & PROT_EXEC;
    bool wr = pr & PROT_WRITE;

    for(uint64_t addr = (uint64_t)p; sz; sz -= 0x1000, addr += 0x1000)
    {
        kmalloc((void *)addr, ex, wr, false);
    }

    return p;
}
