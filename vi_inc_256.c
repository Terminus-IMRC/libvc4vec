#include <string.h>

#include "vc4vec.h"
#include "vc4vec_local.h"
#include "vc4vec_mem.h"
#include "qpu_job_launcher.h"

static const unsigned code[] = {
#include "vi_inc_256.qasm.bin.hex"
};

static struct vc4vec_mem code_mem, unif_mem;

static const int code_size = sizeof(code);
static const int unif_size = 1024;

void vi_inc_256_init()
{
	vc4vec_called.vi_inc_256++;
	if (vc4vec_called.vi_inc_256 != 1)
		return;

	vc4vec_mem_init();

	vc4vec_mem_alloc(&code_mem, code_size);
	vc4vec_mem_alloc(&unif_mem, unif_size);

	memcpy(code_mem.cpu_addr, code, code_size);

	qpu_job_launcher_init();
}

void vi_inc_256_finalize()
{
	vc4vec_called.vi_inc_256--;
	if (vc4vec_called.vi_inc_256 != 0)
		return;

	qpu_job_launcher_finalize();

	vc4vec_mem_free(&unif_mem);
	vc4vec_mem_free(&code_mem);

	vc4vec_mem_finalize();
}

void vi_inc_256(unsigned vpmout_qpu, unsigned vpmin_qpu, const int vec_nmemb)
{
	unsigned *p = unif_mem.cpu_addr;

	*p++ = vec_nmemb / 256;
	*p++ = vpmin_qpu;
	*p++ = vpmout_qpu;

	launch_qpu_job(unif_size > 1023 ? 1024 : unif_size, unif_mem.gpu_addr, code_mem.gpu_addr);
}
