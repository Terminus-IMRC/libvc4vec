#include <stdio.h>
#include <stdlib.h>
#include "error.h"

#include "vc4vec.h"
#include "vc4vec_local.h"
#include "vc4vec_mem.h"
#include "mem_node.h"

#define PNAME vc4vec_mem

static struct mem_allocated_node *orig = NULL;

static void mem_free_all();

void INIT_FUNC_NAME()
{
	CALLED_VAR ++;
	if (CALLED_VAR != 1)
		return;

	mem_allocated_node_init();
}

void FINALIZE_FUNC_NAME()
{
	CALLED_VAR --;
	if (CALLED_VAR != 0)
		return;

	mem_free_all();

	mem_allocated_node_finalize();
}

void vc4vec_mem_alloc(struct vc4vec_mem *mem, int size)
{
	struct mem_allocated_node *p;

	if (orig == NULL)
		p = orig = mem_allocated_node_alloc(size);
	else {
		struct mem_allocated_node *p2 = orig;
		while (p2->next != NULL)
			p2 = p2->next;
		p = p2->next = mem_allocated_node_alloc(size);
	}

	mem->handle = p->handle;
	mem->gpu_addr = p->gpu_addr;
	mem->cpu_addr = p->cpu_addr;
}

void vc4vec_mem_free(struct vc4vec_mem *mem)
{
	struct mem_allocated_node *p = orig, *prev = NULL;

	while (p != NULL) {
		if (p->handle == mem->handle) {
			if (prev == NULL) /* so p == orig */
				orig = orig->next;
			else
				prev->next = p->next;

			mem_allocated_node_free(p);

			return;
		}
		prev = p;
		p = p->next;
	}

	error("non-allocated struct\n");
	exit(EXIT_FAILURE);
}

static void mem_free_all()
{
	while (orig != NULL) {
		struct mem_allocated_node *next = orig->next;

		mem_allocated_node_free(orig);
		orig = next;
	}
}
