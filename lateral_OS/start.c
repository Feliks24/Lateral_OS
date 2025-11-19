#include "debug_utils.h"
#include "remap_utils.h"
#include "set_ivt.h"
#include "init_stack.h"

// TASK 01
/*
__attribute__((naked, section(".init"))) void _start(void)
{
	// start point of the system

	unsigned int char1 = 97; //352 mod 255 = 97
	unsigned int char2 = 353; //352 mod 255 = 97

	pprintf("97 to char: %c\n353 to char: %c\n",char1, char2);
	const char* our_string = "testing";

	pprintf("hex of 97: %x\n",char1);
	pprintf("check \\% and also %\n");

	while(1){
		char c = char_get();
		pprintf("Read input: %c\n", c);
	}
}
*/

// TASK 02
__attribute__((naked, section(".init"))) void _start(void)
{
	// step 1: remap memory
	memory_remap();

	// step 2: IVT
	set_ivt();

	// step 3: stack
	init_stack();


	const char* our_string = "testing";
	pprintf("this string: %s\n", our_string);

	// step 4: test handler
	asm("swi #0");

	for (;;)
	{
		;
	}
}
