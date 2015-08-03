#include <sys/types.h>

#include <string.h>
#include <pwd.h>

#include "getter/getter.h"
#include "types.h"

char*
get_username(struct ps_proc* proc)
{
	struct passwd* pwd;

	pwd = getpwuid((uid_t)proc->p_ucred->cr_uid);

	if (pwd != NULL)
		return strdup(pwd->pw_name);
	else
		return "-";
}

