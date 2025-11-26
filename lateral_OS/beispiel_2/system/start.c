#include <system.h>
#include <lib.h>
#include <dbgu.h>
 
/*
 * start_kernel() - unser C-Eintrittspunkt
 *
 * Wird direkt nach Initialisierung des Supervisor-Stacks gestartet.
 */
void start_kernel(void)
{
 	/*
 	 * System initialisieren.
 	 */
  	init_other_stacks(); 
  	init_exceptions();
 
 	/*
 	 * Test-Programm vorübergehend hier, bis es durch etwas richtiges
 	 * ersetzt wird. :)
 	 */
 	printf("Test der Ausnahmebehandlung:\n"
 	       "Welche Ausnahme soll ausgeloest werden?\n"
 	       "1) Software Interrupt\n"
 	       "2) Data Abort\n"
 	       "3) Undefined Instruction\n"
 	       "> ");
 
 	while (1) {
 		char c = dbgu_getc(); 
 
 		switch(c) { 
 		case '1':
 			/*
 			 * Software Interrupt (siehe Kap. A4.1.107 im ARM
 			 * Architecture Reference Manual)
 			 */
  			asm ("swi #0");
  			break; 
 		case '2':
 			/* Data Abort (siehe Kap. 8 im AT91RM9200 Handbuch) */ 
 			*(int *)0xa0000000 = 0;
  			break; 
 		case '3':
 			/*
 			 * Undefined Instruction (aus dem "Architecturally
 			 * Undefined Instruction space", siehe Kap. A3.16.5 im
 			 * ARM Architecture Reference Manual)
 			 */
  			asm (".word 0x07F000F0"); 
  			break; 
 		default:
  			continue;
 		}
 
 		printf("\n"
 		       "Unerwartete Rückkehr?! (Data Abort geht unter QEMU nicht.)\n"
 		       "Was darf es jetzt sein?\n"
 		       "> ");
 	}
}
