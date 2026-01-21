/*
 * Die folgenden acht Instruktionen werden später in die
 * "Interrupt-Vektor-Tabelle" kopiert, die sich am Anfang des internen RAMs
 * befindet (siehe ARM Architecture Reference Manual, A2.6).
 *
 * Wir können nicht direkt mit PC-relativen Branches (B) arbeiten, da diese nur
 * eine beschränkte Reichweite haben und nicht bis in den externen RAM reichen.
 * Stattdessen verwenden wir eine kleine Sprungtabelle mit den absoluten
 * Adressen, welche direkt hinter den acht Instruktionen liegt und mitkopiert
 * wird.
 *
 * Es wird nicht sofort der Handler im C-Code angesprungen, sondern zunächst
 * ein Trampolin, was uns die notwendige vollständige Kontrolle über die
 * Inhalte der Register gibt.
 */
 
.global _exception_vectors_begin
_exception_vectors_begin:
 	.word	0xE7F000F0		/* Reset --> löst Undef aus */ 
 	ldr	pc, jt_undef		/* Undefined Instruction */ 
 	ldr	pc, jt_swi		/* Software Interrupt */ 
 	ldr	pc, jt_prefabort	/* Prefetch Abort */ 
 	ldr	pc, jt_dataabort	/* Data Abort */ 
 	.word	0xE7F000F0		/* Reserved --> löst Undef aus*/ 
 	ldr	pc, jt_interrupt	/* Interrupt */ 
 	.word	0xE7F000F0		/* Fast Interrupt --> löst Undef aus */ 
 
jt_undef:	.word undef_trampoline
jt_swi:		.word swi_trampoline
jt_prefabort:	.word prefabort_trampoline
jt_dataabort:	.word dataabort_trampoline
jt_interrupt:	.word interrupt_trampoline
 
.global _exception_vectors_end
_exception_vectors_end:
 
/*
 * *_trampoline() - Trampoline für Ausnahmen
 *
 * Da die Trampoline alle recht ähnlich sind, erzeugen wir sie mit einem
 * parametrisierten Makro. Es besteht aus drei Schritten: Sichern,
 * Bearbeiten, Wiederherstellen.
 *
 * Das Trampoline sichert sämtliche User-Register schön sortiert auf dem
 * Stack. Der User-PC (jetzt LR) wird entsprechend der Ausnahme korrigiert
 * gespeichert; das User-CPSR (jetzt SPSR) wird später im C-Code gesichert.
 *
 * Dann erfolgt die eigentliche Prüfung und Behandlung der Ausnahme.
 * Außerdem bekommt der Scheduler die Chance, einen Kontextwechsel
 * durch Tauschen der Register auf dem Stack (und des SPSRs) durchzuführen.
 *
 * Zum Schluss wird der gesicherte Kontext wieder hergestellt.
 */
 
.macro	TRAMPOLINE name lr_offset id handler needs_regs needs_id
  \name:
 	/* Rücksprungadresse ggf. korrigieren und sichern */ 
  .if \lr_offset
 	sub	lr, lr, #(\lr_offset)
  .endif
 	stmfd	sp!, {lr}
 
 	/*
 	 * Unveränderte Register R0-R12 sichern, sowie R13-R14 des User-Modus.
 	 * Aufgrund des S-Bits ist kein Writeback möglich, also Platz auf Stack
 	 * manuell reservieren.
 	 */
 	sub	sp, #(15*4)
 	stmia	sp, {r0-r14}^
 
 	/*
 	 * In unseren C-Code verzweigen:
 	 *   1. Gültigkeit der Exception prüfen
 	 *   2. Exception behandeln
 	 *   3. ggf. Kontextwechsel
 	 */
  	mov	r0, #(\id)
 	bl	_validate_exception
 
  .if \needs_regs
  	mov	r0, sp
  .endif
  .if \needs_id
  	mov	r1, #(\id)
  .endif
 	bl	\handler
 
  	mov	r0, sp
 	bl	schedule
 
 	/*
 	 * Zuvor gesicherte Register wieder herstellen (R0-R12, R13-R14
 	 * User-Modus). Laut Doku sollte in der Instruktion nach LDM^ auf
 	 * keines der umgeschalteten Register zugegriffen werden.
 	 */
 	ldmia	sp, {r0-r14}^
 	nop
 	add	sp, sp, #(15*4)
 
 	/* Rücksprung durch Laden des PC mit S-Bit */ 
 	ldmfd	sp!, {pc}^
.endm
 
/* kein Reset */
TRAMPOLINE undef_trampoline	4 1 _exception_fault	 1 1
TRAMPOLINE swi_trampoline	0 2 _exception_swi	 1 0
TRAMPOLINE prefabort_trampoline	4 3 _exception_fault	 1 1
TRAMPOLINE dataabort_trampoline	8 4 _exception_fault	 1 1
/* kein Reserved */
TRAMPOLINE interrupt_trampoline	4 6 _exception_interrupt 0 0
/* kein Fast Interrupt */
 
