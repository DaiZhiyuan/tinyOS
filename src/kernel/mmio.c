#include <kernel/mmio.h>

/*
 * Memory-Mapped I/O output
 */
void mmio_write(uint64_t reg, uint32_t data)
{
	*(volatile uint32_t*)reg = data;
}

/*
 * Memory-Mapped I/O input
 */
uint32_t mmio_read(uint64_t reg)
{
	return *(volatile uint32_t*)reg;
}

/*
 * Loop <count> times, so the compiler won't optimize away
 */
void delay(int32_t count)
{
	asm volatile("__delay_%=:\n"
			"subs %[count], %[count], #1\n"
			"bne __delay_%=\n"
			: "=r" (count)
			: [count] "0" (count)
			: "cc"
		);
}
