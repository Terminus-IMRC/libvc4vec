#ifndef _VC4VEC_LOCAL_H_
#define _VC4VEC_LOCAL_H_

	void vc4vec_local_init();
	void vc4vec_local_finalize();

	extern int mb;
	extern unsigned *v3d_p;

	struct vc4vec_called {
		signed vc4vec_local; /* vc4vec_local */
		signed vc4vec_mem; /* vc4vec_mem */
		signed mem_allocated_node; /* mem_allocated_node */
		signed qpu_job_launcher; /* qpu_job_launcher */
		signed univ_mem; /* univ_mem */
		signed vi_add_vi_256; /* vi_add_vi_256 */
		signed vi_add_ci_256; /* vi_add_ci_256 */
	};

	extern struct vc4vec_called vc4vec_called;

#define STR(x) #x
#define xSTR(x) STR(x)
#define CCAT(x, y) x ## y
#define xCCAT(x, y) CCAT(x, y)

#define PNAME_STR xSTR(PNAME)

#define CODE_SUFFIX .qasm.bin.hex
#define CODE_FN PNAME CODE_SUFFIX
#define CODE_FN_STR xSTR(CODE_FN)

#define INIT_FUNC_NAME xCCAT(PNAME, _init)
#define FINALIZE_FUNC_NAME xCCAT(PNAME, _finalize)
#define FUNC_NAME PNAME
#define CALLED_VAR vc4vec_called.PNAME

#endif /* _VC4VEC_LOCAL_H_ */
