#include "memlayout.h"
 
/*
 * _start() - Eintrittspunkt
 *
 * Diese Code-Stück wird durch das Linker-Script direkt am Anfang des
 * Speicherabbilds platziert. Dadurch müssen wir nicht umständlich einen
 * variablen Eintrittspunkt auslesen und an mkimage von U-Boot übergeben.
 */
.global _start
_start:
 	/* Supervisor-Stack initialisieren */ 
 	ldr	sp, =STACK_BOTTOM
 
 	/* Nach C wechseln */ 
 	bl      start_kernel
 
 	/*
 	 * Der Scheduler bekommt ein Array aus 16 Werten übergeben.
 	 * Beim ersten Aufruf werden diese eh verworfen, also ist
 	 * deren Inhalt hier nicht wichtig.
 	 */
 	sub	sp, sp, #(16*4)
  	mov	r0, sp
 	bl	schedule
 
 	/* Ersten Thread laden (Idle) und dabei IRQs erlauben */ 
 	ldmia	sp, {r0-r14}^
 	nop
 	add	sp, sp, #(15*4)
 	ldmfd	sp!, {pc}^
