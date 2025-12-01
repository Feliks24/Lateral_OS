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
 
 	get_banked_sp_lr(PSR_SVC, regs[0]); 
 	get_banked_sp_lr(PSR_FIQ, regs[1]); 
 	get_banked_sp_lr(PSR_IRQ, regs[2]); 
 	get_banked_sp_lr(PSR_ABT, regs[3]); 
 	get_banked_sp_lr(PSR_UND, regs[4]); 
 	get_banked_sp_lr(PSR_SYS, regs[5]); 
 
 	printf(">>> Aktuelle modusspezifische Register (außer SPSR und R8-R12) <<<\n\n"
 	       "                 SP           LR\n"
 	       "Supervisor:  %p   %p\n"
 	       "FIQ:         %p   %p\n"
 	       "IRQ:         %p   %p\n"
 	       "Abort:       %p   %p\n"
 	       "Undefined:   %p   %p\n"
 	       "User/System: %p   %p\n\n",
 	       (void *)regs[0][0], (void *)regs[0][1],
 	       (void *)regs[1][0], (void *)regs[1][1],
 	       (void *)regs[2][0], (void *)regs[2][1],
 	       (void *)regs[3][0], (void *)regs[3][1],
 	       (void *)regs[4][0], (void *)regs[4][1],
 	       (void *)regs[5][0], (void *)regs[5][1]); 
}
 
static void print_registers(unsigned int regs[16])
{
 	printf(">>> Registerschnappschuss (aktueller Modus, angefertigt bei %p) <<<\n\n"
 	       "R0:  %p    R4:  %p    R8:  %p    R12: %p\n"
 	       "R1:  %p    R5:  %p    R9:  %p    SP:  %p\n"
 	       "R2:  %p    R6:  %p    R10: %p    LR:  %p\n"
 	       "R3:  %p    R7:  %p    R11: %p    PC:  %p\n\n",
 	       (void *)(regs[15] - calc_store_pc_offset()),
 	       (void *)regs[0], (void *)regs[4], (void *)regs[8],  (void *)regs[12],
 	       (void *)regs[1], (void *)regs[5], (void *)regs[9],  (void *)regs[13],
 	       (void *)regs[2], (void *)regs[6], (void *)regs[10], (void *)regs[14],
 	       (void *)regs[3], (void *)regs[7], (void *)regs[11], (void *)regs[15]); 
}
 
 
/*
 * print_debug_info() - aktuellen Systemzustand ausgeben
 *
 * @regs: gesicherte General-Purpose-Register des aktuellen Modus
 *
 * Nicht ganz perfekt, aber ein guter Anfang.
 */
void print_debug_info(unsigned int regs[16])
{
  	enum psr cpsr = get_cpsr(); 
  	enum psr_mode cmode = cpsr & PSR_MODE; 
 
 	/* normale Register */
 	print_registers(regs);
 
 	/* Statusregister im aktuellen Modus */ 
 	printf(">>> Aktuelle Statusregister (SPSR des aktuellen Modus) <<<\n\n"
 	       "CPSR: ");
 	print_psr(cpsr);
 	if (cmode != PSR_USR && cmode != PSR_SYS) { 
 		/* SPSR ist nur in Ausnahmemodi verfügbar */ 
  		enum psr spsr = get_spsr();
  		enum psr_mode smode = spsr & PSR_MODE; 
 
 		printf("\nSPSR: ");
 		print_psr(spsr);
 
 		if (smode == cmode) { 
 			printf("\n\n   ==> Exception aus demselben CPU-Modus, urspruengliches\n"
 			       "       LR und SPSR sind verloren gegangen!"); 
 		}
 	}
 	printf("\n\n");
 
 	/* ggf. modusspezifische Register (außer SPSR und R8-R12) */ 
 	if (cmode != PSR_USR)
 		print_banked_registers(); 
 	else
 		printf("User-Modus => keine modusspezifischen Register abrufbar!\n\n"); 
}
 
/*
 * stop_execution() - System mit Meldung anhalten
 */
void stop_execution(void)
{
 	printf("System angehalten.\n"); 
 	while(1)
  		continue;
}
 
