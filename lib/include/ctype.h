#ifndef CTYPE_H
#define CTYPE_H

#include <locale.h>
#include <sys/cdefs.h>
#include <sys/types.h>

#define _tolower(chr) tolower(chr)

#define _toupper(chr) toupper(chr)

__BEGIN_DECLS

int
isalnum(int chr);

int
isalnum_l(int chr, locale_t locale);

int
isalpha(int chr);

int
isalpha_l(int chr, locale_t locale);

int
isblank(int chr);

int
isblank_l(int chr, locale_t locale);

int
iscntrl(int chr);

int
iscntrl_l(int chr, locale_t locale);

int
isdigit(int chr);

int
isdigit_l(int chr, locale_t locale);

int
isgraph(int chr);

int
isgraph_l(int chr, locale_t locale);

int
islower(int chr);

int
islower_l(int chr, locale_t locale);

int
isprint(int chr);

int
isprint_l(int chr, locale_t locale);

int
ispunct(int chr);

int
ispunct_l(int chr, locale_t locale);

int
isspace(int chr);

int
isspace_l(int chr, locale_t locale);

int
isupper(int chr);

int
isupper_l(int chr, locale_t locale);

int
isxdigit(int chr);

int
isxdigit_l(int chr, locale_t locale);

int
tolower(int chr);

int
tolower_l(int chr, locale_t locale);

int
toupper(int chr);

int
toupper_l(int chr, locale_t locale);

__END_DECLS

#endif // CTYPE_H
