#ifndef DLFCN_H
#define DLFCN_H

#include <sys/cdefs.h>
#include <sys/types.h>

#define RTLD_LAZY 0x1         // Relocations are performed at an implementation-defined time
#define RTLD_NOW 0x2          // Relocations are performed when the object is loaded
#define RTLD_BINDING_MASK 0x3 // All symbols are available for relocation processing of other modules
#define RTLD_NOLOAD 0x4       // No symbols are made available for relocation processing by other modules

__BEGIN_DECLS

int
dlclose(void* handle);

char*
dlerror(void);

void*
dlopen(const char* path, int flag);

void*
dlsym(void* __restrict handle, const char* __restrict name);

__END_DECLS

#endif // DLFCN_H
