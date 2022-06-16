#include <stddef.h>
#include <stdint.h>

#include <common/string.h>

#include <kernel/uart.h>
#include <kernel/mmio.h>

#include <peripherals/gpio.h>
#include <peripherals/aux.h>


void uart_init()
{
	uint32_t selector;

	/* 
     * Set Alternative Function 5 for GPIO pins 14, 15
	 * Enables mini UART for boards
     *  17:15 bits - Function Select 15
     *  14:12 bits - Function Select 14
	 */
	selector = mmio_read(GPFSEL1);
	selector &= ~(7 << 12);
	selector |= 2 << 12;
	selector &= ~(7 << 15);
	selector |= 2 << 15;
	mmio_write(GPFSEL1, selector);

	/* 
     * Disable pull up/down for pin 14, 15 
     *  31:30 bits - Resistor Select for GPIO15 
	 *  29:28 bits - Resistor Select for GPIO14 
     */
	selector = mmio_read(GPIO_PUP_PDN_CNTRL_REG0);
	selector &= ~((1 << 31) | (1 << 30) | (1 << 29) | (1 << 28));
	mmio_write(GPIO_PUP_PDN_CNTRL_REG0, selector);

	/* Enable mini UART*/
	mmio_write(AUX_ENABLES, 1);

	/* Disable auto flow control and disable receiver and transmitter */
	mmio_write(AUX_MU_CNTL_REG, 0);

	/*
     * AUX_MU_IER_REG register is primarily used to enable interrupts
	 *  Bit 0: Enable receive interrupt
	 *  Bit 1: Enable transmit interrupt
	 *  Bit 2&3: Required in order to receive interrupts
	 */
	mmio_write(AUX_MU_IER_REG, (1 << 0) | (1 << 2) | (1 << 3));

	/* Clear the receive and transmit FIFO, and enables FIFO */
	mmio_write(AUX_MU_IIR_REG, 0xC6);

	/* Enable 8 bit mode */
	mmio_write(AUX_MU_LCR_REG, 3);

	/* Set RTS line to be always high */
	mmio_write(AUX_MU_MCR_REG, 0);

	/* 
     * System_Clock_Freq = 500 MHz 
	 * baudrate = ((System_Clock_Freq / baudrate_reg) / 8 ) - 1 
	 *          = ((500,000,000 / 115200) / 8) - 1 = 541 
     */
	mmio_write(AUX_MU_BAUD_REG, 541);

	/* Finally, enable transmitter and receiver */
	mmio_write(AUX_MU_CNTL_REG, 3);

}

void uart_putc(unsigned char c)
{
	/* Wait for UART to become ready to transmit */
	while (1) {
        /* Transmitter empty */
		if (mmio_read(AUX_MU_LSR_REG) & (1 << 5))
			break;
	}

    /* Data written is put in the transmit FIFO */
	mmio_write(AUX_MU_IO_REG, c);
}


unsigned char uart_getc()
{
	/* Wait for UART to have received something */
	while (1) {
        /* Data ready */
		if (mmio_read(AUX_MU_LSR_REG) & (1 << 0))
			break;
	}

    /* Data read is taken from the receive FIFO */
	return mmio_read(AUX_MU_IO_REG);
}

void uart_puts(const char* str)
{
	for (size_t i = 0; str[i] != '\0'; i++) {
		if (str[i] == '\n')
			uart_putc('\r');
		uart_putc((unsigned char)str[i]);
	}
}

char *uart_gets()
{
	static char str[MAX_INPUT_LENGTH + 1];
	int i = 0;

	/* Initialize input string with null terminators */
	memset(&str, '\0', MAX_INPUT_LENGTH + 1);

	/* Get up to console's maximum length chars */
	for (i = 0; i < MAX_INPUT_LENGTH; i++) {
		/* Get char from serial, echo back */
		str[i] = (char) uart_getc();
		uart_putc(str[i]);
		/* If we get a NL or CR, break */
		if (str[i] == '\r' || str[i] == '\n') {
			break;
		}
	}
	/* Always append a null terminator at end of string */
	str[i] = '\0';

	return str;
}

void handle_uart_irq()
{
	unsigned char c;

	/* While the receiver holds a valid bit (on Read) */
	while ((mmio_read(AUX_MU_IIR_REG) & (1 << 2)) == (1 << 2)) {
		c = uart_getc();
		if (c == '\r') {
			/* When user presses Enter a CR(Carriage Return) is returned */
			uart_putc(c);
            /* Ouput LineFeed */
			uart_putc('\n');
		}
		else {
			uart_putc(c);
		}
	}
}
