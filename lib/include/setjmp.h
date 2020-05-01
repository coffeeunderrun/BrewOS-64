#ifndef SETJMP_H
#define SETJMP_H

#include <sys/cdefs.h>
#include <sys/types.h>

typedef unsigned long long jmp_buf[8];

__BEGIN_DECLS

void
longjmp(jmp_buf env, int val) __attribute__((noreturn));

int
setjmp(jmp_buf env);

__END_DECLS

#endif // SETJMP_H
