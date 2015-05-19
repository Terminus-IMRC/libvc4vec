#ifndef _VC4VEC_LOCAL_H_
#define _VC4VEC_LOCAL_H_

	void vc4vec_local_init();
	void vc4vec_local_finalize();

	extern int mb;
	extern unsigned *v3d_p;

	struct vc4vec_called {
		signed local; /* vc4vec_local */
		signed mem; /* vc4vec_mem */
		signed mem_node; /* mem_allocated_node */
		signed job_launcher; /* qpu_job_launcher */
		signed vi_add_vi_256; /* vi_add_vi_256 */
		signed vi_add_ci_256; /* vi_add_ci_256 */
	};

	extern struct vc4vec_called vc4vec_called;

#endif /* _VC4VEC_LOCAL_H_ */
