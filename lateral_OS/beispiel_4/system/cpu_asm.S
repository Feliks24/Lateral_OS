/*
 * _set_stack() - setzt den Stack eines privilegierten Modus neu
 *
 * @r0:	privilegierter Modus
 * @r1: neuer Wert für SP im entsprechenden Modus
 *
 * Soll der Stack des User-Modus geändert werden, muss der System-Modus
 * als Parameter übergeben werden!
 */
.global _set_stack
_set_stack:
 	/*
 	 * In gewünschten Modus wechseln (mittels Read-Modify-Write, 
 	 * siehe AARM A4.1.39), alten Wert von CPSR in r2 halten.
 	 */
  	mrs	r2, cpsr
 	bic	r3, r2, #0x1f
 	orr	r3, r3, r0
  	msr	cpsr_c, r3
 
 	/* Stack setzen */ 
  	mov	sp, r1
 
 	/* Modus mittels r2 zurücksetzen */ 
  	msr	cpsr_c, r2
 
  	mov	pc, lr
 
/*
 * _get_banked_sp_lr() - modusspezifische Register SP und LR auslesen
 *
 * @r0: privilegierter Modus
 * @r1: Zeiger auf Speicher, wo SP und LR gespeichert werden sollen
 *
 * Diese Funktion hat keine Sicherheitschecks! Siehe get_banked_sp_lr().
 */
.global _get_banked_sp_lr
_get_banked_sp_lr:
 	/*
 	 * In gewünschten Modus wechseln (mittels Read-Modify-Write, 
 	 * siehe AARM A4.1.39), alten Wert von CPSR in r2 halten.
 	 */
  	mrs	r2, cpsr
 	bic	r3, r2, #0x1f
 	orr	r3, r3, r0
  	msr	cpsr_c, r3
 
 	/* Stack und Link Register lesen */ 
 	stmia	r1, {sp, lr}
 
 	/* Modus mittels r2 zurücksetzen */ 
  	msr	cpsr_c, r2
 
  	mov	pc, lr
 
/* get_banked_spsr() - modusspezifisches SPSR auslesen */ 
.global get_banked_spsr
get_banked_spsr:
  	mrs	r2, cpsr
 	bic	r3, r2, #0x1f
 	orr	r3, r3, r0
  	msr	cpsr_c, r3
 
  	mrs	r0, spsr
 
  	msr	cpsr_c, r2
  	mov	pc, lr
 
/* get_cpsr() - CPSR auslesen */ 
.global get_cpsr
get_cpsr:
  	mrs	r0, cpsr
  	mov	pc, lr
 
 
/* get_spsr() - SPSR des aktuellen CPU-Modus auslesen */ 
.global get_spsr
get_spsr:
  	mrs	r0, spsr
  	mov	pc, lr
 
 
/* set_spsr() - SPSR des aktuellen CPU-Modus schreiben */ 
.global set_spsr
set_spsr:
  	msr	spsr, r0
  	mov	pc, lr
 
 
/*
 * calc_store_pc_offset() - PC-Offset bei STM/STR ausrechnen
 *
 * Welcher Wert des PC bei STM/STR gespeichert wird, ist
 * implementierungsabhängig! (siehe AARM, A2.4.3)
 * Von da ist auch der Code.
 */
.global calc_store_pc_offset
calc_store_pc_offset:
 	sub	r1, pc, #4
 	str	pc, [sp, #-4]
 	ldr	r0, [sp, #-4]
 	sub	r0, r0, r1
  	mov	pc, lr
 
/* disable_irq() - I-Flag in CPSR setzen */ 
.global disable_irq
disable_irq:
  	mrs	r0, cpsr
 	orr	r0, r0, #(1 << 7)
  	msr	cpsr_c, r0
  	mov	pc, lr
 
/* enable_irq() - I-Flag in CPSR löschen */ 
.global enable_irq
enable_irq:
  	mrs	r0, cpsr
 	bic	r0, r0, #(1 << 7)
  	msr	cpsr_c, r0
  	mov	pc, lr
 
/* irq_disabled() - Testet, ob Interrupts maskiert */ 
.global irq_disabled
irq_disabled:
  	mrs	r0, cpsr
  	and	r0, r0, #(1 << 7)
  	mov	pc, lr
 
/*
 * idle() - Idle-Thread mit Stromsparmodus
 *
 * (siehe Kapitel 4.9 im ARM920T Handbuch: Wait for Interrupt)
 */
.global idle
idle:
 	mcr	p15, 0, r0, c7, c0, 4
 	b	idle
