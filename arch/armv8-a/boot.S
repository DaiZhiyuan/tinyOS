/*
 * boot.S - Assembly Startup Code for AArch64 mode
 */

#include <armv8-a/sysregs.h>

.section ".text.boot"

.global _start

/*
 * Entry point for the kernel
 * x0 -> 32 bit pointer to DTB in memory (bsp core only) / 0 (ap cores)
 * x1 -> 0
 * x2 -> 0
 * x3 -> 0
 * x4 -> 32 bit kernel entry point, _start location Preserve these registers as
 * argument for kernel_main
 */

_start:
	/* Check if processor ID is zero (executing on bsp core), else hang */
	mrs x0, mpidr_el1
	and x0, x0, #0xFF
	cbz x0, bsp_processor
	b ap_hang
ap_hang:
	b ap_hang

bsp_processor:
    /*
	 * System Control Register (SCTLR)
	 * Configuring different parameters for the processor.
	 */
	ldr x0, =SCTLR_VALUE_MMU_DISABLED
	msr sctlr_el1, x0

	/* 
     * Hypervisor Configuration Register (HCR)
	 * Setting execution state at EL1 to be AArch64.
	 */
	ldr x0, =HCR_VALUE
	msr hcr_el2, x0

	/* 
     * Secure Configuration Register (SCR)
	 * Setting execution state at EL2 to be AArch64
	 * and that all lower exception levels will be "non secure".
	 */
	ldr x0, =SCR_VALUE
	msr scr_el3, x0

	/* 
     * Saved Program Status Register (SPSR)
	 * Preparing processor state.
	 */
	ldr x0, =SPSR_VALUE
	msr spsr_el3, x0

	/* 
     * Exception Link Register (ELR)
	 * Holds the address, to which we are going to return
	 * after eret instruction will be executed.
	 * Here we set this address to the location of el1_entry label.
	 */
	adr x0, el1_entry
	msr elr_el3, x0

	eret

	/* Code executing only by bsp core */
el1_entry:

    /* Setup the stack (64-bit) */
	mov sp, #0x80000

    /* Clear BSS Segment */
	ldr x5, =__bss_start
	ldr w6, =__bss_size
clear_bss:
	cbz w6, kernel_entry
	str xzr, [x5], #8
	sub w6, w6, #1
	cbnz w6, clear_bss

kernel_entry:
    /* Call kernel_main (C Code) */
	bl kernel_main

    /*
     * Should not return from here.
     * failsafe, halt this core too.
     */
    b ap_hang

