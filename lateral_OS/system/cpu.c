#include <system.h>
#include "memlayout.h"
 
/* interne Funktionen/Daten aus system/cpu_asm.S */ 
void _get_banked_sp_lr(enum psr_mode mode, unsigned int regs[2]); 
void _set_stack(enum psr_mode mode, unsigned int addr); 
 
 
/*
 * get_banked_sp_lr() - modusspezifisches SP und LR auslesen
 *
 * @mode: Modus, desser SP und LR ausgelesen werden sollen
 * @regs: Speicherort für gelesenes SP und LR
 *
 * Die Funktion schreibt das modusspezifischen SP nach regs[0]
 * und das modusspezifische LR nach regs[1].
 *
 * Es wird für alle validen Modi das richtige gemacht (insb.
 * beim User-Modus und beim aktuellen Modus).
 */
void get_banked_sp_lr(enum psr_mode mode, unsigned int regs[2])
{
 	/*
 	 * User- und System-Modus teilen sich die Register und unsere
 	 * Auslesefunktion arbeitet nur mit dem System-Modus korrekt.
 	 * Also fälschen wir ggf. einfach den Modus!
 	 */
 	if (mode == PSR_USR)
  		mode = PSR_SYS;
 
 	_get_banked_sp_lr(mode, regs); 
}
 
/*
 * init_other_stacks() - initialisiert Stacks
 *
 * Stack vom Supervisor-Modus wurde bereits in entry.S auf STACK_BOTTOM gesetzt
 * und wird hier übersprungen.
 */
void init_other_stacks(void)
{
 	/* Ausnahmemodi (außer Supervisor-Modus) */ 
 	_set_stack(PSR_FIQ, STACK_BOTTOM - 1 * STACK_SIZE); 
 	_set_stack(PSR_IRQ, STACK_BOTTOM - 2 * STACK_SIZE); 
 	_set_stack(PSR_ABT, STACK_BOTTOM - 3 * STACK_SIZE); 
 	_set_stack(PSR_UND, STACK_BOTTOM - 4 * STACK_SIZE); 
 
 	/*
 	 * User- und Systemmodus teilen sich den Stack (siehe ARM Architecture
 	 * Reference Manual, A2.2ff)
 	 */
 	_set_stack(PSR_SYS, STACK_BOTTOM - 5 * STACK_SIZE); 
}
 
