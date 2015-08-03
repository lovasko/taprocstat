#include <its.h>

#include "getter/getter.h"
#include "types.h"

char*
get_ppid(struct ps_proc* proc)
{
	if (proc->p_pptr != NULL)
		return its(&proc->p_pptr->p_pid,
			         ITS_SIZE_INTMAX,
		           ITS_SIGNED,
		           ITS_BASE_DEC);
	else
		return "-";
}

