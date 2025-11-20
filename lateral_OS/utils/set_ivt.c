
#include <stdio.h>
#include <string.h>
#include <debug_utils.h>

#define IVT_START 0x00000000

extern char _interrupt_table[];
extern char _interrupt_table_end[];


void set_ivt(void)
{
	//write the interrupt table into memory
	char * ivt_memory_base = (char *)IVT_START;

	char * ivt_start = (char *)_interrupt_table;
	char * ivt_end = (char *)_interrupt_table_end;
	
	size_t ivt_size = ivt_end - ivt_start;

	pprintf("%p\n", (void *) ivt_size);
	size_t i;
	for(i = 0; i <= ivt_size; i++)
	{
		*ivt_memory_base = *ivt_start;
		ivt_memory_base++;
		ivt_start++;
	} 
	
}


void _general_exception()
{
	pprintf("detected an exception");
}
