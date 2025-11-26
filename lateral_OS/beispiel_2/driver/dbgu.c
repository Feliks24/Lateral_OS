/*
 * Alle Quellenangaben beziehen sich auf das Handbuch
 * zum Atmel AT91RM9200 (Revision 1768I).
 */
 
/* Adresse der Debug-Schnittstelle (Abb. 8-1) */ 
#define DBGU_BASE 0xFFFFF200
 
/* genutzte Register der Debug-Unit (Tab. 26-2) */ 
struct dbgu { 
 	unsigned int unused[5]; 
 	unsigned int sr;
 	unsigned int rhr;
 	unsigned int thr;
};
 
/* genutzte Bits des Statusregisters (Kap. 26.5.6) */ 
enum dbgu_status { 
 	RXRDY = 1 << 0,
 	TXRDY = 1 << 1,
};
 
static volatile struct dbgu * const dbgu = (struct dbgu *)DBGU_BASE; 
 
/* Zeichen schreiben (Kap. 26.4.3) */ 
void dbgu_putc(char chr)
{
  	do {} while (!(dbgu->sr & TXRDY)); 
 	dbgu->thr = chr; 
}
 
/* Zeichen lesen (Kap. 26.4.2) */ 
char dbgu_getc(void)
{
  	do {} while (!(dbgu->sr & RXRDY)); 
 	return dbgu->rhr; 
}
