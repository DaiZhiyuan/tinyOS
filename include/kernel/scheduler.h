#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>

/*
 * Structure that contains values of all registers that might be different
 * between the tasks, that are being switched.
 */
typedef struct {
	uint64_t x19; /* General Purpose 64-bit register X19 (Callee-saved) */
	uint64_t x20; /* General Purpose 64-bit register X20 (Callee-saved) */
	uint64_t x21; /* General Purpose 64-bit register X21 (Callee-saved) */
	uint64_t x22; /* General Purpose 64-bit register X22 (Callee-saved) */
	uint64_t x23; /* General Purpose 64-bit register X23 (Callee-saved) */
	uint64_t x24; /* General Purpose 64-bit register X24 (Callee-saved) */
	uint64_t x25; /* General Purpose 64-bit register X25 (Callee-saved) */
	uint64_t x26; /* General Purpose 64-bit register X26 (Callee-saved) */
	uint64_t x27; /* General Purpose 64-bit register X27 (Callee-saved) */
	uint64_t x28; /* General Purpose 64-bit register X28 (Callee-saved) */
	uint64_t fp;  /* Fram Pointer: 64-bit register X29 (Callee-saved) */
	uint64_t sp;  /* Stack Pointer */
	uint64_t pc;  /* Program Counter */
} cpu_context;

/*
 * Structure that describes a process.
 */
typedef struct {
	cpu_context cpu_context; /* A struct for the registers to be saved for the process */
	int64_t state; /* The state of the currently running task */
	int64_t counter; /* Used to determine how long the current task has been running */
	int64_t priority; /* When a new task is scheduled its priority is copied to counter */
	int64_t preempt_count; /* Non-zero value indicates critical task, that must not be interrupted. */
} task_struct;

/*
 * Task States
 */
#define TASK_RUNNING 0

/*
 * Maximum number of concurrent tasks
 */
#define NR_TASKS 8


/* Array that holds all the tasks */
extern task_struct *task[NR_TASKS];
/* Pointer that points to the currently executing task */
extern task_struct *current;
/* Number of currently running tasks */
extern int nr_tasks;

/*
 * Define the first task.
 */
#define FIRST_TASK task[0]
/*
 * Define the last task.
 */
#define LAST_TASK task[NR_TASKS-1]

/*
 * Defines Pagesize of each process.
 */
#define THREAD_SIZE 4096

/*
 * Defines the init task' that is run on kernel startup.
 */
#define INIT_TASK { /*  cpu_context */ {0,0,0,0,0,0,0,0,0,0,0,0,0}, /* state etc */ 0,0,1,0 }


/*
 * Starting point for each new task.
 */
void ret_from_fork();

/*
 * Performs the context switch, by changing the registers.
 */
extern void cpu_switch_to(task_struct *prev, task_struct *next);

/*
 * Disables preemption for current running process.
 */
extern void preempt_disable();
/*
 * Enables preemption for current running process.
 */
extern void preempt_enable();

/*
 * Tail function that enables preemption for current process.
 */
extern void schedule_tail();

/*
 * Function that switches from current to the next task.
 */
extern void switch_to(task_struct* next);

/*
 * Main scheduler function.
 */
extern void schedule();

/*
 * Calls _schedule() after a time interval.
 */
extern void timer_tick();

#endif
