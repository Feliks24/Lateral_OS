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
 * ein Trampolin, was es uns ermöglicht einige Register für Debug-Ausgaben zu
 * sichern.
 *
 * (Als Alternative lassen sich die Trampoline mit in den internen RAM
 * kopieren. Dann könnte man die acht Sprungbefehle mit B realisieren, und in
 * den Trampolinen müsste man LDR (auch mit Sprungtabelle) statt B verwenden.)
 */
 
.global _exception_vectors_begin
_exception_vectors_begin:
 	ldr	pc, jt_unhandled	/* Reset */ 
 	ldr	pc, jt_undef		/* Undefined Instruction */ 
 	ldr	pc, jt_swi		/* Software Interrupt */ 
 	ldr	pc, jt_prefabort	/* Prefetch Abort */ 
 	ldr	pc, jt_dataabort	/* Data Abort */ 
 	ldr	pc, jt_unhandled	/* Reserved */ 
 	ldr	pc, jt_unhandled	/* Interrupt */ 
 	ldr	pc, jt_unhandled	/* Fast Interrupt */ 
 
jt_undef:	.word undef_trampoline
jt_swi:		.word swi_trampoline
jt_prefabort:	.word prefabort_trampoline
jt_dataabort:	.word dataabort_trampoline
jt_unhandled:	.word unhandled_trampoline
 
.global _exception_vectors_end
_exception_vectors_end:
 
 
/*
 * *_trampoline() - Trampoline für Ausnahmeweiterleitung nach C
 *
 * Die nachfolgenden Trampoline sind im Prinzip identisch: Nach dem Sichern
 * einer Momentaufnahme aller Register auf dem Stack wird der C-Handler
 * angesprungen, der alles weitere übernimmt.
 *
 * (Da wir SP mitsichern, können wir kein Writeback verwenden, sondern müssen
 * den Stack-Pointer manuell versetzen.)
 */
 
undef_trampoline:
 	/* Debug-Registersatz sichern */ 
 	stmdb	sp, {r0-r15}
 	sub	sp, sp, #(16*4)
 
 	/* C-Handler (ohne Wiederkehr) aufrufen mit Zeiger auf Registersatz */ 
  	mov	r0, sp
 	b	_exception_undef
 
swi_trampoline:
 	/* Debug-Registersatz sichern */ 
 	stmdb	sp, {r0-r15}
 	sub	sp, sp, #(16*4)
 
 	/* C-Handler (ohne Wiederkehr) aufrufen mit Zeiger auf Registersatz */ 
  	mov	r0, sp
 	b	_exception_swi
 
prefabort_trampoline:
 	/* Debug-Registersatz sichern */ 
 	stmdb	sp, {r0-r15}
 	sub	sp, sp, #(16*4)
 
 	/* C-Handler (ohne Wiederkehr) aufrufen mit Zeiger auf Registersatz */ 
  	mov	r0, sp
 	b	_exception_prefabort
 
dataabort_trampoline:
 	/* Debug-Registersatz sichern */ 
 	stmdb	sp, {r0-r15}
 	sub	sp, sp, #(16*4)
 
 	/* C-Handler (ohne Wiederkehr) aufrufen mit Zeiger auf Registersatz */ 
  	mov	r0, sp
 	b	_exception_dataabort
 
unhandled_trampoline:
 	/* Debug-Registersatz sichern */ 
 	stmdb	sp, {r0-r15}
 	sub	sp, sp, #(16*4)
 
 	/* C-Handler (ohne Wiederkehr) aufrufen mit Zeiger auf Registersatz */ 
  	mov	r0, sp
 	b	_exception_unhandled
 
