#ifndef _VC4VEC_MEM_H_
#define _VC4VEC_MEM_H_

	void vc4vec_mem_init();
	void vc4vec_mem_finalize();

	void* vc4vec_mem_alloc(int size);
	void vc4vec_mem_free(void *cpu_addr);

#endif /* _VC4VEC_MEM_H_ */
