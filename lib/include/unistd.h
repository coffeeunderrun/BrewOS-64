#ifndef UNISTD_H
#define UNISTD_H

#include <sys/stat.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

int chdir(const char *path);

int close(int filedes);

int execv(const char *path, char *const argv[]);

int execve(const char *path, char *const argv[], char *const envp[]);

int execvp(const char *file, char *const argv[]);

int execvpe(const char *file, char *const argv[], char *const envp[]);

int fork(void);

char* getcwd(char* buf, size_t size);

pid_t getpid(void);

pid_t getppid();

off_t lseek(int fd, off_t offset, int whence);

ssize_t read(int fd, void* buf, size_t count);

void* sbrk(intptr_t increment);

unsigned sleep(unsigned seconds);

long int tell(int filedes);

ssize_t write(int fildes, const void *buf, size_t size);

#ifdef __cplusplus
}
#endif

#endif // UNISTD_H
