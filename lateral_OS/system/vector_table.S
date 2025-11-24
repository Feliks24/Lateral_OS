/*
* The instructions that are supposed to be written into the IVT each of them
* can jump to a different label. For each IVT entry there is a separate C
* function tha twill just print the interrupt that it gets.

* The trampoline functions are written right after the IVT

* note that second undef is actually reserved space but it needs to be filled

*/


.global _interrupt_table
_interrupt_table:
 	ldr	pc,trampoline_reset 	
 	ldr	pc,trampoline_undef 	
 	ldr	pc,trampoline_swi 	 
 	ldr	pc,trampoline_prefetch 	
 	ldr	pc,trampoline_data	
 	ldr	pc,trampoline_irq	
 	ldr	pc,trampoline_fiq	

trampoline_reset:
    mov r0, #RESET
    b _general_exception
trampoline_undef:
    mov r0, #UNDEFINED
    b _general_exception
trampoline_swi:
    mov r0, #SOFTWARE_INTERUPT
    b _general_exception
trampoline_prefetch:
    mov r0, #PREFETCH_ABORT
    b _general_exception
trampoline_data:
    mov r0, #DATA_ABORT
    b _general_exception
trampoline_irq:	
    mov r0, #IRQ
    b _general_exception
trampoline_fiq:
    mov r0, #FIQ
    b _general_exception

.global _interrupt_table_end
_interrupt_table_end:
