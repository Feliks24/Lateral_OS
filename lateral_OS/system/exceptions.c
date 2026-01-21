#include <system.h>
#include <lib.h>
#include <mc.h>
#include <dbgu.h>
#include <st.h>
 
#define INTERNAL_RAM ((void *)0x00200000)
 
enum exception {
 	E_RESET = 0,
 	E_UNDEF,
 	E_SWI,
 	E_PREFABORT,
 	E_DATAABORT,
 	E_RESERVED,
 	E_IRQ,
 	E_FIQ,
 	E_MAX,
};
 
/* Lookup-Table für Exception-Id => Name */ 
static const char *exception_names[] = { 
 	"Reset",
 	"Undefined Instruction",
 	"Software Interrupt",
 	"Prefetch Abort",
 	"Data Abort",
 	"Reserved",
 	"Interrupt",
 	"Fast Interrupt",
};
 
/* Lookup-Table für Exception-Id => resultierender Modus */ 
static const enum psr_mode expected_mode[] = { 
 	0,	 /* Reset */
 	PSR_UND, /* Undefined Instruction */ 
 	PSR_SVC, /* Software Interrupt */
 	PSR_ABT, /* Prefetch Abort */ 
 	PSR_ABT, /* Data Abort */ 
 	0,	 /* Reserved */
 	PSR_IRQ, /* Interrupt */
 	0,	 /* Fast Interrupt */
};
 
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
 * _validate_exception() - Prüft aktuelle Ausnahme auf Sinnhaftigkeit
 *
 * @e: die aufgetretene Ausnahme
 *
 * Reagiert mit Abschießen des aktuellen Threads oder mit Anhalten des
 * Systems, falls ein Missstand festgestellt wird.
 */
void _validate_exception(enum exception e)
{
  	enum psr_mode mode = get_cpsr() & PSR_MODE; 
 
 	if (mode == PSR_USR) {
 		/*
 		 * Noch haben wir keinen Speicherschutz, womit ausgeschlossen
 		 * wäre, dass ein User-Programm hier ankommt. Falls also doch
 		 * ein User-Programm hier ankommt, ist es falsch und wir
 		 * beenden es hart. (Anstatt das System anzuhalten.)
 		 */
  		asm (".word 0xE7F000F0"); 
 	}
 	BUG_ON(e >= E_MAX);
 	BUG_ON(mode != expected_mode[e]); 
 	BUG_ON(!irq_disabled()); 
}
 
/*
 * _exception_interrupt() - Interrupt-Handler
 *
 * Wir benutzen das Vectoring-Feature des AIC nicht. Mit anderen Worten: _alle_
 * Interrupts kommen hier an. Da alle unsere benutzten Komponenten am
 * SYS-Interrupt hängen, macht das allerdings keinen Unterschied.
 *
 * Der merkbare Unterschied ist, dass wir AIC_IVR bisher nicht gelesen haben.
 * Wie sich herausstellt, muss man dann auch nicht AIC_EOICR schreiben. Da wir
 * außerdem Level-sensitive Interrupts verwenden, gibt es auch keinen Grund
 * AIC_ICCR zu schreiben.
 *
 * Im Grunde erfährt der AIC nie, dass wir die IRQ-Behandlung angetreten haben.
 * Wenn wir die IRQs an den Geräten bestätigen, führt dies dazu, dass der AIC
 * auch die IRQ-Leitung am Kern runter nimmt. Aus Sicht des AICs sehen damit
 * alle Interrupts wie Spurious Interrupts aus, was uns aber nicht stört.
 */
void _exception_interrupt(void)
{
  	enum psr_mode mode = get_spsr() & PSR_MODE; 
 
 	/*
 	 * Interrupts dürfen bei uns nur in Nicht-Ausnahme-Modi generiert
 	 * werden; der Kontextwechsel funktioniert sonst nicht verlässlich.
 	 */
 	BUG_ON(mode != PSR_USR && mode != PSR_SYS); 
 
 	/*
 	 * Durchprobieren aller möglichen Quellen. Da Level-sensitiv, könnten
 	 * wir genaugenommen nach dem ersten Handler, der tatsächlich eine
 	 * Interrupt-Ursache beseitigt, zurückspringen. (Für Edge-triggered ist
 	 * das einfache Durchgehen noch nicht genug. Da könnten wir immer noch
 	 * etwas verpassen.)
 	 */
 	st_handle_irq(); 
 	dbgu_handle_irq(); 
}
 
/*
 * _exception_fault() - Handler für Abstürze
 *
 * @regs: Register des Usermodus
 * @e: die aufgetretene Ausnahme
 *
 * Bei diesem Handler kommt alles an, was eine sinnvolle Ausnahme ist (siehe
 * _validate_exception) und auf einen Fehler hindeutet:
 *
 * - Data- und Prefetch-Abort
 * - Undefined Instruction
 * - SWI aus was anderem als dem User-Modus
 *
 * Je nach Ursprung des Absturzes wird entweder der aktuelle Thread beendet
 * oder das System angehalten. Zusätzlich gibt es einen Haufen (hoffentlich
 * hilfreicher) Informationen zum Debugging.
 */
void _exception_fault(unsigned int regs[16], enum exception e)
{
  	enum psr spsr = get_spsr();
  	enum psr_mode mode = spsr & PSR_MODE; 
  	const char *what; 
 
 	if (mode == PSR_USR) {
 		printf("\n===========================================================================\n\n");
 		what = "User-Thread"; 
 	} else {
 		printf("\n###########################################################################\n\n");
 		what = "Kernel";
 	}
 	printf("%s abgestuerzt!\n\n%s an Adresse %p!\n", what, exception_names[e], (void *)regs[15]); 
 
 	if (e == E_DATAABORT)
 		printf("Versuchter Zugriff auf Adresse %p.\n\n", mc_get_abort_address()); 
 
 	if (e == E_UNDEF || e == E_SWI)
 		printf("Datenwort an eben dieser Adresse: %p\n\n", (void *)*(unsigned int *)regs[15]); 
 
 	if (mode == PSR_USR) {
 		print_thread_info(regs, spsr); 
 		printf("\n===========================================================================\n\n");
  		end_current_thread(); 
 	} else {
 		print_exception_info(regs);
 		stop_execution();
 	}
}
 
/*
 * _exception_swi() - Software-Interrupt-Handler
 *
 * @regs: Register des Usermodus
 *
 * Derzeit einzige Funktion: Beenden von User-Threads.
 */
void _exception_swi(unsigned int regs[16])
{
  	enum psr_mode mode = get_spsr() & PSR_MODE; 
 
 	if(mode == PSR_USR) {
 		/* SWI aus User-Modus aus ist gewollt: beende den Thread */ 
  		end_current_thread(); 
 	} else {
 		if (regs[11] == 0xde00)
  			asm ("mov r11, #0; swi 0");
 		/*
 		 * SWI durch Betriebssystem-Code ist ungewollt. Der PC zeigt
 		 * momentan auf die Instruktion nach dem SWI; für die Fehler-
 		 * ausgabe ändern wir das.
 		 */
 		regs[15] -= 4;
 		_exception_fault(regs, E_SWI); 
 	}
}
