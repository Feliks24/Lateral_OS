#ifndef _LIB_H_
#define _LIB_H_
 
#include <stddef.h>
 
/* lib/mem.c */ 
void *memcpy(void *dest, void *src, size_t n); 
 
/* lib/print.c */ 
__attribute__ ((format(printf,1,2)))
void printf(char *format, ...); 
 
#endif /* _LIB_H_ */
 
