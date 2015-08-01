#include <stdint.h>

struct ps_proc;

struct ps_proc_list_entry {
	struct ps_proc* le_next;
};

struct ps_timeval {
	intmax_t tv_sec;
	intmax_t tv_usec;
};

struct ps_pstats {
	struct ps_timeval p_start;
};

struct ps_ucred {
	uintmax_t cr_uid;
};

struct ps_rusage {
	struct ps_timeval ru_utime;
	struct ps_timeval ru_stime;
};

struct ps_proc {
	intmax_t p_pid;
	intmax_t p_numthreads;
	intmax_t p_comm[20];
	struct ps_pstats* p_stats;
	struct ps_ucred* p_ucred;
	struct ps_rusage p_ru;
	struct ps_proc_list_entry p_list;
	struct ps_proc* p_pptr;
};

struct ps_proclist {
	struct ps_proc* lh_first;
};

