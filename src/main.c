#include <sys/types.h>

#include <stdlib.h>
#include <limits.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <tak.h>
#include <pwd.h>
#include <tabl.h>
#include <its.h>
#include <ctype.h>

#include "types.h"

static int
number_sort(void* a_str, void* b_str)
{
	uintmax_t a;
	uintmax_t b;

	a = strtoumax((char*)a_str, NULL, 10);
	b = strtoumax((char*)b_str, NULL, 10);

	if (a < b)
		return -1;
	
	if (a > b)
		return 1;

	return 0;
}

static char*
get_pid(struct ps_proc* proc)
{
	return its(&proc->p_pid, ITS_SIZE_INTMAX, ITS_SIGNED, ITS_BASE_DEC);
}

static char*
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

static char*
get_numthreads(struct ps_proc* proc)
{
		return its(&proc->p_numthreads,
		           ITS_SIZE_INTMAX,
		           ITS_SIGNED,
		           ITS_BASE_DEC);
}

static char*
get_command(struct ps_proc* proc)
{
	char comm[20];
	unsigned int i;

	memset(comm, '\0', 20);
	for (i = 0; i < 20; i++)
		comm[i] = (char)(proc->p_comm[i]);

	return strdup(comm);
}

static char*
get_username(struct ps_proc* proc)
{
	struct passwd* pwd;

	pwd = getpwuid((uid_t)proc->p_ucred->cr_uid);

	if (pwd != NULL)
		return strdup(pwd->pw_name);
	else
		return "-";
}

int
main(void)
{
	struct tak t_conn;
	int retval;
	struct ps_proclist* proclist;
	struct ps_proc* proc;
	struct tabl table;
	struct m_list values;

	retval = tak_open(&t_conn, TAK_TYPE_INFO_CTF, TAK_DATA_SOURCE_KVM);
	if (retval != TAK_OK) {
		fprintf(stderr, "ERROR: %s\n", tak_error_string(retval));
		return EXIT_FAILURE;
	}

	proclist = malloc(sizeof(struct ps_proclist));
	retval = tak_map_sym(&t_conn,
	                     "struct ps_proclist",
	                     "allproc",
	                     (void**)&proclist);
	if (retval != TAK_OK) {
		fprintf(stderr, "ERROR: %s\n", tak_error_string(retval));
		return EXIT_FAILURE;
	}

	m_list_init(&values);
	tabl_init(&table, 0);
	tabl_add_column(&table, "PID", NULL, TABL_ALIGN_RIGHT);
	tabl_add_column(&table, "PPID", NULL, TABL_ALIGN_RIGHT);
	tabl_add_column(&table, "THR", NULL, TABL_ALIGN_RIGHT);
	tabl_add_column(&table, "LOGIN", NULL, TABL_ALIGN_LEFT);
	tabl_add_column(&table, "COMM", NULL, TABL_ALIGN_LEFT);

	for (proc = proclist->lh_first;
	     proc != NULL;
	     proc = proc->p_list.le_next) {
		m_list_clear(&values);
		m_list_append(&values, M_LIST_COPY_SHALLOW, get_pid(proc), 1);
		m_list_append(&values, M_LIST_COPY_SHALLOW, get_ppid(proc), 1);
		m_list_append(&values, M_LIST_COPY_SHALLOW, get_numthreads(proc), 1);
		m_list_append(&values, M_LIST_COPY_SHALLOW, get_username(proc), 1);
		m_list_append(&values, M_LIST_COPY_SHALLOW, get_command(proc), 1);
		tabl_add_row(&table, &values);
	}

	tabl_sort(&table, 0, number_sort);
	tabl_render(&table, NULL); 

	return EXIT_SUCCESS;
}

