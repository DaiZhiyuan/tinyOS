#ifndef STDLIB_H
#define STDLIB_H

/*
 * Converts an int (Only base 10 supported for now)
 * to a null-terminated string using the specified base.
 */
char *itoa(int value);

/*
 * Converts a string to an int
 */
int atoi(const char *str);

#endif
