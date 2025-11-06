#include "debug_utils.h"

#define DBGU_REGISTER 0xFFFFF200
#define DBGU_SR 0x0014


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
		//check SR register of DBGU for RXRDY bit 
		if( *(volatile int *)(DBGU_REGISTER+DBGU_SR) & 0x1){
			//if bit is set read the DBGU_RHR into c
			char c = char_get();
			my_printer("Read input: %c\n", c);
		}
	}
}


