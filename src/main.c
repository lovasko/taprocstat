#include <sys/types.h>

#include <stdint.h>
#include <stdio.h>
#include <tak.h>
#include <pwd.h>
#include <tabl.h>
#include <its.h>
#include <ctype.h>

#include "types.h"

int strcmpbynum(const char *s1, const char *s2) {
  for (;;) {
    if (*s2 == '\0')
      return *s1 != '\0';
    else if (*s1 == '\0')
      return 1;
    else if (!(isdigit(*s1) && isdigit(*s2))) {
      if (*s1 != *s2)
        return (int)*s1 - (int)*s2;
      else
        (++s1, ++s2);
    } else {
      char *lim1, *lim2;
      unsigned long n1 = strtoul(s1, &lim1, 10);
      unsigned long n2 = strtoul(s2, &lim2, 10);
      if (n1 > n2)
        return 1;
      else if (n1 < n2)
        return -1;
      s1 = lim1;
      s2 = lim2;
    }
  }
}

int
main(void)
{
	struct tak t_conn;
	int retval;
	struct ps_proclist* proclist;
	struct ps_proc* runner;
	struct ps_timeval* boottime;
	struct passwd* pwd;
	struct tabl table;
	struct m_list values;
	char* pid_str;
	char* numthreads_str;
	char comm[20];
	unsigned int i;

	m_list_init(&values);
	tabl_init(&table, 0);
	tabl_add_column(&table, "PID", NULL, TABL_ALIGN_RIGHT);
	tabl_add_column(&table, "THR", NULL, TABL_ALIGN_RIGHT);
	tabl_add_column(&table, "LOGIN", NULL, TABL_ALIGN_LEFT);
	tabl_add_column(&table, "COMM", NULL, TABL_ALIGN_LEFT);

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

	boottime = malloc(sizeof(struct ps_timeval));
	retval = tak_map_sym(&t_conn,
	                     "struct ps_timeval",
	                     "boottime",
	                     (void**)&boottime);
	if (retval != TAK_OK) {
		fprintf(stderr, "ERROR: %s\n", tak_error_string(retval));
		return EXIT_FAILURE;
	}

	printf("\n------\n");
	printf("proclist.lh_first = %p\n", proclist->lh_first);
	for (runner = proclist->lh_first;
	     runner != NULL;
	     runner = runner->p_list.le_next) {
		pwd = getpwuid((uid_t)runner->p_ucred->cr_uid);
		/* printf("%lld %lld %s %lld.%lld %lld.%lld %lld.%lld\n", */
		/* 	runner->p_pid, */
		/* 	runner->p_numthreads, */
		/* 	pwd->pw_name, */

		/* 	runner->p_ru.ru_stime.tv_sec, */
		/* 	runner->p_ru.ru_stime.tv_usec, */

		/* 	runner->p_ru.ru_utime.tv_sec, */
		/* 	runner->p_ru.ru_utime.tv_usec, */

		/* 	boottime->tv_sec + runner->p_stats->p_start.tv_sec, */
		/* 	boottime->tv_usec + runner->p_stats->p_start.tv_usec); */

		m_list_clear(&values);
		pid_str = its(&runner->p_pid, ITS_SIZE_INTMAX, ITS_SIGNED, ITS_BASE_DEC);
		numthreads_str = its(&runner->p_numthreads, ITS_SIZE_INTMAX, ITS_SIGNED, ITS_BASE_DEC);

		memset(comm, '\0', 20);
		for (i = 0; i < 20; i++)
			comm[i] = (char)(runner->p_comm[i]);
		
		m_list_append(&values, M_LIST_COPY_SHALLOW, pid_str, 1);
		m_list_append(&values, M_LIST_COPY_SHALLOW, numthreads_str, 1);
		m_list_append(&values, M_LIST_COPY_DEEP, pwd->pw_name, strlen(pwd->pw_name)+1);
		m_list_append(&values, M_LIST_COPY_SHALLOW, strdup(comm), 1);
		tabl_add_row(&table, &values);
	}

	tabl_sort(&table, 0, strcmpbynum);
	tabl_render(&table, NULL); 

	return EXIT_SUCCESS;
}

