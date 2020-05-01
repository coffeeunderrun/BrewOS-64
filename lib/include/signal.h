#ifndef SIGNAL_H
#define SIGNAL_H

#include <sys/cdefs.h>
#include <sys/types.h>

typedef __SIG_ATOMIC_TYPE__ sig_atomic_t;

typedef void (*sig_handler_t)(int);

__BEGIN_DECLS

int
raise(int sig);

sig_handler_t
signal(int sig, sig_handler_t handler);

__END_DECLS
#endif // SIGNAL_H
