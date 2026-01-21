#include <lib.h>

void print_call(char c){
	register unsigned int r_0_val asm("r0") = (unsigned int) c;
	asm volatile ("swi #1 \n": : "r" (r_0_val));
}

char getc_call(){
	//get char from r0
   	register unsigned int result asm("r0");
    	asm volatile ("swi #5 \n": "=r" (result):: "memory");
	return (char) result;
}

void kill_call(){
	asm("swi #3");
}

void make_call(char c){
	register unsigned int r_0_val asm("r0") = (unsigned int) c;
	asm volatile ("swi #4 \n": : "r" (r_0_val));
}

void wait_call(void){
	asm("swi #5");
}
