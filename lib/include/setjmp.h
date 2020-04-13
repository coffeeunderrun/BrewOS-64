#ifndef SETJMP_H
#define SETJMP_H

typedef unsigned long long jmp_buf[8];

#ifdef __cplusplus
extern "C" {
#endif

void longjmp(jmp_buf env, int val) __attribute__((noreturn));

int setjmp(jmp_buf env);

#ifdef __cplusplus
}
#endif

#endif // SETJMP_H
