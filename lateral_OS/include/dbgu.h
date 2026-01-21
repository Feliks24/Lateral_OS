#ifndef _DBGU_H_
#define _DBGU_H_
 
void dbgu_init(void); 
void dbgu_putc(char c); 
char dbgu_getc(void); 
void dbgu_handle_irq(void); 
 
#endif /* _DBGU_H_ */
