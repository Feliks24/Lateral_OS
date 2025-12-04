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
void _sleep(void);


 
#endif /* _LIB_H_ */
 
