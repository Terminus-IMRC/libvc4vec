#include <stdio.h>
#include <stdlib.h>
#include "error.h"

#include "vc4vec.h"
#include "vc4vec_local.h"
#include "mem_node.h"

#include "mailbox/mailbox.h"
#include "mailbox/xmailbox.h"

#define GPU_MEM_FLG (1 << 2)

static const int align = 4096;

void mem_allocated_node_init()
{
	static _Bool is_called = 0;

	if (is_called)
		return;
	is_called = !0;

	vc4vec_local_init();
}

void mem_allocated_node_finalize()
{
	static _Bool is_called = 0;

	if (is_called)
		return;
	is_called = !0;

	vc4vec_local_finalize();
}

struct mem_allocated_node* mem_allocated_node_alloc(size_t size)
{
	struct mem_allocated_node *p;

	p = malloc(sizeof(struct mem_allocated_node));
	if (p == NULL) {
		error("failed to malloc a node\n");
		exit(EXIT_FAILURE);
	}

	p->size = size;
	p->handle = xmem_alloc(mb, size, align, GPU_MEM_FLG);
	p->gpu_addr = xmem_lock(mb, p->handle);
	p->cpu_addr = mapmem_cpu(p->gpu_addr, size);
	p->next = NULL;

	return p;
}

void mem_allocated_node_free(struct mem_allocated_node *p)
{
	if (p == NULL) {
		error("NULL is specified\n");
		exit(EXIT_FAILURE);
	}

	unmapmem_cpu(p->cpu_addr, p->size);
	xmem_unlock(mb, p->gpu_addr);
	xmem_free(mb, p->handle);

	free(p);
}
