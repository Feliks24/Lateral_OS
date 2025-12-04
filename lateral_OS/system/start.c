#include <system.h>
#include <lib.h>
#include <dbgu.h>

void start_kernel(void)
{
	print_title();
	/*
	 * System initialisieren.
	 */
	init_other_stacks();
	init_exceptions();
	dbgu_init();

	// !! HIER TIMER FREQUENZ EINSTELLEN !!
	set_timer(32768);

	printf("System bereit! Druecke Tasten!\n");
	printf("Timer-Interrupts: ! (alle 1 s)\n");
	printf("Tastendruck: Zeichen wird 20x ausgegeben\n\n");

	while (1)
	{
		char c = dbgu_getc();

		int i;
		for (i = 0; i < 20; i++)
		{
			dbgu_putc(c);
			_sleep();
		}
	}
}
