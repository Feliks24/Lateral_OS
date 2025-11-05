#include "debug_utils.h"

__attribute__((naked, section(".init"))) void _start(void)
{
	test_01();
}

//testing differnt printInputs
void test_01()
{
	// start point of the system
	char_put('t');

	while (1)
	{
		char c = char_get();
		my_printer("Hello World!\n");
		my_printer("Zahl in Hex: %x\n", 255);
		my_printer("Pointer: %p\n", (void *)0xDEADBEEF);

		my_printer("einfacher String \\%p\n");
	}
}