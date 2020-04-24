#ifndef STRINGS_H
#define STRINGS_H

#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

int strcasecmp(const char *s1, const char *s2);

int strncasecmp(const char *s1, const char *s2, size_t cnt);

__END_DECLS

#endif // STRINGS_H
