#include <stdio.h>
#include <stdlib.h>
#include "error.h"

#include "vc4vec.h"
#include "vc4vec_local.h"
#include "vc4vec_mem.h"
#include "mem_node.h"

static struct mem_allocated_node *orig = NULL;

static void mem_free_all();

void vc4vec_mem_init()
{
	static _Bool is_called = 0;

	if (is_called)
		return;
	is_called = !0;

	mem_allocated_node_init();
}

void vc4vec_mem_finalize()
{
	static _Bool is_called = 0;

	if (is_called)
		return;
	is_called = !0;

	mem_free_all();

	mem_allocated_node_finalize();
}

void* vc4vec_mem_alloc(int size)
{
	struct mem_allocated_node *p;

	if (orig == NULL)
		p = orig = mem_allocated_node_alloc(size);
	else
		p = orig->next = mem_allocated_node_alloc(size);

	return p->cpu_addr;
}

void vc4vec_mem_free(void *cpu_addr)
{
	struct mem_allocated_node *p = orig, *prev = NULL;

	while (p != NULL) {
		if (p->cpu_addr == cpu_addr) {
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

	error("non-allocated address\n");
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
