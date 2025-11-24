
#include <stdio.h>
#include <string.h>

#include "utils.h"
#include "system.h"

// is after remaping SRAM
#define IVT_START 0x00000000

extern char _interrupt_table[];
extern char _interrupt_table_end[];

// Exception typs
typedef enum
{
	RESET = 0,
	UNDEFINED = 1,
	SOFTWARE_INTERRUPT = 2,
	PREFETCH_ABORT = 3,
	DATA_ABORT = 4,
	IRQ = 5,
	FIQ = 6,
	EXCEPTION_COUNT
} Exception;

static const char *exception_names[] = {
	"RESET",
	"UNDEFINED",
	"SOFTWARE_INTERRUPT",
	"PREFETCH_ABORT",
	"DATA_ABORT",
	"IRQ",
	"FIQ"};

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

void _handel_exceptions(int ex_i)
{
	const char *ex_name = exception_names[ex_i];

	lprintf("----------------------------------------");
	lprintf("detected an exception: %s (%d)", ex_name, ex_i);
	lprintf("----------------------------------------");

		switch (ex_i)
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
	lprintf("OS stoped ");
}
