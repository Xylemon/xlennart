#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>

#ifdef RAND
#undef RAND
#endif
#define RAND(lb, ub) (rand() % ((ub) - (lb) + 1) + (lb))

#ifdef MAX
#undef MAX
#endif
#define MAX(x, y) ((x) > (y) ? (x) : (y))

#ifdef MIN
#undef MIN
#endif
#define MIN(x, y) ((x) < (y) ? (x) : (y))

#ifdef UNUSED
#undef UNUSED
#endif
#define UNUSED(x) (void)(x)

void *xalloc(int size);
void fatal(const char *format, ...);

#endif
