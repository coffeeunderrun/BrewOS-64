#ifndef DLFCN_H
#define DLFCN_H

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

void *dlopen(const char *filename, int flags);

int dlclose(void *handle);

#ifdef __cplusplus
}
#endif

#endif // DLFCN_H
