#define _BSD_SOURCE

#include <stdlib.h>
#include <string.h>

#include "vc4vec.h"
#include "vc4vec_local.h"
#include "vc4vec_mem.h"
#include "univ_mem.h"
#include "error.h"

#define PNAME univ_mem

struct vc4vec_mem *univ_mem = NULL;
static unsigned int univ_mem_len = 0;
int *univ_mem_size = NULL;
int *univ_mem_use_count = NULL;

struct id_str {
	univ_mem_id_t id;
	char *str;
	struct id_str *next;
} *id_str = NULL;

void INIT_FUNC_NAME()
{
	CALLED_VAR ++;
	if (CALLED_VAR != 1)
		return;

	vc4vec_mem_init();
}

void FINALIZE_FUNC_NAME()
{
	int i;
	struct id_str *p;

	CALLED_VAR --;
	if (CALLED_VAR != 0)
		return;

	free(univ_mem_use_count);
	free(univ_mem_size);
	univ_mem_use_count = NULL;
	univ_mem_size = NULL;

	for (i = 0; (unsigned int) i < univ_mem_len; i ++) {
		if (univ_mem[i].cpu_addr != NULL) {
			error("univ_mem[%d] is not freed\n", i);
			exit(EXIT_FAILURE);
		}
	}
	univ_mem_len = 0;
	free(univ_mem);
	univ_mem = NULL;

	for (p = id_str; p != NULL; ) {
		struct id_str *p_tmp = p->next;

		free(p->str);
		free(p);
		p = p_tmp;
	}
	id_str = NULL;

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

		univ_mem_use_count = realloc(univ_mem_use_count, univ_mem_len_new * sizeof(*univ_mem_use_count));
		if (univ_mem_use_count == NULL) {
			error("failed to realloc univ_mem_use_count\n");
			exit(EXIT_FAILURE);
		}

		for (i = univ_mem_len; i < univ_mem_len_new; i ++) {
			univ_mem[i].cpu_addr = NULL;
			univ_mem_size[i] = 0;
			univ_mem_use_count[i] = 0;
		}

		univ_mem_len = univ_mem_len_new;
	}

	univ_mem_use_count[id] ++;

	/* Note: We do not shrink the size */
	if (univ_mem_size[id] < size) {
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

	univ_mem_use_count[id] --;

	if (univ_mem_use_count[id] < 0) {
		error("use_count[%d] is less than 0\n", id);
		exit(EXIT_FAILURE);
	} else if (univ_mem_use_count[id] != 0)
		return;

	vc4vec_mem_free(&univ_mem[id]);
	univ_mem[id].cpu_addr = NULL;
}

univ_mem_id_t univ_mem_str_to_id(const char *str)
{
	struct id_str *p, *p_prev;

	if (str == NULL) {
		error("specified str is NULL\n");
		exit(EXIT_FAILURE);
	}

	if (id_str == NULL) {
		id_str = malloc(sizeof(struct id_str));
		if (id_str == NULL) {
			error("failed to malloc id_str\n");
			exit(EXIT_FAILURE);
		}
		id_str->id = 0;
		id_str->str = strdup(str);
		id_str->next = NULL;

		return id_str->id;
	}

	for (p_prev = p, p = id_str; p != NULL; p_prev = p, p = p->next)
		if ((p->str != NULL) && !strcmp(p->str, str))
			return p->id;

	p = p_prev;
	p->next = malloc(sizeof(struct id_str));
	if (id_str == NULL) {
		error("failed to malloc id_str\n");
		exit(EXIT_FAILURE);
	}
	p->next->next = NULL;

	/* So do not use manually-defined ids if you use id_str system */
	p->next->id = p->id + 1;
	p->next->str = strdup(str);

	return p->next->id;
}

void univ_mem_get_addr_by_id(struct vc4vec_mem *p, const univ_mem_id_t id)
{
	if (id >= univ_mem_len) {
		error("attempted to copy non-allocated univ_mem\n");
		exit(EXIT_FAILURE);
	}

	memcpy(p, &univ_mem[id], sizeof(struct vc4vec_mem));
}
