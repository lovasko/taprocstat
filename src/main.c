#include <sys/types.h>

#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <inttypes.h>
#include <stdio.h>
#include <tak.h>

#include "mode/mode.h"
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

static void
usage(void)
{
	printf("procstat [-b | -s] [-a | pid]\n");
}

static int
get_mode(int option)
{
	if (option == 'b') return MODE_BINARY;
	if (option == 's') return MODE_SECURITY;

	return MODE_DEFAULT;
}

int
main(int argc, char* argv[])
{
	struct tak t_conn;
	int retval;
	struct ps_proclist* proclist;
	struct ps_proc* proc;
	struct m_list procs;
	int mode;
	int option;
	intmax_t pid;

	pid = -2;
	mode = MODE_DEFAULT;

	while ((option = getopt(argc, argv, "abs")) != -1) {
		switch (option)	{
			case 'b':
			case 's':
				if (mode == MODE_DEFAULT)
					mode = get_mode(option);
				else {
					fprintf(stderr, "ERROR: only one mode allowed.\n");
					usage();
					return EXIT_FAILURE;
				}
			break;

			case 'a':
				pid = -1;
			break;

			case '?':
				fprintf(stderr, "ERROR: invalid option '%c'\n", optopt);	
				usage();
			return EXIT_FAILURE;

			default: 
				fprintf(stderr, "ERROR: unknown error during option parsing\n");	
				usage();
			return EXIT_FAILURE;
		}
	}

	if (argc - optind != 1) {
		if (pid != -1) {
			usage();
			return EXIT_FAILURE;
		}
	} else {
		pid = (intmax_t)strtoumax(argv[optind], NULL, 10);
	}

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

	m_list_init(&procs);
	for (proc = proclist->lh_first;
	     proc != NULL;
	     proc = proc->p_list.le_next) {
		if (pid == -1 || pid == proc->p_pid)
			m_list_append(&procs, M_LIST_COPY_SHALLOW, proc, 1);
	}

	if (m_list_is_empty(&procs) == M_LIST_TRUE) {
		fprintf(stderr, "ERROR: no matching processes found\n");
		return EXIT_FAILURE;
	}

	if (mode == MODE_DEFAULT) mode_default(&procs);
	if (mode == MODE_BINARY) mode_binary(&procs);
	if (mode == MODE_SECURITY) mode_security(&procs);

	return EXIT_SUCCESS;
}

