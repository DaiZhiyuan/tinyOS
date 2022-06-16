#include <stdint.h>

#ifndef MMIO_H
#define MMIO_H

/*
 * Memory-Mapped I/O output.
 */
void mmio_write(uint64_t reg, uint32_t data);
/*
 * Memory-Mapped I/O input.
 */
uint32_t mmio_read(uint64_t reg);

/*
 * Delays count of clock cycles.
 */
void delay(int32_t count);

#endif
