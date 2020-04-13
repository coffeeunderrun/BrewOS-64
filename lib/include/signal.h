#ifndef SIGNAL_H
#define SIGNAL_H

typedef __SIG_ATOMIC_TYPE__ sig_atomic_t;

typedef void (*sig_handler_t)(int);

#ifdef __cplusplus
extern "C" {
#endif

int raise(int sig);

sig_handler_t signal(int sig, sig_handler_t handler);

#ifdef __cplusplus
}
#endif

#endif // SIGNAL_H
