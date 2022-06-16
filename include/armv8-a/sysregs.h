#ifndef SYSREGS_H
#define SYSREGS_H

/*
 * SCTLR_EL1, System Control Register (EL1)
 */

/* Reserved bits are initialized to 1 */
#define SCTLR_RESERVED		(3 << 28) | (3 << 22) | (1 << 20) | (1 << 11)
/* Work only with little-endian format at EL1 */
#define SCTLR_EE_LITTLE_ENDIAN	(0 << 25)
/* Work only with little-endian format at EL0 */
#define SCTLR_EOE_LITTLE_ENDIAN	(0 << 24)
/* Disable instruction cache */
#define SCTLR_I_CACHE_DISABLED	(0 << 12)
/* Disable data cache */
#define SCTLR_D_CACHE_DISABLED	(0 << 2)
/* Disable MMU */
#define SCTLR_MMU_DISABLED	    (0 << 0)

#define SCTLR_VALUE_MMU_DISABLED	(SCTLR_RESERVED | SCTLR_EE_LITTLE_ENDIAN | SCTLR_I_CACHE_DISABLED | SCTLR_D_CACHE_DISABLED | SCTLR_MMU_DISABLED)

/*
 * HCR_EL2, Hypervisor Configuration Register (EL2)
 */

/* Sets execution state at next lower level to be AArch64 */
#define HCR_RW			(1 << 31)

#define HCR_VALUE		HCR_RW

/*
 * SCR_EL3, Secure Configuration Register (EL3)
 */
/* Reserved bits are initialized to 1 */
#define SCR_RESERVED                    (3 << 4)
/* Sets execution state at next lower level to be AArch64 */
#define SCR_RW                          (1 << 10)
/*
 * EL0 and EL1 are in Non-secure state, memory accesses,
 * from those exception levels cannot access Secure memory. 
 */
#define SCR_NS                          (1 << 0)

#define SCR_VALUE                       (SCR_RESERVED | SCR_RW | SCR_NS)

/*
 * SPSR_EL3, Saved Program Status Register (EL3)
 */

/* After we change EL to EL1 all types of interrupts (F, I, A) will be masked */
#define SPSR_MASK_ALL 	(7 << 6)
/* EL1h mode: We are using EL1 dedicated stack pointer */
#define SPSR_EL1h		(5 << 0)

#define SPSR_VALUE		(SPSR_MASK_ALL | SPSR_EL1h)

#endif
