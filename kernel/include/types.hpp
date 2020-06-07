#ifndef KERNEL_TYPES
#define KERNEL_TYPES

namespace BrewOS {

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

typedef void* Handle;

typedef struct Registers Registers;

} // BrewOS

#endif // KERNEL_TYPES
