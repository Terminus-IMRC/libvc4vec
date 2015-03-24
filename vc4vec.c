#include <stdlib.h>

#include "vc4vec.h"
#include "vc4vec_local.h"
#include "vc4vec_mem.h"

void vc4vec_init()
{
	static _Bool is_called = 0;

	if (is_called)
		return;
	is_called = !0;

	vc4vec_mem_init();

	atexit(vc4vec_finalize);
}

void vc4vec_finalize()
{
	static _Bool is_called = 0;

	if (is_called)
		return;
	is_called = !0;

	vc4vec_mem_finalize();
}
