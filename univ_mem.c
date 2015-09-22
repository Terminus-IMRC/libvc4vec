#include <stdlib.h>
#include <string.h>

#include "vc4vec.h"
#include "vc4vec_local.h"
#include "vc4vec_mem.h"
#include "univ_mem.h"
#include "error.h"

struct vc4vec_mem *univ_mem = NULL;
static unsigned int univ_mem_len = 0;
int *univ_mem_size = NULL;

void univ_mem_init()
{
	vc4vec_called.univ_mem ++;
	if (vc4vec_called.univ_mem != 1)
		return;

	vc4vec_mem_init();
}

void univ_mem_finalize()
{
	int i;

	vc4vec_called.univ_mem--;
	if (vc4vec_called.univ_mem != 0)
		return;

	for (i = 0; (unsigned int) i < univ_mem_len; i ++) {
		if (univ_mem[i].cpu_addr != NULL) {
			error("univ_mem[%d] is not freed\n", i);
			exit(EXIT_FAILURE);
		}
	}

	vc4vec_mem_finalize();
}

void univ_mem_set_size(const univ_mem_id_t id, const int size)
{
	int i;

	if (univ_mem_len <= id) {
		int univ_mem_len_new = id + 1;

		univ_mem = realloc(univ_mem, univ_mem_len_new * sizeof(*univ_mem));
		if (univ_mem == NULL) {
			error("failed to realloc univ_mem\n");
			exit(EXIT_FAILURE);
		}

		univ_mem_size = realloc(univ_mem_size, univ_mem_len_new * sizeof(*univ_mem_size));
		if (univ_mem_size == NULL) {
			error("failed to realloc univ_mem_size\n");
			exit(EXIT_FAILURE);
		}

		for (i = univ_mem_len; i < univ_mem_len_new; i ++) {
			univ_mem[i].cpu_addr = NULL;
			univ_mem_size[i] = 0;
		}

		univ_mem_len = univ_mem_len_new;
	}

	if (univ_mem_size[id] != size) {
		struct vc4vec_mem mem_new;

		vc4vec_mem_alloc(&mem_new, size);
		if (univ_mem[id].cpu_addr != NULL) {
			memcpy(mem_new.cpu_addr, univ_mem[id].cpu_addr, univ_mem_size[id] < size ? univ_mem_size[id] : size);
			univ_mem_free(id);
		}
		memcpy(&univ_mem[id], &mem_new, sizeof(univ_mem[id]));
		univ_mem_size[id] = size;
	}
}

void univ_mem_free(const univ_mem_id_t id)
{
	if (id >= univ_mem_len) {
		error("attempted to free non-allocated univ_mem\n");
		exit(EXIT_FAILURE);
	} else if (univ_mem[id].cpu_addr == NULL) {
		error("attempted to free non-allocated vc4vec_mem in univ_mem\n");
		exit(EXIT_FAILURE);
	}

	vc4vec_mem_free(&univ_mem[id]);
	univ_mem[id].cpu_addr = NULL;
}
