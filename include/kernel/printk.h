#include <stdarg.h>

#ifndef PRINTK_H
#define PRINTK_H

/*
 * Prints a kernel message, like printf.
 */
int printk(const char *fmt, ...);

#endif
