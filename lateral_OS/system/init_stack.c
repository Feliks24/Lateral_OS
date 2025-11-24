#include <debug_utils.h>

#define STACK_SIZE 8192*4
#define STACK_START 0x00300000-(STACK_SIZE*8)

#define USR_BIT 0b10000
#define FIQ_BIT 0b10001
#define IRQ_BIT 0b10010
#define SVC_BIT 0b10011
#define ABT_BIT 0b10111
#define UND_BIT 0b11011
#define SYS_BIT 0b11111


void _init_stack(unsigned int mode, unsigned int address); 



void singular_stack_set(unsigned int mode,void * address)
{
	//r0 -> mode
	//r1 -> address


	asm volatile(
		"mrs r2, cpsr\n"
		"bic r3,r2, #0x1f\n"
		"orr r3,r3,%0\n" 
		"msr cpsr_c, r3\n"
		"mov sp, %1\n" 
		"msr cpsr_c, r2\n"
		"mov pc, lr\n"
		:: "r" (mode), "r" (address)
	);

}

void init_stack(void)
{
	
	int modes[] ={FIQ_BIT, IRQ_BIT, SVC_BIT, ABT_BIT,UND_BIT,SYS_BIT};
	unsigned int i;
	for(i=0; i<6; i++){
		_init_stack(modes[i],(unsigned int) STACK_START +i*STACK_SIZE);
		lprintf("stack set for mode %p\n", modes[i]);
	}
	lprintf("end of stack init");
	return;
}


