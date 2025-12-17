#ifndef _LIB_H_
#define _LIB_H_
 
#include <stddef.h>
 
/* lib/mem.c */ 
void *memcpy(void *dest, void *src, size_t n); 
 

/* lib/print.c */ 
__attribute__ ((format(printf,1,2)))
void printf(char *format, ...); 

/* lib/boot_print.c */
void print_title(void);
void _sleep(volatile long n);

/* 
	lib/thread.c 
	one function for a thread that just prints char
		n times
	null_thread is the thread that will always be
		there and it just sleeps
*/
void dummy_thread(char character, unsigned int n);
void null_thread(void);

 
#endif /* _LIB_H_ */
 
