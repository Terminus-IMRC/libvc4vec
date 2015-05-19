#include <stdlib.h>

#include "vc4vec.h"
#include "vc4vec_local.h"
#include "vc4vec_mem.h"
#include "qpu_job_launcher.h"
#include "vi_add_vi_256.h"
#include "vi_add_ci_256.h"

#include "error.h"

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
	vc4vec_called.vi_add_vi_256 = 0;
	vc4vec_called.vi_add_ci_256 = 0;

	vc4vec_local_init();
	vc4vec_mem_init();
	qpu_job_launcher_init();
	vi_add_vi_256_init();
	vi_add_ci_256_init();

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
	vi_add_vi_256_finalize();
	vi_add_ci_256_finalize();

	if (vc4vec_called.local != 0) {
		error("non-zero called.local: %d\n", vc4vec_called.local);
		exit(EXIT_FAILURE);
	}
	if (vc4vec_called.mem != 0) {
		error("non-zero called.mem: %d\n", vc4vec_called.mem);
		exit(EXIT_FAILURE);
	}
	if (vc4vec_called.mem_node != 0) {
		error("non-zero called.mem_node: %d\n", vc4vec_called.mem_node);
		exit(EXIT_FAILURE);
	}
	if (vc4vec_called.job_launcher != 0) {
		error("non-zero called.job_launcher: %d\n", vc4vec_called.job_launcher);
		exit(EXIT_FAILURE);
	}
	if (vc4vec_called.vi_add_vi_256 != 0) {
		error("non-zero called.vi_add_vi_256: %d\n", vc4vec_called.vi_add_vi_256);
		exit(EXIT_FAILURE);
	}
	if (vc4vec_called.vi_add_ci_256 != 0) {
		error("non-zero called.vi_add_ci_256: %d\n", vc4vec_called.vi_add_ci_256);
		exit(EXIT_FAILURE);
	}
}
