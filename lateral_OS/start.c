#include "debug_utils.h"




__attribute__((naked, section(".init"))) void _start(void)
{
	// start point of the system
	//char_put('t');


	unsigned int char1 = 97; //352 mod 255 = 97
	//unsigned int char2 = 353; //352 mod 255 = 97
	//unsigned int char3 = 353 + 256 +1;
	//theorie beides ist 'a'

	//%c works
	//my_printer("97 to char: %c\n 353 to char: %c\n %c ",char1, char2, char3);
	//const char* our_string = "testing";

	//my_printer("this string: %s, and this is a char: %c", our_string, char3);
	my_printer("test hex: %x",char1);
	while(1);
}


