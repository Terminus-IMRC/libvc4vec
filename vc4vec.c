#include <stdlib.h>

#include "vc4vec.h"
#include "vc4vec_local.h"
#include "vc4vec_mem.h"
#include "qpu_job_launcher.h"
#include "univ_mem.h"
#include "vi_add_vi_256.h"
#include "vi_add_ci_256.h"

#include "error.h"

static signed called = 0;

void vc4vec_init()
{
	called ++;
	if (called != 1)
		return;

	vc4vec_called.vc4vec_local = 0;
	vc4vec_called.vc4vec_mem = 0;
	vc4vec_called.mem_allocated_node = 0;
	vc4vec_called.qpu_job_launcher = 0;
	vc4vec_called.univ_mem = 0;
	vc4vec_called.vi_add_vi_256 = 0;
	vc4vec_called.vi_add_ci_256 = 0;

	vc4vec_local_init();
	vc4vec_mem_init();
	qpu_job_launcher_init();
	univ_mem_init();
	vi_add_vi_256_init();
	vi_add_ci_256_init();

	atexit(vc4vec_finalize);
}

void vc4vec_finalize()
{
	called --;
	if (called != 0)
		return;

	vc4vec_local_finalize();
	vc4vec_mem_finalize();
	qpu_job_launcher_finalize();
	univ_mem_finalize();
	vi_add_vi_256_finalize();
	vi_add_ci_256_finalize();

	if (vc4vec_called.vc4vec_local != 0) {
		error("non-zero called.vc4vec_local: %d\n", vc4vec_called.vc4vec_local);
		exit(EXIT_FAILURE);
	}
	if (vc4vec_called.vc4vec_mem != 0) {
		error("non-zero called.vc4vec_mem: %d\n", vc4vec_called.vc4vec_mem);
		exit(EXIT_FAILURE);
	}
	if (vc4vec_called.mem_allocated_node != 0) {
		error("non-zero called.mem_allocated_node: %d\n", vc4vec_called.mem_allocated_node);
		exit(EXIT_FAILURE);
	}
	if (vc4vec_called.qpu_job_launcher != 0) {
		error("non-zero called.qpu_job_launcher: %d\n", vc4vec_called.qpu_job_launcher);
		exit(EXIT_FAILURE);
	}
	if (vc4vec_called.univ_mem != 0) {
		error("non-zero called.univ_mem: %d\n", vc4vec_called.univ_mem);
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
