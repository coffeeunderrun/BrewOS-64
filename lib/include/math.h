#ifndef MATH_H
#define MATH_H

#include <sys/cdefs.h>
#include <sys/types.h>

extern int signgam;

__BEGIN_DECLS

double acos(double x);

float acosf(float x);

double acosh(double x);

float acoshf(float x);

long double acoshl(long double x);

long double acosl(long double x);

double asin(double x);

float asinf(float x);

double asinh(double x);

float asinhf(float x);

long double asinhl(long double x);

long double asinl(long double x);

double atan(double x);

double atan2(double y, double x);

float atan2f(float y, float x);

long double atan2l(long double y, long double x);

float atanf(float x);

double atanh(double x);

float atanhf(float x);

long double atanhl(long double x);

long double atanl(long double x);

double cbrt(double x);

float cbrtf(float x);

long double cbrtl(long double x);

double ceil(double x);

float ceilf(float x);

long double ceill(long double x);

double copysign(double x, double y);

float copysignf(float x, float y);

long double copysignl(long double x, long double y);

double cos(double x);

float cosf(float x);

double cosh(double x);

float coshf(float x);

long double coshl(long double x);

long double cosl(long double x);

double erf(double x);

double erfc(double x);

float erfcf(float x);

long double erfcl(long double x);

float erff(float x);

long double erfl(long double x);

double exp(double x);

double exp2(double x);

float exp2f(float x);

long double exp2l(long double x);

float expf(float x);

long double expl(long double x);

double expm1(double x);

float expm1f(float x);

long double expm1l(long double x);

double fabs(double x);

float fabsf(float x);

long double fabsl(long double x);

double fdim(double x, double y);

float fdimf(float x, float y);

long double fdiml(long double x, long double y);

double floor(double x);

float floorf(float x);

long double floorl(long double x);

double fma(double x, double y, double z);

float fmaf(float x, float y, float z);

long double fmal(long double x, long double y, long double z);

double fmax(double x, double y);

float fmaxf(float x, float y);

long double fmaxl(long double x, long double y);

double fmin(double x, double y);

float fminf(float x, float y);

long double fminl(long double x, long double y);

double fmod(double numer, double denom);

float fmodf(float numer, float denom);

long double fmodl(long double numer, long double denom);

double frexp(double x, int *expnt);

float frexpf(float x, int *expnt);

long double frexpl(long double x, int *expnt);

double hypot(double x, double y);

float hypotf(float x, float y);

long double hypotl(long double x, long double y);

int ilogb(double x);

int ilogbf(float x);

int ilogbl(long double x);

double j0(double x);

double j1(double x);

double jn(int n, double x);

double ldexp(double x, int expnt);

float ldexpf(float x, int expnt);

long double ldexpl(long double x, int expnt);

double lgamma(double x);

float lgammaf(float x);

long double lgammal(long double x);

long long llrint(double x);

long long llrintf(float x);

long long llrintl(long double x);

long long llround(double x);

long long llroundf(float x);

long long llroundl(long double x);

double log(double x);

double log10(double x);

float log10f(float x);

long double log10l(long double x);

double log1p(double x);

float log1pf(float x);

long double log1pl(long double x);

double log2(double x);

float log2f(float x);

long double log2l(long double x);

double logb(double x);

float logbf(float x);

long double logbl(long double x);

float logf(float x);

long double logl(long double x);

long lrint(double x);

long lrintf(float x);

long lrintl(long double x);

long lround(double x);

long lroundf(float x);

long lroundl(long double x);

double modf(double x, double *intgr);

float modff(float x, float *intgr);

long double modfl(long double x, long double *intgr);

double nan(const char *tagp);

float nanf(const char *tagp);

long double nanl(const char *tagp);

double nearbyint(double x);

float nearbyintf(float x);

long double nearbyintl(long double x);

double nextafter(double x, double y);

float nextafterf(float x, float y);

long double nextafterl(long double x, long double y);

double nexttoward(double x, long double y);

float nexttowardf(float x, long double y);

long double nexttowardl(long double x, long double y);

double pow(double base, double expnt);

float powf(float base, float expnt);

long double powl(long double base, long double expnt);

double remainder(double numer, double denom);

float remainderf(float numer, float denom);

long double remainderl(long double numer, long double denom);

double remquo(double numer, double denom, int *quot);

float remquof(float numer, float denom, int *quot);

long double remquol(long double numer, long double denom, int *quot);

double rint(double x);

float rintf(float x);

long double rintl(long double x);

double round(double x);

float roundf(float x);

long double roundl(long double x);

double scalbln(double x, long n);

float scalblnf(float x, long n);

long double scalblnl(long double x, long n);

double scalbn(double x, int n);

float scalbnf(float x, int n);

long double scalbnl(long double x, int n);

double sin(double x);

float sinf(float x);

double sinh(double x);

float sinhf(float x);

long double sinhl(long double x);

long double sinl(long double x);

double sqrt(double x);

float sqrtf(float x);

long double sqrtl(long double x);

double tan(double x);

float tanf(float x);

double tanh(double x);

float tanhf(float x);

long double tanhl(long double x);

long double tanl(long double x);

double tgamma(double x);

float tgammaf(float x);

long double tgammal(long double x);

double trunc(double x);

float truncf(float x);

long double truncl(long double x);

double y0(double x);

double y1(double x);

double yn(int n, double x);

__END_DECLS

#endif // MATH_H
