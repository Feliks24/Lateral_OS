#include <system.h>
#include <lib.h>
#include <dbgu.h>


void demo_thread(char character, unsigned int n){
	while(n > 0){
		_sleep(10);
		printf("%c", character);
		n -= 1;
	}
}

void start_kernel(void)
{
	print_title();
	/*
	 * System initialisieren.
	 */
	init_other_stacks();
	init_exceptions();
	dbgu_init();
	set_timer(32768);



	init_tcbs();

	demo_thread('A', 10);
	/*
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
	*/
	while(1);
}

