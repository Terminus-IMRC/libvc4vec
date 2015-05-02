#ifndef _VC4VEC_H_
#define _VC4VEC_H_

	void vc4vec_init();
	void vc4vec_finalize();

	struct vc4vec_mem {
		size_t size;
		unsigned handle;
		unsigned gpu_addr;
		void *cpu_addr;
	};

	void vc4vec_mem_alloc(struct vc4vec_mem *mem, int size);
	void vc4vec_mem_free(struct vc4vec_mem *mem);

#endif /* _VC4VEC_H_ */
