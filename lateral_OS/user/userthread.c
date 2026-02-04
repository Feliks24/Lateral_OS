#include "userlib.h"
 
/*
 * print_string() - String via Syscall ausgeben
 *
 * Noch kein printf(), aber das braucht unsere Anwendung auch noch nicht
 * wirklich.
 */
static void print_string(char *s)
{
 	while (*s)
 		putchar(*s++); 
}
 
/*
 * char_repeater() - geforderter Thread für Aufgabe 5
 *
 * Zeichen wiederholt mit Pausen ausgeben. Bei Großbuchstaben wird in den
 * Pausen aktiv gewartet, bei allen anderen passiv.
 */
static void char_repeater(void *data)
{
 	unsigned int i;
 	char c = *(unsigned int *)data; 
 
  	for (i = 0; i < 10; i++) {
 		putchar(c); 
 		if (c >= 'A' && c <= 'Z')
  			busy_wait(500000); 
 		else
 			wait(10);
 	}
 
  	exit();
}
 
/*
 * main() - Hauptthread für Aufgabe 5
 *
 * Für jede Taste wird ein Thread geforkt.
 */
void main(void *data)
{
 	(void)data; /* Unbenutztes Argument */ 
 
 	while (1) {
 		char c = getchar(); 
 		if (fork(char_repeater, c))
 			print_string("Fehler bei Thread-Erzeugung!\n"); 
 	}
}
