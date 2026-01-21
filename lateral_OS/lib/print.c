#include <stdarg.h>
#include <dbgu.h>
 
/*
 * val in hexadezimaler Darstellung ausgeben, auf Wunsch mit führendem
 * "0x" (do_prefix) und mit mindestens width Ziffern (bis zu 8).
 */
static void print_hex(unsigned int val, int do_prefix, int width)
{
 	char *hex_mask = "0123456789abcdef"; 
  	int i;
 
 	if (do_prefix) {
 		dbgu_putc('0'); 
 		dbgu_putc('x'); 
 	}
 
 	/* Nibble von links nach rechts durchgehen: 0x76543210 */ 
  	for (i = 7; i >= 0; i--) {
 		unsigned int nibble = (val >> (4 * i)) & 0xf; 
 
 		/* ggf. führende Nullen überspringen */ 
 		if (!nibble && i >= width)
  			continue;
 
 		dbgu_putc(hex_mask[nibble]); 
 
 		/* nach erster Ausgabe keine weiteren Nullen ignorieren */ 
 		width = 8;
 	}
}
 
static void print_string(char *s)
{
 	while (*s)
 		dbgu_putc(*s++); 
}
 
__attribute__ ((format(printf,1,2)))
void printf(char *format, ...)
{
 	va_list ap; 
 	va_start(ap, format); 
 
 	while (*format) {
 		/* Normales Zeichen? */ 
 		if (*format != '%') {
 			dbgu_putc(*format++); 
  			continue;
 		}
 
 		/* %-Zeichen gefunden, jetzt nächstes Zeichen interpretieren */ 
  		format++;
 
 		/* Formatstring unerwartet zu Ende? */ 
 		if (!*format) {
 			dbgu_putc('%'); 
  			break; 
 		}
 
 		switch (*format) { 
 		case 'c':
 			dbgu_putc(va_arg(ap, unsigned int)); 
  			break; 
 		case 's':
 			print_string(va_arg(ap, char*)); 
  			break; 
 		case 'p':
 			/* Pointer wird direkt als Zahl vom Stack gelesen */ 
 			print_hex(va_arg(ap, unsigned int), 1, 8); 
  			break; 
 		case 'x':
 			print_hex(va_arg(ap, unsigned int), 1, 1); 
  			break; 
 		case '%':
 			dbgu_putc('%'); 
  			break; 
 		default:
 			/* Unbekannte Formate einfach ausgeben */ 
 			dbgu_putc('%'); 
 			dbgu_putc(*format); 
 		}
 
  		format++;
 	}
 
 	va_end(ap); 
}
