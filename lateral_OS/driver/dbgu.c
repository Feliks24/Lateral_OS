#include <lib.h>
#include <aic.h>
 
/*
 * Hardware-Interface zur Debug-Unit
 *
 * Alle Quellenangaben beziehen sich auf das Handbuch
 * zum Atmel AT91RM9200 (Revision 1768I).
 */
 
/* Adresse der Debug-Schnittstelle (Abb. 8-1) */ 
#define DBGU_BASE 0xFFFFF200
 
/* genutzte Register der Debug-Unit (Tab. 26-2) */ 
struct dbgu { 
 	unsigned int unused0[2]; 
 	unsigned int ier;
 	unsigned int unused1[2]; 
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
 
 
/*
 * Ringpuffer für empfangende Zeichen
 *
 * Wir verwenden einen Puffer der Größe 2^x. Das erlaubt es uns, komplett auf
 * Rücksetzlogik von HEAD und TAIL zu verzichten:
 *
 * - Zugriffe auf den Puffer erfolgen immer nur mit den untersten Bits von HEAD
 *   und TAIL. (Deswegen 2^x)
 *
 * - Der Puffer ist leer, wenn HEAD und TAIL identisch sind.
 *
 * - Der Puffer ist voll, wenn HEAD weit genug vor TAIL ist. (Dieser Test
 *   funktioniert insbesondere auch, wenn HEAD schon übergelaufen ist und TAIL
 *   noch nicht.)
 *
 * Zum Testen werden HEAD und TAIL so vorinitialisiert, dass der Überlauf schon
 * nach 8 Zeichen auftritt.
 *
 * Bei vollem Puffer werden die empfangenden Zeichen verworfen.
 */
 
#define RX_BUFFSHIFT 5
#define RX_BUFFSIZE (1 << RX_BUFFSHIFT)
#define RX_BUFFMASK (RX_BUFFSIZE - 1)
 
static char rx_buff[RX_BUFFSIZE]; 
 
static unsigned int rx_head = 0xfffffff8; 
static unsigned int rx_tail = 0xfffffff8; 
 
/*
 * rx_buff_putc() - Zeichen in Empfangspuffer schreiben
 *
 * @c: zu schreibendes Zeichen.
 *
 * Zeichen wird kommentarlos verworfen, falls Puffer voll.
 *
 * Es wird davon ausgegangen, dass diese Funktion während eines Interrupts
 * aufgerufen wird und dadurch nicht unterbrochen werden kann. (Und auch nicht
 * mehrfach gleichzeitig aufgerufen wird.)
 */
static void rx_buff_putc(char c)
{
#if 0
 	/* Für Debug-Zwecke */ 
 	printf("%s: RX_HEAD = %x, RX_TAIL = %x, RX_HEAD - RX_TAIL = %x\n",
 	       __func__, rx_head, rx_tail, rx_head - rx_tail); 
#endif
 
 	BUG_ON(!irq_disabled()); 
 	BUG_ON(rx_head - rx_tail > RX_BUFFSIZE); 
 
 	/* Raus, falls Puffer voll */ 
 	if (rx_head - rx_tail >= RX_BUFFSIZE)
 		return;
 
 	/* Zeichen schreiben und produzieren. */ 
 	rx_buff[rx_head & RX_BUFFMASK] = c; 
 	rx_head++; 
}
 
/*
 * rx_buff_getc() - Zeichen aus Empfangspuffer lesen
 *
 * Darf nur aufgerufen werden, wenn auch ein Zeichen im Empfangspuffer ist.
 * Außerdem wird davon ausgegangen, dass Interrupts abgeschaltet sind, so dass
 * wir exklusiven Zugriff auf den Puffer haben.
 */
static char rx_buff_getc(void)
{
 	char c; 
 
 	BUG_ON(!irq_disabled()); 
 	BUG_ON(rx_head - rx_tail > RX_BUFFSIZE); 
 	BUG_ON(rx_head - rx_tail == 0); 
 
 	/* Zeichen lesen und konsumieren */ 
 	c = rx_buff[rx_tail & RX_BUFFMASK]; 
 	rx_tail++;
 
 	return c;
}
 
 
/*
 * Öffentliches Interface
 */
 
 
/*
 * dbgu_init() - Debug-Unit initialisieren
 */
void dbgu_init(void)
{
 	/* Interrupt bei Zeichenempfang aktivieren */ 
 	dbgu->ier = RXRDY; 
 
 	/* Beim AIC Bescheid sagen */ 
 	aic_enable_irq(SYSIRQ); 
}
 
/*
 * dbgu_putc() - Zeichen schreiben (Kap. 26.4.3)
 *
 * @c: zu schreibendes Zeichen
 */
void dbgu_putc(char c)
{
 	/* Busy-Wait bis Transmit Holding Register frei */ 
  	do {} while (!(dbgu->sr & TXRDY)); 
 	dbgu->thr = c; 
}
 
/*
 * dbgu_getc() - Zeichen lesen (Kap. 26.4.2)
 */
char dbgu_getc(void)
{
 	/* Lesen aus dem Empfangspuffer */ 
 	return rx_buff_getc(); 
}
 
/*
 * dbgu_handle_irq() - Interrupt behandeln
 */
void dbgu_handle_irq(void)
{
 	unsigned int sr = dbgu->sr; 
 
 	/* Zeichen empfangen? */ 
 	if (sr & RXRDY) {
 		rx_buff_putc(dbgu->rhr); 
 		wakeup_charwait(); 
 	}
}
 
/*
 * dbgu_has_char() - Test auf Zeichen im Eingabepuffer
 *
 * Wenn != 0, dann darf dbgu_getc() aufgerufen werden.
 */
int dbgu_has_char(void)
{
 	return rx_head != rx_tail; 
}
