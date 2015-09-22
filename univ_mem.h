#ifndef _UNIV_MEM_H_
#define _UNIV_MEM_H_

#include "vc4vec_mem.h"

	typedef unsigned int univ_mem_id_t;

	extern struct vc4vec_mem *univ_mem;
	extern int *univ_mem_size;

	void univ_mem_init();
	void univ_mem_finalize();

	void univ_mem_set_size(const univ_mem_id_t id, const int size);
	void univ_mem_free(const univ_mem_id_t id);

#endif /* _UNIV_MEM_H_ */
