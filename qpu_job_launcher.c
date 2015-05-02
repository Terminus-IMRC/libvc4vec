#include <stdlib.h>
#include <stdint.h>
#include "error.h"

#include "vc4vec_local.h"
#include "qpu_job_launcher.h"

#include "v3d/v3d.h"

void qpu_job_launcher_init()
{
	static _Bool is_called = 0;

	if (is_called)
		return;
	is_called = !0;

	vc4vec_local_init();
}

void qpu_job_launcher_finalize()
{
	static _Bool is_called = 0;

	if (is_called)
		return;
	is_called = !0;

	vc4vec_local_finalize();
}

void launch_qpu_job(uint32_t unif_length, uint32_t unif_addr, uint32_t prog_addr)
{
	v3d_write(v3d_p, V3D_QPURQUL, unif_length);
	v3d_write(v3d_p, V3D_QPURQUA, unif_addr);
	v3d_write(v3d_p, V3D_QPURQPC, prog_addr);
}
