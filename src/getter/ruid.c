#include <its.h>

#include "getter/getter.h"
#include "types.h"

char*
get_ruid(struct ps_proc* proc)
{
	return its(&proc->p_ucred->cr_ruid,
	           ITS_SIZE_INTMAX,
	           ITS_UNSIGNED,
	           ITS_BASE_DEC);
}

