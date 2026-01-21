#include <lib.h>
 
/* Unser Thread beendet sich durch einen Software-Interrupt. */ 
__attribute__ ((noreturn))
void terminate(void)
{
  	asm("swi #0");
 	BUG();
}
 
/* Ein einfaches Testprogamm, was als Thread im Usermode ausgeführt werden soll */ 
void test_print_thread(void *x)
{
 	char c = *(char *)x; 
 	unsigned i;
 
  	for(i = 0; i < 64; i++) {
 		printf("%c", c); 
  		busy_wait(50000); 
 	}
 
#if 0
 	/* Das Programm zu Testzwecken abstürzen lassen */ 
 	if (c == 'D')
 		*(int *)0xa0000000 = 0;
 	else if (c == 'U')
  		asm (".word 0xE7F000F0"); 
 	else if (c == 'X')
  		asm ("mov pc, #0x04"); 
 	else if (c == 'B')
 		BUG();
#endif
}
