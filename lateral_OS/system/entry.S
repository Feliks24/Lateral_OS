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
 
 	/* System anhalten, falls wir aus Versehen zurückkehren sollten */ 
        b       stop_execution
