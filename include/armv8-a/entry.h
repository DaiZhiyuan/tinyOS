/*
 * Exception type values - System Registers - ARMv8-a
 */

#ifndef ARM8_ENTRY_H
#define ARM8_ENTRY_H

/*
 * Registers
 *   Size of all saved registers (31 8-byte registes)
 */
#define S_FRAME_SIZE		256

/*
 * EL1t
 * For exceptions taken from EL1 while stack pointer was shared with EL0
 */
#define SYNC_INVALID_EL1t	0	/* Synchronous exception on EL1t */
#define IRQ_INVALID_EL1t	1	/* Interrupt Request exception on EL1t */
#define FIQ_INVALID_EL1t	2	/* Fast Interrupt Request exception on EL1t */
#define ERROR_INVALID_EL1t	3	/* System Error on EL1t */

/*
 * EL1h
 * For exceptions taken from EL1 at the time when dedicated
 * stack pointer was allocated for EL1.
 */
#define SYNC_INVALID_EL1h	4	/* Synchronous exception on EL1h */
#define IRQ_INVALID_EL1h	5	/* Interrupt Request exception on EL1h */
#define FIQ_INVALID_EL1h	6	/* Fast Interrupt Request exception on EL1h */
#define ERROR_INVALID_EL1h	7	/* System Error on EL1h */

/*
 * EL0_64
 * For exceptions taken from EL0, while executing in 64-bit mode.
 */
#define SYNC_INVALID_EL0_64	8	/* Synchronous exception on EL0_64 */
#define IRQ_INVALID_EL0_64	9	/* Interrupt Request exception on EL0_64 */
#define FIQ_INVALID_EL0_64	10	/* Fast Interrupt Request exception on EL0_64 */
#define ERROR_INVALID_EL0_64	11	/* System Error on EL0_64 */

/*
 * EL0_32
 * For exceptions taken from EL0, while executing in 32-bit mode.
 */
#define SYNC_INVALID_EL0_32	12	/* Synchronous exception on EL0_32 */
#define IRQ_INVALID_EL0_32	13	/* Interrupt Request exception on EL0_32 */
#define FIQ_INVALID_EL0_32	14	/* Fast Interrupt Request exception on EL0_32 */
#define ERROR_INVALID_EL0_32	15	/* System Error on EL0_32 */

#endif
