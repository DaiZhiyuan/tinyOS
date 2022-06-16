#include <stdint.h>

#include <kernel/scheduler.h>
#include <kernel/mm.h>
#include <kernel/printk.h>

int copy_process(uint64_t fn, uint64_t arg)
{
	/*
	 * Preemption is disabled for the current task.
	 * We don't want to be rescheduled to a different task
	 * in the middle of the copy_process function.
	*/
	preempt_disable();
	/* Allocate pointer for the new task */
	task_struct *p;

	/* Allocate a page for the new task */
	p = (task_struct *) get_free_page();
	/* If there are no available pages, return with error */
	if (p == 0) {
		return 1;
	}

	/*
	 * p points to the starting address of the allocated page
	 * At the bottom of this page, we place the task_struct for the new task
	 * The rest of this page, will be used as the stack of the new task
	 */
	p->state = TASK_RUNNING;
	p->priority = current->priority; /* Based on current task priority */
	p->counter = p->priority;	 /* Based on current task priority */
	p->preempt_count = 1;		 /* Should not be rescheduled */

	/* Initialize the cpu_context of the new task
	* x19: The function to call for the new task
	* x20: The argument for the function of the new task
	* pc: Points to the starting point for each new task (in asm)
	* sp: The stack pointer is set to the top of the newly allocated memory page
	*/
	p->cpu_context.x19 = fn;
	p->cpu_context.x20 = arg;
	p->cpu_context.pc = (uint64_t) ret_from_fork;
	p->cpu_context.sp = (uint64_t) p + THREAD_SIZE;

	/* Add newly created task to the task array */
	int pid = nr_tasks++;
	task[pid] = p;

	/* Enable preemption for the current task (fork) */
	preempt_enable();

	return 0;
}
