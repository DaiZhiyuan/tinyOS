#ifndef FORK_H
#define FORK_H

#include <stdint.h>

/*
 * Function for creating a new process.
 */
int copy_process(uint64_t fn, uint64_t arg);

#endif
