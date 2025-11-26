/*
 * _set_stack() - setzt den Stack eines priviligierten Modus neu
 *
 * @r0:	priviligierter Modus
 * @r1: neuer Wert für SP im entsprechenden Modus
 *
 * Soll der Stack des User-Modus geändert werden, muss der System-Modus
 * als Parameter übergeben werden!
 */
.global _set_stack
_set_stack:
 	/*
 	 * In gewünschten Modus wechseln (mittels Read-Modify-Write, 
 	 * siehe AARM A4.1.39), alten Wert von CPRS in r2 halten.
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
 * @r0: priviligierter Modus
 * @r1: Zeiger auf Speicher, wo SP und LR gespeichert werden sollen
 *
 * Diese Funktion hat keine Sicherheitschecks! Siehe get_banked_sp_lr().
 */
.global _get_banked_sp_lr
_get_banked_sp_lr:
 	/*
 	 * In gewünschten Modus wechseln (mittels Read-Modify-Write, 
 	 * siehe AARM A4.1.39), alten Wert von CPRS in r2 halten.
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
 
 
/* get_cpsr() - CPSR des aktuellen CPU-Modus auslesen */ 
.global get_cpsr
get_cpsr:
  	mrs	r0, cpsr
  	mov	pc, lr
 
 
/* get_spsr() - SPSR des aktuellen CPU-Modus auslesen */ 
.global get_spsr
get_spsr:
  	mrs	r0, spsr
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
 
