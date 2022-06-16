#ifndef PROCESS_H
#define PROCESS_H

#include <stddef.h>

/*
 * Creates processes using @ref copy_process().
 */
void create_processes(size_t proc_num);


/*
 * Kills all running processes (except `init_task`).
 */
void kill_processes();

#endif
