#include "utils.h"
#include "system.h"
#include "cosmetic.h"

__attribute__((naked, section(".init"))) void _start(void)
{
	// cosmetic
	_print_titel();

	// -----------------------------------------------------------

	// main functionality

	// step 1: remap memory
	//memory_remap();

	// step 2: IVT
	set_ivt();

	// step 3: stack
	init_stack();

	// step 4: test handler
	lprintf("Choose a exception to test the functionality \n");
	lprintf("1.........................Software Interrupt \n");
	lprintf("2.................................Data Abort \n");
	lprintf("3......................Undefined Instruction \n");

	while (1)
	{
		char c = char_get();
		lprintf("Read input: %c\n", c);

		switch (c)
		{
		case '1':
			/*
			 * Software Interrupt (siehe Kap. A4.1.107 im ARM
			 * Architecture Reference Manual)
			 */
			asm("swi #0");
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
			asm(".word 0x07F000F0");
			break;
		default:
			continue;
		}

		lprintf("\n"
			   "Unerwartete Rückkehr?! (Data Abort geht unter QEMU nicht.)\n"
			   "Was darf es jetzt sein?\n"
			   "> ");
	}
}
