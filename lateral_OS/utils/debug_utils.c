#include <stdarg.h>

#define DBGU_REGISTER 0xFFFFF200
#define DBGU_RHR 0x0018
#define DBGU_THR 0x001C
#define DBGU_SR 0x0014

#define TXEN 6
#define ENABLE_THR (0x1 << TXEN)

static inline void write_u32(unsigned int addr, unsigned int val)
{
	*(volatile unsigned int *)addr = val;
}

void _sleep(int test)
{
	(void)test;
	volatile long i;
	for (i = 0; i < 100000000; i++)
		;
}

void char_put(char _char)
{

	// check if TXRDY is set at DBGU_SR
	while (!(*(volatile int *)(DBGU_REGISTER + DBGU_SR) & (0x1 << 1)))
	{
	};
	// write to the THR registry of the DBGU
	write_u32(DBGU_REGISTER + DBGU_THR, _char);
}

char char_get(void)
{
	// check if RXRDY is set at DBGU_SR
	while (!(*(volatile int *)(DBGU_REGISTER + DBGU_SR) & 0x1))
	{
	};
	// RHR -> receive holding register
	return *((volatile char *)(DBGU_REGISTER + DBGU_RHR));
}

void int_to_hex(unsigned int num)
{

	const char *HEX_DIGITS = "0123456789ABCDEF";
	char buffer[8];
	int i = 0;

	while (num && i < 8)
	{
		// take last bits
		unsigned int nibble = num & 0xF;
		// look up hex
		buffer[i] = HEX_DIGITS[nibble];
		// shift to next bits
		num = num >> 4;
		// loop over chars in string
		i++;
	}

	// start with 0x
	char_put('0');
	char_put('x');
	// print buffer in correct order
	while (i > 0)
	{
		i--;
		char_put(buffer[i]);
	}
}

void arg_to_int(int num)
{
	//max 10 digits + (-)
	char buffer[11];
	int i = 0;
	unsigned int u;

	if (num == 0)
	{
		char_put('0');
		return;
	}
	else if (num < 0)
	{
		char_put('-');
		u = (unsigned int)(-num);
	}
	else
	{
		u = (unsigned int)num;
	}
	while (u > 0)
	{
		// get last digit (%10) and convert to char
		// + 48 --> ascii '0'
		buffer[i++] = '0' + (u % 10);
		u /= 10;
	}

	while (i--)
		char_put(buffer[i]);
}

__attribute__((format(printf, 1, 2))) int lprintf(char *fmt, ...)
{

	// va_list && va_start && va_end --> brauchen hier eigene Speicheradressen
	va_list args;
	va_start(args, fmt);

	// point to string
	char *str = fmt;
	int count = 0;

	while (*str)
	{
		// flags set
		if (*str == '%')
		{
			// if %% print % and skip next one
			if (*(str + 1) == '%')
			{
				char_put('%');
				count++;
				str += 2;
				continue;
			}
			str++;

			if (!*str)
			{
				char_put('%');
				break;
			}

			switch (*str)
			{
			case 'c':
				// just put char
				char_put((char)va_arg(args, unsigned int));
				break;
			case 's':; // gcc has an issue with some label mmmm
				// loop over chars in string
				char *our_string = va_arg(args, const char *);

				while (*our_string)
				{
					char_put((char)*our_string);
					our_string++;
				}
				break;
			case 'x':
				// unsinged int to hexadecimal
				int_to_hex(va_arg(args, unsigned int));
				break;

			case 'd':
				// unsinged int
				arg_to_int(va_arg(args, unsigned int));
				break;
			case 'p':
				// assuming both unsigned int and void * have 4bytes
				int_to_hex(va_arg(args, unsigned int));
				break;
			default:
				// edge ignore, handel as char
				char_put('%');
				char_put(*str);
				count += 2;
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
