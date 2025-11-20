
.global _init_stack
_init_stack:
	mrs r2, cpsr
	bic r3,r2, #0x1f
	orr r3,r3,r0
	msr cpsr_c, r3
	mov sp, r1
	msr cpsr_c, r2
	mov pc, lr

