.text
.globl __platform_reset

__platform_reset:
	/* So now we setup everything to copy the values over. */
    ldr r0, =_vectors
	mov r1, #0x0000

	/* And then we do the actual data copy. */
	ldmia r0!,{r2,r3,r4,r5,r6,r7,r8,r9}
	stmia r1!,{r2,r3,r4,r5,r6,r7,r8,r9}
	ldmia r0!,{r2,r3,r4,r5,r6,r7,r8,r9}
	stmia r1!,{r2,r3,r4,r5,r6,r7,r8,r9}
	
	bx lr

/* ARM exception vector table.  This is copied to address 0.  See A2.6
 * "Exceptions" of the ARM Architecture Reference Manual.  */
_vectors:
	ldr pc, reset_addr    /* Reset handler */
	ldr pc, undef_addr	  /* Undefined instruction handler */
	ldr pc, swi_addr	  /* Software interrupt handler */
	ldr pc, prefetch_addr /* Prefetch abort handler */
	ldr pc, abort_addr	  /* Data abort handler */
	ldr pc, reserved_addr /* Reserved */
	ldr pc, irq_addr	  /* IRQ (Interrupt request) handler */
	ldr pc, fiq_addr	  /* FIQ (Fast interrupt request) handler */

reset_addr:     .word _start  /* can't resolve reset, so, meh :( */
undef_addr:     .word arm_undefined
swi_addr:       .word arm_syscall
prefetch_addr:  .word arm_prefetch_abort
abort_addr:     .word arm_data_abort
reserved_addr:  .word arm_reserved
irq_addr:       .word arm_irq
fiq_addr:       .word arm_fiq
