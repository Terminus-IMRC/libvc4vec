#include <stdlib.h>

#include "vc4vec.h"
#include "vc4vec_local.h"
#include "error.h"

#include "v3d.h"
#include "mailbox.h"

int mb;
unsigned *v3d_p;

struct vc4vec_called vc4vec_called;

void vc4vec_local_init()
{
	vc4vec_called.local++;
	if (vc4vec_called.local != 1)
		return;

	mb = xmbox_open();
	if (qpu_enable(mb, 1)) {
		error("failed to enable QPU\n");
		exit(EXIT_FAILURE);
	}
	v3d_init();
	v3d_p = mapmem_cpu(v3d_peripheral_addr(), V3D_LENGTH);
}

void vc4vec_local_finalize()
{
	vc4vec_called.local--;
	if (vc4vec_called.local != 0)
		return;

	unmapmem_cpu(v3d_p, V3D_LENGTH);
	v3d_finalize();
	if (qpu_enable(mb, 0)) {
		error("failed to disable QPU\n");
		exit(EXIT_FAILURE);
	}
	xmbox_close(mb);
}
