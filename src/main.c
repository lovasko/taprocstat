#include <stdint.h>
#include <stdio.h>
#include <tak.h>

struct my_proc;

struct my_list_entry {
	struct my_proc* le_next;
};

struct my_proc {
	intmax_t p_pid;	
	intmax_t p_numthreads;
	struct my_list_entry p_list;
};

struct my_proclist {
	struct my_proc* lh_first;
};

int
main(void)
{
	struct tak t_conn;
	int retval;
	struct my_proclist* proclist;
	struct my_proc* runner;

	retval = tak_open(&t_conn, TAK_TYPE_INFO_CTF, TAK_DATA_SOURCE_KVM);
	if (retval != TAK_OK) {
		fprintf(stderr, "ERROR: %s\n", tak_error_string(retval));
		return EXIT_FAILURE;
	}

	proclist = malloc(sizeof(struct my_proclist));
	retval = tak_map_sym(&t_conn,
	                     "struct my_proclist",
	                     "allproc",
	                     (void**)&proclist);
	if (retval != TAK_OK) {
		fprintf(stderr, "ERROR: %s\n", tak_error_string(retval));
		return EXIT_FAILURE;
	}

	printf("\n------\n");
	printf("proclist.lh_first = %p\n", proclist->lh_first);
	for (runner = proclist->lh_first;
	     runner != NULL;
	     runner = runner->p_list.le_next) {
		printf("%lld %lld\n", runner->p_pid, runner->p_numthreads);
	}

	return EXIT_SUCCESS;
}

