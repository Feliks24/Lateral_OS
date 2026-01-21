#ifndef _LIB_H_
#define _LIB_H_
 
#include <stddef.h>
#include <system.h>
 
/* lib/mem.c */ 
void *memcpy(void *dest, const void *src, size_t n); 
 
/* lib/print.c */ 
__attribute__ ((format(printf,1,2)))
void printf(char *format, ...); 
 
 
 
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
 * BUG() und BUG_ON() - kritische Situationen prüfen und reagieren
 *
 * Beide Makros sind dem Linux-Vorbild nachempfunden.
 */
 
#define BUG() do { \
 	unsigned int regs[16]; \
  	asm ("stmia %0, {r0-r15}" :: "r" (regs) : "memory"); \
 	print_bug_info(regs, __func__, __FILE__, __LINE__); \
} while(0)
 
#define BUG_ON(cond) do { if (cond) BUG(); } while(0)
 
#endif /* _LIB_H_ */
 
