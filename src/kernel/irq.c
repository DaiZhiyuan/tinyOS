#include <stdint.h>

#include <kernel/printk.h>
#include <kernel/mmio.h>
#include <kernel/uart.h>
#include <kernel/timer.h>

#include <peripherals/irq.h>
#include <peripherals/aux.h>

const char entry_error_messages[16][32] = {
	"SYNC_INVALID_EL1t",
	"IRQ_INVALID_EL1t",
	"FIQ_INVALID_EL1t",
	"ERROR_INVALID_EL1T",

	"SYNC_INVALID_EL1h",
	"IRQ_INVALID_EL1h",
	"FIQ_INVALID_EL1h",
	"ERROR_INVALID_EL1h",

	"SYNC_INVALID_EL0_64",
	"IRQ_INVALID_EL0_64",
	"FIQ_INVALID_EL0_64",
	"ERROR_INVALID_EL0_64",

	"SYNC_INVALID_EL0_32",
	"IRQ_INVALID_EL0_32",
	"FIQ_INVALID_EL0_32",
	"ERROR_INVALID_EL0_32"
};

void enable_interrupt_controller()
{
	/*
	 * Enable AUX interrupts (for mini UART) and system timer 1
	 */
	mmio_write(IRQ0_SET_EN_0, /* AUX_IRQ | */ SYSTEM_TIMER_IRQ_1);
}

void show_invalid_entry_message(int type, uint64_t esr, uint64_t address)
{
	printk("%s, ESR: %x, address: %x\n", \
			entry_error_messages[type], esr, address);
}

void handle_irq()
{
	uint32_t irq;

	irq = mmio_read(IRQ0_PENDING_0);

	/* While we have a valid value from the interrupt */
	while (irq) {
		/* Check which interrupt is it */
		if (irq & SYSTEM_TIMER_IRQ_1) {
			/* Remove the bit we handled */
			irq &= ~SYSTEM_TIMER_IRQ_1;
			handle_timer_irq();
		}
		else if (irq & AUX_IRQ) {
			/* Remove the bit we handled */
			irq &= ~AUX_IRQ;
			handle_uart_irq();
		}
		else {
			printk("Unknown pending irq: %x\n", irq);
		}
	}
}
