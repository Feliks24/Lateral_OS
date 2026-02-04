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
 
