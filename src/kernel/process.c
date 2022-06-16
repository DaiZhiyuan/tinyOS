#include <stdint.h>
#include <stddef.h>

#include <kernel/printk.h>
#include <kernel/mmio.h>
#include <kernel/timer.h>
#include <kernel/fork.h>
#include <kernel/scheduler.h>
#include <kernel/uart.h>
#include <kernel/mm.h>

#include <common/string.h>
#include <common/stdlib.h>

/* String array, for the message of each process */
static char task_args[NR_TASKS][MAX_INPUT_LENGTH];

/*
 * Dummy function that simulates a process.
 */
void process(char *array)
{
	size_t items = strlen(array);
	while (1) {
		for (size_t i = 0; i < items; i++) {
			printk("%c", array[i]);
			delay(100000);
		}
        printk("\n");
	}
}

void create_processes(size_t task_num)
{
	/* Get running tasks, for generating pid for each new task in the system */
	int pid = nr_tasks;
	int start_pid = nr_tasks;

	/* Create only a maximum of NR_TASKS */
	for (size_t i = 0; i < task_num; i++, pid++) {

		/* Check if processes >= Maximum Total Processes */
		if (pid >= NR_TASKS) {
			printk("Can't create %d process(es)\n", task_num - (pid - start_pid));
			printk("Total processes must be: 0 < tasks < %d \n", NR_TASKS);
			return;
		}

		printk("Forking process %d...", pid);

		strcpy(task_args[pid], "Task ");
        strcat(task_args[pid], itoa(i));
        strcat(task_args[pid], ", with pid = ");
        strcat(task_args[pid], itoa(pid));
        strcat(task_args[pid], ". ");

		int res = copy_process((uint64_t) &process, (uint64_t) task_args[pid]);
		if (res != 0) {
			printk("Error while starting process %d\n", pid);
			return;
		}
		printk("Done\n");
	}
}

void kill_processes()
{
	/*
	 * Preemption is disabled for the current task.
	 * We don't want to be rescheduled to a different task
	 * in the middle of killing another task.
	 */
	preempt_disable();

	/* Allocate pointer for the new task */
	task_struct *p;

	/*
	 * Iterate over all tasks and try to kill all runing ones.
	 */
	for (size_t i = 0; i < NR_TASKS; i++) {
		p = task[i];
		/* If it is an allocated task, and not the init task */
		if (p != 0 && i != 0) {
			/* Free allocated memory of task */
			free_page((uint64_t) p);

			/* Decrease number of processes */
			nr_tasks--;

			/* Remove task_struct from task array */
			task[i] = 0;

			printk("Killed task %d, located at %d\n", i, p);
		}
	}

}
