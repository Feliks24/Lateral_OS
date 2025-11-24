
#include <stdio.h>
#include <string.h>
#include <debug_utils.h>

#include "remap_utils.h"

#define IVT_START 0x00000000

extern char _interrupt_table[];
extern char _interrupt_table_end[];

enum Expection
{
	RESET,
	UNDEFINED,
	SOFTWARE_INTERUPT,
	PREFETCH_ABORT,
	DATA_ABORT,
	IRQ,
	FIQ
};

void set_ivt(void)
{
	memory_remap();

	// write the interrupt table into memory
	char *ivt_memory_base = (char *)IVT_START;

	char *ivt_start = (char *)_interrupt_table;
	char *ivt_end = (char *)_interrupt_table_end;

	size_t ivt_size = ivt_end - ivt_start;

	// lprintf("%p\n", (void *) ivt_size);
	size_t i;
	for (i = 0; i < ivt_size; i++)
	{
		*ivt_memory_base = *ivt_start;
		ivt_memory_base++;
		ivt_start++;
	}
}

void _stop_process(void)
{
	lprintf("prozess will be stopped");
	for (;;)
	{
		;
	}
}

static const char *exception_names[] = {
	"RESET",
	"UNDEFINED",
	"SOFTWARE_INTERRUPT",
	"PREFETCH_ABORT",
	"DATA_ABORT",
	"IRQ",
	"FIQ"};

void _general_exception(Expection ex)
{
	lprintf("----------------------------------------");
	lprintf("detected an exception: %s", exception_names[ex]);
	lprintf("----------------------------------------");

	switch (ex)
	{
	// will stop ongoing process
	case RESET:
	case UNDEFINED:
	case PREFETCH_ABORT:
	case DATA_ABORT:
	{

		_stop_process();
		break;
	}
	// will continue
	case FIQ:
	case IRQ:
		// ! if its os fault
		// case PREFETCH_ABORT:
		// case DATA_ABORT:
		{
			// TODO: Continue process
			break;
		}
	}
	lprintf("OS stoped ")
}
