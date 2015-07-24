#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include "error.h"

#include "vc4vec_local.h"
#include "qpu_job_launcher.h"
#include "vc4vec_mem.h"

#include "v3d.h"
#include "mailbox.h"

static struct vc4vec_mem ml_control;
static const unsigned MAX_QPUS = 4 * 3;

void qpu_job_launcher_init()
{
	vc4vec_called.job_launcher++;
	if (vc4vec_called.job_launcher != 1)
		return;

	vc4vec_local_init();
	vc4vec_mem_init();

	vc4vec_mem_alloc(&ml_control, MAX_QPUS * 2 * (32 / 8));
}

void qpu_job_launcher_finalize()
{
	vc4vec_called.job_launcher--;
	if (vc4vec_called.job_launcher != 0)
		return;

	vc4vec_mem_free(&ml_control);

	vc4vec_mem_finalize();
	vc4vec_local_finalize();
}

void launch_qpu_job(uint32_t unif_length, uint32_t unif_addr, uint32_t prog_addr)
{
	launch_qpu_job_v3d(unif_length, unif_addr, prog_addr);
}

void launch_qpu_job_v3d(uint32_t unif_length, uint32_t unif_addr, uint32_t prog_addr)
{
	v3d_write(v3d_p, V3D_QPURQUL, unif_length);
	v3d_write(v3d_p, V3D_QPURQUA, unif_addr);
	v3d_write(v3d_p, V3D_QPURQPC, prog_addr);
}

void launch_qpu_job_mailbox(uint32_t num_qpus, uint32_t noflush, unsigned timeout, ...)
{
	unsigned i;
	uint32_t *p;
	va_list ap;

	if (num_qpus > MAX_QPUS) {
		error("too big num_qpus: %d (max:%d)\n", num_qpus, MAX_QPUS);
		exit(EXIT_FAILURE);
	}

	p = ml_control.cpu_addr;
	va_start(ap, timeout);
	for (i = 0; i < num_qpus; i ++) {
		p[i * 2 + 0] = va_arg(ap, uint32_t); /* unif addr for QPU i */
		p[i * 2 + 1] = va_arg(ap, uint32_t); /* prog addr for QPU i */
	}
	va_end(ap);

	execute_qpu(mb, num_qpus, ml_control.gpu_addr, noflush, timeout);
}
