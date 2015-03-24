#include <stdlib.h>

#include "vc4vec.h"
#include "vc4vec_local.h"

void vc4vec_init()
{
	static _Bool is_called = 0;

	if (is_called)
		return;
	is_called = !0;

	atexit(vc4vec_finalize);
}

void vc4vec_finalize()
{
	static _Bool is_called = 0;

	if (is_called)
		return;
	is_called = !0;
}
