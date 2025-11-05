#include <stdarg.h>

#define DBGU_REGISTER 0xFFFFF200
#define DBGU_RHR 0x0018
#define DBGU_THR 0x001C

#define TXEN 6
#define ENABLE_THR (0x1 << TXEN)

static inline void write_u32(unsigned int addr, unsigned int val)
{
	*(volatile unsigned int *)addr = val;
}

void _sleep(int test)
{
	(void) test;
	volatile long i;
	for (i = 0; i < 100000000; i++)
		;
}


void char_put(char _char)
{
	
	// enable thr
	// this is apparently not necessary
	// write_u32 (DBGU_CR, ENABLE_THR);

	// write to the THR registry of the DBGU
	write_u32(DBGU_REGISTER + DBGU_THR, _char);
}

char char_get(void)
{
	// read the RHR register of DBGU unit
	return *((volatile char *)(DBGU_REGISTER + DBGU_RHR));
}


void int_to_hex(unsigned int num){

	const char *HEX_DIGITS = "0123456789ABCDEF";

	char_put('0');
	char_put('x');
	while(!num){
		// take last bits
		unsigned int nibble = num & 0xF;
		//look up hex
		char_put(HEX_DIGITS[nibble]);	
		//shift to next bits
		num = num >> 4;
	}

}



__attribute__((format(printf, 1, 2)))
int my_printer(char *fmt, ...)
{

	// va_list && va_start && va_end --> brauchen hier eigene Speicheradressen
	va_list args;
	va_start(args, fmt);

	//point to string
	char *str = fmt;
	int count = 0;

	while (*str)
	{
		// flags set
		// ? % && (startingadress || last char was not \ )
		if (*str == '%' && (str == fmt || *(str - 1) != '\\'))
		{
			str++;
			switch (*str)
			{
			case 'c':
				char_put((char) va_arg(args, unsigned int));
				break;
			case 's': 
				;
				char *our_string= va_arg(args, const char*);

				while(*our_string){
					char_put((char)*our_string);
					our_string++;
				}
				break;
			case 'x':
				//unsinged int to hexadecimal
				int_to_hex(va_arg(args, unsigned int));
				break;
			case 'p':
				break;
			default:
				// edge ignore, handel as char
				char_put(*str);
				count++;
				break;
			}
		}
		// regular char
		else
		{
			char_put(*str);
			count++;
		}
		str++;
	}

	va_end(args);
	return count;
}
