#ifndef CONSOLE_H
#define CONSOLE_H

#include <stdint.h>

/*
 * Length of total prompt string for the console.
 */
#define PROMPT_LENGTH 11

/*
 * Available commands for the console.
 */
typedef enum {
	cmd_help,            /* Prints available commands to the console */
	cmd_create_task,     /* Creates `task_num` kernel processes. */
	cmd_run_task,        /* Runs the created kernel processes concurrently. */
	cmd_kill_task,       /* Kills all created kernel processes. */
	cmd_halt             /* Halts the system. */
} command;


/*
 * Creates the prompt for the minimal console, based on the device.
 */
char *console_init();

/*
 * Returns the correct command number, parsed from console input.
 */
int console_get_cmd(char *input);

/*
 * Main console function.
 */
void console();

/*
 * Help command
 */
void console_help();

#endif
