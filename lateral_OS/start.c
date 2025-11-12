#include "debug_utils.h"



__attribute__((naked, section(".init"))) void _start(void)
{
	// start point of the system


	unsigned int char1 = 97; //352 mod 255 = 97
	unsigned int char2 = 353; //352 mod 255 = 97

	my_printer("97 to char: %c\n 353 to char: %c\n",char1, char2);
	const char* our_string = "testing";

	my_printer("this string: %s\n", our_string);
	my_printer("hex of 97: %x\n",char1);

	while(1){
		char c = char_get();
		my_printer("Read input: %c\n", c);
	}
}


