#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

#include <peripherals/gpio.h>

/*
 * Sets the Alternative Function @a func for GPIO pin at @a pin_num, using GPFSELn
 */
int gpio_pin_set_func(uint8_t pin_num, gpio_func func);

/*
 * Sets the GPIO pin (High) at @a pin_num, using GPSETn ( @ref GPSET0, @ref GPSET1).
 */
int gpio_pin_set(uint8_t pin_num);

/*
 * Clears the GPIO pin (Low) at pin_num, using GPCLRn.
 */
int gpio_pin_clear(uint8_t pin_num);

#endif
