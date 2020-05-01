#ifndef ASSERT_H
#define ASSERT_H

#include <sys/cdefs.h>
#include <sys/types.h>

#ifdef __cplusplus
#define __ASSERT_FUNCTION __PRETTY_FUNCTION__
#else
#define __ASSERT_FUNCTION __func__
#endif

#ifdef NDEBUG
#define assert(expr) ((void)0)
#else
#define assert(expr) ((void)0)
//#define assert(expr) ((expr) ? (void)0 : dprint(#expr, __FILE__, __LINE__, __ASSERT_FUNCTION))
#endif

__BEGIN_DECLS

extern void
dprint(const char* expr, const char* file, int line, const char* func) __attribute__((noreturn));

__END_DECLS

#endif // ASSERT_H
