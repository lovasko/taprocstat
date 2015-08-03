#include <m_list.h>
#include <tabl.h>

#include "getter/getter.h"
#include "mode/mode.h"
#include "types.h"

static void
row(void* _proc, void* _table)
{
	struct tabl* table;
	struct ps_proc* proc;
	struct m_list values;

	proc = _proc;
	table = _table;

	m_list_init(&values);
	m_list_append(&values, M_LIST_COPY_SHALLOW, get_pid(proc), 1);
	m_list_append(&values, M_LIST_COPY_SHALLOW, get_command(proc), 1);
	m_list_append(&values, M_LIST_COPY_SHALLOW, get_osrel(proc), 1);
	tabl_add_row(table, &values);
}

void
mode_binary(struct m_list* procs)
{
	struct tabl table;

	tabl_init(&table, 0);
	tabl_add_column(&table, "PID", NULL, TABL_ALIGN_RIGHT);
	tabl_add_column(&table, "COMM", NULL, TABL_ALIGN_LEFT);
	tabl_add_column(&table, "OSREL", NULL, TABL_ALIGN_RIGHT);

	m_list_map(procs, row, &table);

	/* tabl_sort(&table, 0, number_sort); */
	tabl_render(&table, NULL);
}

