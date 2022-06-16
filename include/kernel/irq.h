/*
 * Code for handling exceptions/interupts.
 */

#ifndef IRQ_H
#define IRQ_H

/*
 * Enables System Timer and UART interrupts.
 */
void enable_interrupt_controller();

/*
 * Prints information about the error, on an invalid exception.
 */
void show_invalid_entry_message(int type, unsigned long esr, \
						unsigned long address);

/*
 * Exception handler for all IRQs.
 */
void handle_irq();

#endif
