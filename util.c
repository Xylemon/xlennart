#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void
fatal(const char *format, ...) {
	va_list args;

	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
	fprintf(stderr, "\n");
	exit(1);
}

void *
xalloc(int size) {
	void *p = malloc(size);
	if (p == NULL)
		fatal("out of memory");
	return p;
}
