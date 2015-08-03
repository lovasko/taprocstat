#include <string.h>

#include "getter/getter.h"
#include "types.h"

char*
get_command(struct ps_proc* proc)
{
	char comm[20];
	unsigned int i;

	memset(comm, '\0', 20);
	for (i = 0; i < 20; i++)
		comm[i] = (char)(proc->p_comm[i]);

	return strdup(comm);
}

