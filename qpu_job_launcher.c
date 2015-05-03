#include <stdlib.h>
#include <stdint.h>
#include "error.h"

#include "vc4vec_local.h"
#include "qpu_job_launcher.h"

#include "v3d.h"

void qpu_job_launcher_init()
{
	vc4vec_called.job_launcher++;
	if (vc4vec_called.job_launcher != 1)
		return;

	vc4vec_local_init();
}

void qpu_job_launcher_finalize()
{
	vc4vec_called.job_launcher--;
	if (vc4vec_called.job_launcher != 0)
		return;

	vc4vec_local_finalize();
}

void launch_qpu_job(uint32_t unif_length, uint32_t unif_addr, uint32_t prog_addr)
{
	v3d_write(v3d_p, V3D_QPURQUL, unif_length);
	v3d_write(v3d_p, V3D_QPURQUA, unif_addr);
	v3d_write(v3d_p, V3D_QPURQPC, prog_addr);
}
