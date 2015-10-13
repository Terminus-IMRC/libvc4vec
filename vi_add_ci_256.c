#include <string.h>

#include "vc4vec.h"
#include "vc4vec_local.h"
#include "vc4vec_mem.h"
#include "univ_mem.h"
#include "qpu_job_launcher.h"

#define PNAME vi_add_ci_256

static const unsigned code[] = {
#include CODE_FN_STR
};

static univ_mem_id_t code_id, unif_id;
static struct vc4vec_mem code_mem, unif_mem;

static const int code_size = sizeof(code);
static const int unif_len = 1024;

void INIT_FUNC_NAME()
{
	CALLED_VAR ++;
	if (CALLED_VAR != 1)
		return;

	univ_mem_init();

	code_id = univ_mem_str_to_id("vc4vec:" PNAME_STR ":code");
	univ_mem_set_size(code_id, code_size);
	univ_mem_get_addr_by_id(&code_mem, code_id);

	unif_id = univ_mem_str_to_id("vc4vec:unif");
	univ_mem_set_size(unif_id, unif_len * (32 / 8));
	univ_mem_get_addr_by_id(&unif_mem, unif_id);

	memcpy(code_mem.cpu_addr, code, code_size);

	qpu_job_launcher_init();
}

void FINALIZE_FUNC_NAME()
{
	CALLED_VAR --;
	if (CALLED_VAR != 0)
		return;

	qpu_job_launcher_finalize();

	univ_mem_free(unif_id);
	univ_mem_free(code_id);

	univ_mem_finalize();
}

void FUNC_NAME(unsigned vpmout_qpu, unsigned vpmin_qpu, const signed c, const int vec_nmemb)
{
	unsigned *p = unif_mem.cpu_addr;

	*p++ = vec_nmemb / 256;
	*p++ = vpmin_qpu;
	*p++ = vpmout_qpu;
	*p++ = c;

	launch_qpu_job(unif_len > 1023 ? 1024 : unif_len, unif_mem.gpu_addr, code_mem.gpu_addr);
}
