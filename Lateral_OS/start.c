#include "main.h"
__attribute__((naked, section(".init")))
void _start(void) {
	while(1){
		_sleep();
		yellow_on();
	}
}
