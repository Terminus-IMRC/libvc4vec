#ifndef _VC4VEC_MEM_NODE_H_
#define _VC4VEC_MEM_NODE_H_

#include <stdlib.h>

#include "vc4vec.h"
#include "vc4vec_local.h"

	struct mem_allocated_node {
		size_t size;
		unsigned handle;
		unsigned gpu_addr;
		void *cpu_addr;
		struct mem_allocated_node *next;
	};

	void mem_allocated_node_init();
	void mem_allocated_node_finalize();

	struct mem_allocated_node* mem_allocated_node_alloc(size_t size);
	void mem_allocated_node_free(struct mem_allocated_node *p);

#endif /* _VC4VEC_MEM_NODE_H_ */
