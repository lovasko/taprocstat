#include <its.h>

#include "getter/getter.h"
#include "types.h"

char*
get_pid(struct ps_proc* proc)
{
	return its(&proc->p_pid, ITS_SIZE_INTMAX, ITS_SIGNED, ITS_BASE_DEC);
}

