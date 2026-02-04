#ifndef _USERLIB_H_
#define _USERLIB_H_
 
/*
 * barrier() - Compiler Optimization Barrier
 *
 * Verhindert ohne weiteren Overhead, dass GCC irgendwelche Speicherstellen
 * über die Barriere hinweg in Registern zwischenspeichert.
 */
static inline void barrier(void)
{
  	asm("" ::: "memory");
}
 
/*
 * busy_wait() - Aktiv nichts tun
 *
 * @usec: Länge in Mikrosekunden
 *
 * Die Umrechnung von Mikrosekunden auf Schleifendurchläufe stimmt nur für
 * unsere Hardware und aktivierte Optimierungen: unsere CPU hat 180 MHz und
 * ein Schleifendurchlauf hat 2 Instruktionen, die 4 Takte benötigen.
 */
static inline void busy_wait(unsigned int usec)
{
 	unsigned int loops = usec * (180 / 4); 
 	while(loops--)
 		barrier(); 
}
 
 
 
/*
 * Syscall-C-Interface.
 *
 * Für das Warum und Wieso siehe user/syscalls.S
 */
 
 
 
/*
 * fork() - generiert einen neuen Thread im User-Modus
 *
 * @entry: Einsprungspunkt, bzw. die Funktion, die der Thread ausführen soll
 * @arg: Argument, was der Funktion übergeben wird
 *
 * Liefert 0 bei Erfolg und != 0 bei Fehler.
 */
int fork(void (*f)(void *), unsigned int arg); 
 
/*
 * wait() - legt den Thread schlafen
 *
 * @ticks: Anzahl der zu verschlafenden Ticks
 */
void wait(unsigned int ticks); 
 
/*
 * exit() - beendet den Thread
 */
__attribute__ ((noreturn))
void exit(void); 
 
/*
 * getchar() - liest ein Zeichen von der DBGU
 *
 * Thread wird schlafen gelegt, falls Puffer leer.
 *
 * Liefert das Zeichen zurück.
 */
char getchar(void); 
 
/*
 * putchar() - sendet ein Zeichen über die DBGU
 *
 * @c: zu sendendes Zeichen
 */
void putchar(char c); 
 
#endif /* _USERLIB_H_ */
 
