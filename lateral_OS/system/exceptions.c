#include <system.h>
#include <lib.h>
#include <mc.h>

#define INTERNAL_RAM ((void *)0x00200000)

/* interne Funktionen/Daten aus system/exception_asm.S */
extern char _exception_vectors_begin[];
extern char _exception_vectors_end[];

/*
 * init_exceptions() - Exception-Handler installieren
 *
 * Macht die Exception-Handler ausführbereit:
 * - die "Interrupt-Vektor-Tabelle" wird mitsamt Sprungtabelle an den Anfang
 *   vom internen RAM kopiert,
 * - der Memory-Remap wird durchgeführt.
 *
 * Die Reihenfolge der Operationen ist übrigens egal, da wir die IVT nicht nach
 * 0x0000 0000 schreiben, sondern nach 0x0020 0000.
 */
void init_exceptions(void)
{
	memcpy(INTERNAL_RAM, _exception_vectors_begin,
		   _exception_vectors_end - _exception_vectors_begin);
	mc_remap();
}

/*
 * _exception_*() - verschiedene Exception-Handler
 *
 * @regs: Registersatz zum Zeitpunkt der Ausnahme
 *
 * Wir behandeln die vier durch Code ausgelösten Ausnahmen explizit:
 * - Undefined Instruction
 * - Software Interrupt
 * - Prefetch Abort
 * - Data Abort
 *
 * Zusätzlich werden die vier anderen Ausnahmen, die (derzeit) nicht auftreten
 * sollten, unspezifisch abgefangen.
 *
 * Jeder Handler macht eine Meldung, gibt (hoffentlich hilfreiche)
 * Debugginginformationen und hält anschließend das System an.
 */

void _exception_undef(unsigned int regs[16])
{
	printf("\n###########################################################################\n\n");

	printf("Undefined Instruction an Adresse %p!\n\n",
		   (void *)(regs[14] - 4));

	print_debug_info(regs);
	stop_execution();
}

void _exception_swi(unsigned int regs[16])
{
	printf("\n###########################################################################\n\n");

	printf("Software Interrupt an Adresse %p!\n\n", (void *)(regs[14] - 4));

	print_debug_info(regs);
	stop_execution();
}

void _exception_prefabort(unsigned int regs[16])
{
	printf("\n###########################################################################\n\n");

	printf("Prefetch Abort an Adresse %p!\n\n", (void *)(regs[14] - 4));

	print_debug_info(regs);
	stop_execution();
}

void _exception_dataabort(unsigned int regs[16])
{
	printf("\n###########################################################################\n\n");

	printf("Data Abort an Adresse %p bei Zugriff auf Adresse %p!\n\n",
		   (void *)(regs[14] - 8), mc_get_abort_address());

	print_debug_info(regs);
	stop_execution();
}

void _exception_unhandled(unsigned int regs[16])
{
	printf("\n###########################################################################\n\n");

	printf("Unbehandelte Ausnahme (Reset, Reserved, IRQ oder FIQ) aufgetreten!\n"
		   "=> Programmierfehler?\n\n");

	print_debug_info(regs);
	stop_execution();
}