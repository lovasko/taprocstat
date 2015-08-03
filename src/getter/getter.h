#ifndef TA_PROCSTAT_GETTER_H
#define TA_PROCSTAT_GETTER_H

#include "types.h"

char* get_command(struct ps_proc* proc);
char* get_numthreads(struct ps_proc* proc);
char* get_osrel(struct ps_proc* proc);
char* get_pid(struct ps_proc* proc);
char* get_ppid(struct ps_proc* proc);
char* get_uid(struct ps_proc* proc);
char* get_username(struct ps_proc* proc);
char* get_ruid(struct ps_proc* proc);
char* get_svuid(struct ps_proc* proc);

#endif

