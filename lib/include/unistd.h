#ifndef UNISTD_H
#define UNISTD_H

#include <sys/cdefs.h>
#include <sys/stat.h>
#include <sys/types.h>

__BEGIN_DECLS

int chdir(const char *path);

int close(int fd);

int execv(const char *path, char *const argv[]);

int execve(const char *path, char *const argv[], char *const envp[]);

int execvp(const char *file, char *const argv[]);

int execvpe(const char *file, char *const argv[], char *const envp[]);

int fork(void);

char *getcwd(char *buf, size_t size);

pid_t getpid(void);

pid_t getppid(void);

off_t lseek(int fd, off_t offset, int whence);

ssize_t read(int fd, void *buf, size_t cnt);

void *sbrk(intptr_t incr);

unsigned sleep(unsigned seconds);

long int tell(int fd);

ssize_t write(int fd, const void *buf, size_t size);

__END_DECLS

#endif // UNISTD_H
