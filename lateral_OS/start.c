#include "utils.h"
#include "system.h"
#include "cosmetic.h"

__attribute__((naked, section(".init")))
void _start(void)
{
	_print_titel();

	// step 1: remap memory
	memory_remap();

	// step 2: IVT
	set_ivt();

	// step 3: stack
	init_stack();

	// step 4: test handler
	//*(int *) 0x00400029 = 0;
	lprintf("now testing software interrupt");
	asm("swi #0");

	const char *our_string = "testing";
	lprintf("this string: %s\n", our_string);

	for (;;)
	{
		;
	}
}
