#ifndef STATUS_H
#define STATUS_H

#include <sys/types.h>

#define STATUS_OKAY 0

#define ERROR_BAD_ADDRESS   -1
#define ERROR_OUT_OF_MEMORY -2

typedef int64_t status_t;

#endif // STATUS_H
