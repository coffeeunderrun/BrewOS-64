#ifndef CDEFS_H
#define CDEFS_H

#ifdef __cplusplus

#define __BEGIN_DECLS extern "C" {
#define __END_DECLS }

#define __restrict

#else // !__cplusplus

#define __BEGIN_DECLS
#define __END_DECLS

#define __restrict restrict

#endif

#endif // CDEFS_H
