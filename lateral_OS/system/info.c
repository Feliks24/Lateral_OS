#include <system.h>
#include <lib.h>
 
static const char *get_psr_mode_name(enum psr_mode mode)
{
 	switch(mode) { 
 	case PSR_USR:
 		return "User";
 	case PSR_FIQ:
 		return "FIQ";
 	case PSR_IRQ:
 		return "IRQ";
 	case PSR_SVC:
 		return "Supervisor";
 	case PSR_ABT:
 		return "Abort";
 	case PSR_UND:
 		return "Undefined"; 
 	case PSR_SYS:
 		return "System";
 	}
 	return "<unknown>";
}
 
static void print_psr(enum psr psr)
{
  	enum psr_mode mode = psr & PSR_MODE; 
 
 	printf("%c%c%c%c %c%c%c %s",
 	       (psr & PSR_N) ? 'N' : '_',
 	       (psr & PSR_Z) ? 'Z' : '_',
 	       (psr & PSR_C) ? 'C' : '_',
 	       (psr & PSR_V) ? 'V' : '_',
 	       (psr & PSR_I) ? 'I' : '_',
 	       (psr & PSR_F) ? 'F' : '_',
 	       (psr & PSR_T) ? 'T' : '_',
 	       get_psr_mode_name(mode)); 
}
 
static void print_banked_registers(void)
{
 	unsigned int regs[6][2];
  	enum psr psr[5];
 
 	get_banked_sp_lr(PSR_SVC, regs[0]); 
 	get_banked_sp_lr(PSR_FIQ, regs[1]); 
 	get_banked_sp_lr(PSR_IRQ, regs[2]); 
 	get_banked_sp_lr(PSR_ABT, regs[3]); 
 	get_banked_sp_lr(PSR_UND, regs[4]); 
 	get_banked_sp_lr(PSR_SYS, regs[5]); 
 
 	psr[0] = get_banked_spsr(PSR_SVC); 
 	psr[1] = get_banked_spsr(PSR_FIQ); 
 	psr[2] = get_banked_spsr(PSR_IRQ); 
 	psr[3] = get_banked_spsr(PSR_ABT); 
 	psr[4] = get_banked_spsr(PSR_UND); 
 
 	printf(">>> Aktuelle modusspezifische Register (ausser R8-R12) <<<\n\n"
 	       "             ----SP----   ----LR----   -------SPSR--------\n"
 	       "Supervisor:  %p   %p   ",
 	       (void *)regs[0][0], (void *)regs[0][1]); 
 	print_psr(psr[0]);
 	printf("\nFIQ:         %p   %p   ",
 	       (void *)regs[1][0], (void *)regs[1][1]); 
 	print_psr(psr[1]);
 	printf("\nIRQ:         %p   %p   ",
 	       (void *)regs[2][0], (void *)regs[2][1]); 
 	print_psr(psr[2]);
 	printf("\nAbort:       %p   %p   ",
 	       (void *)regs[3][0], (void *)regs[3][1]); 
 	print_psr(psr[3]);
 	printf("\nUndefined:   %p   %p   ",
 	       (void *)regs[4][0], (void *)regs[4][1]); 
 	print_psr(psr[4]);
 	printf("\nUser/System: %p   %p\n\n",
 	       (void *)regs[5][0], (void *)regs[5][1]); 
}
 
static void print_registers(unsigned int regs[16])
{
 	printf("R0:  %p    R4:  %p    R8:  %p    R12: %p\n"
 	       "R1:  %p    R5:  %p    R9:  %p    SP:  %p\n"
 	       "R2:  %p    R6:  %p    R10: %p    LR:  %p\n"
 	       "R3:  %p    R7:  %p    R11: %p    PC:  %p\n",
 	       (void *)regs[0], (void *)regs[4], (void *)regs[8],  (void *)regs[12],
 	       (void *)regs[1], (void *)regs[5], (void *)regs[9],  (void *)regs[13],
 	       (void *)regs[2], (void *)regs[6], (void *)regs[10], (void *)regs[14],
 	       (void *)regs[3], (void *)regs[7], (void *)regs[11], (void *)regs[15]); 
}
 
static void print_context(unsigned int regs[16], enum psr cpsr, enum psr spsr)
{
  	enum psr_mode cmode = cpsr & PSR_MODE; 
 
 	print_registers(regs);
 	printf("CPSR: ");
 	print_psr(cpsr);
 	if (cmode != PSR_USR && cmode != PSR_SYS) { 
 		printf("\nSPSR: ");
 		print_psr(spsr);
 	}
 	printf("\n");
}
/*
 * print_thread_info() - Zustand eines User-Threads ausgeben
 *
 * @regs: gesicherte General-Purpose-Register des Threads
 * @cpsr: gesichertes Statusregister des Threads
 */
void print_thread_info(unsigned int regs[16], enum psr cpsr)
{
 	printf(">>> Thread-Kontext <<<\n\n"); 
 	print_context(regs, cpsr, 0); 
}
 
/*
 * print_exception_info() - aktuellen Systemzustand bei Ausnahme ausgeben
 *
 * @regs: gesicherte General-Purpose-Register
 */
void print_exception_info(unsigned int regs[16])
{
  	enum psr cpsr = get_spsr();
  	enum psr_mode cmode = cpsr & PSR_MODE; 
  	enum psr spsr = get_banked_spsr(cmode); 
  	enum psr_mode smode = spsr & PSR_MODE; 
 
 	/* Registersatz reparieren */ 
 	get_banked_sp_lr(cmode, &regs[13]); 
 
 	printf(">>> Kontext bei Feststellung der Ausnahme <<<\n\n"); 
 	print_context(regs, cpsr, spsr); 
 
 	if (smode == cmode)
 		printf("\n==> Exception aus demselben CPU-Modus: urspruengliches\n"
 		       "    LR und SPSR sind verloren gegangen!\n"); 
 	if (smode == PSR_FIQ && cmode != PSR_FIQ)
 		printf("\n==> Exception in FIQ-Modus: beim Kontext stimmen R8-R12 nicht!\n"); 
 
 	printf("\n");
 
 	print_banked_registers(); 
}
 
/*
 * print_bug_info() - aktuellen Systemzustand bei BUG() ausgeben
 *
 * @regs: gesicherte General-Purpose-Register
 * @func: __func__
 * @file: __FILE__
 * @line: __LINE__
 */
__attribute__ ((noreturn))
void print_bug_info(unsigned int regs[16], const char *func, const char *file, int line)
{
  	enum psr cpsr = get_cpsr(); 
  	enum psr_mode cmode = cpsr & PSR_MODE; 
  	enum psr spsr = 0;
 
 	/* Registersatz reparieren */ 
 	regs[15] -= calc_store_pc_offset(); 
 
 	if (cmode != PSR_USR && cmode != PSR_SYS)
 		spsr = get_spsr();
 
 	printf("BUG in %s() (%s:%x)!\n\n", func, file, line);
 
 	printf(">>> Kontext nach Feststellung des Fehlers <<<\n\n"); 
 	print_context(regs, cpsr, spsr); 
 	printf("\n");
 
 	if (cmode != PSR_USR) { 
 		print_banked_registers(); 
 		stop_execution();
 	} else {
 		printf("Thread beendet.\n"); 
 		terminate();
 	}
}
 
/*
 * stop_execution() - System mit Meldung anhalten
 */
__attribute__ ((noreturn))
void stop_execution(void)
{
 	printf("System angehalten.\n"); 
 	disable_irq(); 
 	while(1)
  		continue;
}
 
