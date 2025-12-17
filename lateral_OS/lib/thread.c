#include <lib.h>


void dummy_thread(char character, unsigned int n){
	while(n>0){
		_sleep(5);
		printf("%c", character);
		n--;
	}
}

void null_thread(){
	while(1)
}
