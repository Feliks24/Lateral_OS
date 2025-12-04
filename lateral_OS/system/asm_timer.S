
.global _cpsr_interrupt_enable
_cpsr_interrupt_enable:
	
	//get cpsr
	MRS r0, CPSR

	//bit clear command
	//clears the bit 7 in r0
	BIC r0, r0, #0x80    

	//write back
	//with _c bit set, only write control fields
	MSR CPSR_c, r0	

    //return
    mov pc, lr   
