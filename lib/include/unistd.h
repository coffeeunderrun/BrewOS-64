#ifndef UNISTD_H
#define UNISTD_H

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

int execv(const char *path, char *const argv[]);

int execve(const char *path, char *const argv[], char *const envp[]);

int execvp(const char *file, char *const argv[]);

int execvpe(const char *file, char *const argv[], char *const envp[]);

int fork(void);

pid_t getpid(void);

ssize_t write(int fildes, const void *buf, size_t size);

#ifdef __cplusplus
}
#endif

#endif // UNISTD_H
