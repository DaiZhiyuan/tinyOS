#ifndef UART_H
#define UART_H

/*
 * Maximum length for the serial input.
 */
#define MAX_INPUT_LENGTH 80

/*
 * Initializes the UART interface based on the running device.
 */
void uart_init();

/*
 * Sends a byte to the UART (serial output).
 */
void uart_putc(unsigned char c);

/*
 * Gets a byte to the UART (serial input).
 */
unsigned char uart_getc();

/*
 * Sends a string to the UART (serial output).
 */
void uart_puts(const char* str);

/*
 * Gets a string from the UART (serial input).
 */
char *uart_gets();

/*
 * Exception handler for the UART (Asynchronous read from the serial input).
 */
void handle_uart_irq();

#endif
