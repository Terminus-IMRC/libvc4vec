#include <stdlib.h>

#include "vc4vec.h"
#include "vc4vec_local.h"
#include "vc4vec_mem.h"
#include "qpu_job_launcher.h"

void vc4vec_init()
{
	static _Bool is_called = 0;

	if (is_called)
		return;
	is_called = !0;

	vc4vec_called.local = 0;
	vc4vec_called.mem = 0;
	vc4vec_called.mem_node = 0;
	vc4vec_called.job_launcher = 0;
	vc4vec_called.vi_inc_256 = 0;

	vc4vec_local_init();
	vc4vec_mem_init();
	qpu_job_launcher_init();

	atexit(vc4vec_finalize);
}

void vc4vec_finalize()
{
	static _Bool is_called = 0;

	if (is_called)
		return;
	is_called = !0;

	vc4vec_local_finalize();
	vc4vec_mem_finalize();
	qpu_job_launcher_finalize();
}
