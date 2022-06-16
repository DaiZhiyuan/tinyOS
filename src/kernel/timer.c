#include <stdint.h>

#include <kernel/timer.h>
#include <kernel/mmio.h>
#include <kernel/printk.h>
#include <kernel/console.h>
#include <kernel/scheduler.h>

#include <peripherals/timer.h>

uint32_t interval; /* Time interval for System Timer */
uint32_t cur_val = 0; /* Current value of System Timer */

void timer_init(uint32_t msec)
{
	/* Set time interval to be msec milliseconds */
	interval = msec * TIMER_CLK_1000_HZ;

	/* Read current counter value */
	cur_val = mmio_read(TIMER_CLO);

	/* Increase it by an interval */
	cur_val += interval;

	/* Set this value to be compared for System Timer 1 Interrupt */
	mmio_write(TIMER_C1, cur_val);
}

void timer_stop()
{
	/* Set 0 to compare register, in order to stop timer interrupts */
	mmio_write(TIMER_C1, 0);
}


void handle_timer_irq()
{
	/* Update current counter value, for next interrupt at same interval */
	cur_val += interval;

	/* Set this value to be compared for System Timer 1 Interrupt */
	mmio_write(TIMER_C1, cur_val);

	/* Acknowledge the interrupt by writing 1 to the TIMER_CS register */
	uint32_t timer_cs = mmio_read(TIMER_CS);
	timer_cs |= TIMER_CS_M1;
	mmio_write(TIMER_CS, timer_cs);

	/* Use Timer 1 IRQ, for scheduler */
	timer_tick();
}

uint64_t timer_get_ticks()
{
	uint32_t counter_hi = mmio_read(TIMER_CHI);
	uint32_t counter_lo = mmio_read(TIMER_CLO);

	/* Check if high value didn't change, after setting it */
	while (counter_hi != mmio_read(TIMER_CHI)) {
		counter_hi = mmio_read(TIMER_CHI);
		counter_lo = mmio_read(TIMER_CLO);
	}

	/* Create the full 64-bit counter value */
	uint64_t counter = ((uint64_t) counter_hi << 32) | counter_lo;

	return counter;

}

void timer_msleep (uint32_t msec)
{
	/* Get counters current calue */
	uint64_t start = timer_get_ticks();

	/* Wait until msec have passed from start */
	while (timer_get_ticks() < start + (msec * 1000)) {
		;
	}
}
