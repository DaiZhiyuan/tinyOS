#include <stdint.h>

#include <kernel/console.h>
#include <kernel/printk.h>
#include <kernel/uart.h>
#include <kernel/timer.h>
#include <kernel/scheduler.h>
#include <kernel/process.h>

#include <common/string.h>
#include <common/stdlib.h>

char *console_init(char *device)
{
	static char prompt[PROMPT_LENGTH];

	strcpy(prompt, "hpc@");
	strcat(prompt, device);
	strcat(prompt, "#");

	return prompt;
}

int console_get_cmd(char *input)
{
	if (strcmp(input, "help") == 0)
		return cmd_help;
	else if (strcmp(input, "create_task") == 0)
		return cmd_create_task;
	else if (strcmp(input, "run_task") == 0)
		return cmd_run_task;
	else if (strcmp(input, "kill_task") == 0)
		return cmd_kill_task;
	else if (strcmp(input, "halt") == 0)
		return cmd_halt;
	else
		return -1;
}

void console()
{
	char *input;
	char *args;
	char *prompt;
    int task_num;

	prompt = console_init("pi4");

	printk("\n");
	printk("This is a minimal console, type 'help' to see the available commands.\n");

	/* Main functionality */
	while (1) {
		printk("%s ", prompt);
		input = uart_gets();
		printk("\n");

		/* Find given command */
		command cmd = console_get_cmd(input);

		switch (cmd) {
		case cmd_help:
			console_help();
			break;
		case cmd_create_task:
			printk("Enter number of process(es): ");
			args = uart_gets();
			printk("\n");
			task_num = atoi(args);
			if (task_num <= 0 || task_num >= NR_TASKS) {
				printk("Not a valid number: %s\n", args);
				printk("Total processes must be: 0 < tasks < %d \n", NR_TASKS);
				break;
			}
			printk("Creating %d process(es)...\n", task_num);
			create_processes(task_num);
			printk("Done\n");
			break;
		case cmd_run_task:
			/* Initialize Timer for scheduler */
			printk("Initializing Timer ...");
			timer_init(2000);
			printk("Done\n");
			/* Schedule */
			printk("Entering in scheduling mode...\n");
			while(1) {
				/*
				* Core scheduler function.
				* Checks whether there is a new task,
				* that needs to preempt the current one.
				*/
				schedule();
				/* Continue or Stop, based on user input */
				timer_stop();
				printk("Continue? [y/n]: ");
				args = uart_gets();
				printk("\n");
				if (strcmp(args, "n") == 0)
					break;
				else {
					timer_init(2000);
					continue;
				}
			}
			/* Stop Timer from calling the scheduler */
			timer_stop();
			break;
		case cmd_kill_task:
			printk("Killing all process(es)...\n");
			kill_processes();
			printk("Done\n");
			break;
		case cmd_halt:
			printk("Halt.\n");
			printk("So long and thanks!\n");
			return;
		default:
			printk("%s: command not found\n", input);
			printk("type 'help' to see the available commands\n");
			break;
		}
	}

}

void console_help()
{
	printk("Available commands:\n");
	printk("    help:\n");
	printk("        Prints available commands to the console.\n");
	printk("    create_task:\n");
	printk("        Creates task_num kernel processes.\n");
	printk("    run_task:\n");
	printk("        Runs the created kernel processes concurrently.\n");
	printk("    kill_task:\n");
	printk("        Kills all created kernel processes.\n");
	printk("    halt:\n");
	printk("        Halts the system.\n");
}
