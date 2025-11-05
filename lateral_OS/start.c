#include "debug_utils.h"


__attribute__((naked, section(".init")))
void _start(void) {
	//start point of the system

	char_put('t');
	while(1){
		char c = char_get();
		char_put(c);
	}
}
