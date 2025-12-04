#ifndef _DBGU_H_
#define _DBGU_H_
 
void dbgu_putc(char chr); 
char dbgu_getc(void); 
void dbgu_init(void);
void dbgu_irq_handler(void);

 
#endif /* _DBGU_H_ */
