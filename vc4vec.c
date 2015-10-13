#include <stdlib.h>

#include "names_inc.h"

#include "error.h"

static signed called = 0;

void vc4vec_init()
{
	called ++;
	if (called != 1)
		return;

#define X(x) vc4vec_called.x = 0;
#include "xnames.h"
#undef X

#define X(x) xCCAT(x, _init());
#include "xnames.h"
#undef X

	atexit(vc4vec_finalize);
}

void vc4vec_finalize()
{
	called --;
	if (called != 0)
		return;

#define X(x) xCCAT(x, _finalize());
#include "xnames.h"
#undef X

#define X(x) do { \
	if (vc4vec_called.x != 0) { \
		error("non-zero called.%s: %d\n", STR(x), vc4vec_called.x); \
		exit(EXIT_FAILURE); \
	} \
} while (0);
#include "xnames.h"
#undef X
}
