#include <string.h>

#include "vc4vec.h"
#include "vc4vec_local.h"
#include "vc4vec_mem.h"
#include "univ_mem.h"
#include "qpu_job_launcher.h"

static const unsigned code[] = {
#include "vi_add_vi_256.qasm.bin.hex"
};

static univ_mem_id_t code_id, unif_id;
static struct vc4vec_mem code_mem, unif_mem;

static const int code_size = sizeof(code);
static const int unif_len = 1024;

void vi_add_vi_256_init()
{
	vc4vec_called.vi_add_vi_256++;
	if (vc4vec_called.vi_add_vi_256 != 1)
		return;

	univ_mem_init();

	code_id = univ_mem_str_to_id("vc4vec:vi_add_vi_256:code");
	univ_mem_set_size(code_id, code_size);
	univ_mem_get_addr_by_id(&code_mem, code_id);

	unif_id = univ_mem_str_to_id("vc4vec:unif");
	univ_mem_set_size(unif_id, unif_len * (32 / 8));
	univ_mem_get_addr_by_id(&unif_mem, unif_id);

	memcpy(code_mem.cpu_addr, code, code_size);

	qpu_job_launcher_init();
}

void vi_add_vi_256_finalize()
{
	vc4vec_called.vi_add_vi_256--;
	if (vc4vec_called.vi_add_vi_256 != 0)
		return;

	qpu_job_launcher_finalize();

	univ_mem_free(unif_id);
	univ_mem_free(code_id);

	univ_mem_finalize();
}

void vi_add_vi_256(unsigned vpmout_qpu, unsigned vpmin1_qpu, unsigned vpmin2_qpu, const int vec_nmemb)
{
	unsigned *p = unif_mem.cpu_addr;

	*p++ = vec_nmemb / 256;
	*p++ = vpmin1_qpu;
	*p++ = vpmin2_qpu;
	*p++ = vpmout_qpu;

	launch_qpu_job(unif_len > 1023 ? 1024 : unif_len, unif_mem.gpu_addr, code_mem.gpu_addr);
}
