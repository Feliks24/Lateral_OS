/*
 * Syscall-Assembler-Interface.
 *
 * Jeder Syscall ist sehr einfach gehalten: nur der SWI-Aufruf gefolgt von
 * einem Rücksprung. Daraus ergeben sich verschiedene Dinge für
 * Parameterübergabe und zu rettende Register:
 * 
 * 1. Wir können bis zu vier 32-Bit-Parameter haben. Diese werden bei
 *    Aufruf des Syscalls in r0 bis r3 platziert und stehen dann dem
 *    SWI-Handler zur Verfügung. (Parameter via Stack ist eine schlechte
 *    Idee, da unterschiedliche Stacks und User-Pointer in Kernel-Modus
 *    erstmal nicht vertrauenswürdig.)
 *
 *    Menge und Typen der Parameter ergeben sich einfach durch die 
 *    C-Deklaration.
 *
 * 2. Der SWI-Handler muss den User-Kontext wie einen AAPCS-Funktionsaufruf
 *    behandeln, was änderbare Register angeht.
 *
 * 3. Wir haben einen 32-Bit-Rückgabewert in r0.
 */
 
#include <syscallids.h>
 
.global fork
fork:
 	swi	SWI_FORK
  	mov	pc, lr
 	
.global wait
wait:
 	swi	SWI_WAIT
  	mov	pc, lr
 	
.global exit
exit:
 	swi	SWI_EXIT
 
.global getchar
getchar:
 	swi	SWI_GETCHAR
  	mov	pc, lr
 
.global putchar
putchar:
 	swi	SWI_PUTCHAR
  	mov	pc, lr
