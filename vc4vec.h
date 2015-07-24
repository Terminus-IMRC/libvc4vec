#ifndef _VC4VEC_H_
#define _VC4VEC_H_

#include <stdint.h>

	void vc4vec_init();
	void vc4vec_finalize();

	struct vc4vec_mem {
		unsigned handle;
		unsigned gpu_addr;
		void *cpu_addr;
	};

	void vc4vec_mem_alloc(struct vc4vec_mem *mem, int size);
	void vc4vec_mem_free(struct vc4vec_mem *mem);

	void launch_qpu_job(uint32_t unif_length, uint32_t unif_addr, uint32_t prog_addr);
	void launch_qpu_job_v3d(uint32_t unif_length, uint32_t unif_addr, uint32_t prog_addr);
	void launch_qpu_job_mailbox(uint32_t num_qpus, uint32_t noflush, unsigned timeout, ...);

	void vi_add_vi_256(unsigned vpmout_qpu, unsigned vpmin1_qpu, unsigned vpmin2_qpu, const int vec_nmemb);
	void vi_add_ci_256(unsigned vpmout_qpu, unsigned vpmin_qpu, const signed c, const int vec_nmemb);

#endif /* _VC4VEC_H_ */
