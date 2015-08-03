#ifndef TA_PROCSTAT_MODE_H
#define TA_PROCSTAT_MODE_H

#include <m_list.h>

#define MODE_DEFAULT  0
#define MODE_BINARY   1
#define MODE_SECURITY 2

void mode_default(struct m_list* procs);
void mode_binary(struct m_list* procs);
void mode_security(struct m_list* procs);

#endif

