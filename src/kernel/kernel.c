#include <stddef.h>
#include <stdint.h>

#include <kernel/uart.h>
#include <kernel/printk.h>
#include <kernel/utils.h>
#include <kernel/irq.h>
#include <kernel/timer.h>
#include <kernel/console.h>

#include <common/string.h>

#include <armv8-a/irq.h>

void kernel_main(uint64_t dtb_ptr32, uint64_t x1, uint64_t x2, uint64_t x3)
{
    /* Declare as unused */
    dtb_ptr32 = dtb_ptr32;
    x1 = x1;
    x2 = x2;
    x3 = x3;

    /* mini UART */
    uart_init();

    /* Print Initial Starting Message to Serial Console */
    printk("\n\n");
    printk("        __           __   __                  \n");
    printk(".-----.|  |--.--.--.|  |_|__|.--.--.--------. \n");
    printk("|  _  ||     |  |  ||   _|  ||  |  |        | \n");
    printk("|   __||__|__|___  ||____|__||_____|__|__|__| \n");
    printk("|__|         |_____|                          \n");

	printk("\nPhytium Bare-metal system initializing...   \n");

    /* Board Info */
    printk("\n\tBoard: Raspberry Pi 4\n");

    /* Arch Info */
    printk("\tArch: aarch64\n");
    printk("\n");

    /* Exception Levels */
	int el = get_el();
    printk("\n----- Exception level: EL%d -----\n", el);

    /* Interrupts */
    printk("Initializing IRQs...");
    irq_vector_init();
    printk("Done\n");

    printk("Enabling IRQ controllers...");
    enable_interrupt_controller();
    printk("Done\n");

    printk("Enabling IRQs...");
    irq_enable();
    printk("Done\n");

    /* Console */
    console();
}
