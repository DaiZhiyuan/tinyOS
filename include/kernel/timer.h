#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

/*
 * Initializes System Timer 1, with a time interval of msec milliseconds.
 */
void timer_init(uint32_t msec);


/*
 * Disables System Timer 1.
 */
void timer_stop();

/*
 * Exception handler for System Timer 1.
 */
void handle_timer_irq();

/*
 * Reads the 64-bit free running counter value that contains,
 * the number of ticks since the system's initialization.
 */
uint64_t timer_get_ticks();

/*
 * Sleeps for msec milliseconds, using the timers free running counter.
 */
void timer_msleep (uint32_t msec);

#endif
