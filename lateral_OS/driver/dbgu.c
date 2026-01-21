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
 
 	/*
 	 * Zeichen schreiben und produzieren. Im Gegensatz zu rx_buff_getc()
 	 * ist die Reihenfolge hier egal, da wir nicht unterbrochen werden
 	 * können.
 	 */
 	rx_buff[rx_head & RX_BUFFMASK] = c; 
 	rx_head++; 
}
 
/*
 * rx_buff_getc() - Zeichen aus Empfangspuffer lesen
 *
 * Es wird auf ein Zeichen gewartet, falls Puffer leer. Das wiederum bedeutet,
 * dass diese Funktion nur bei angeschalteten Interrupts aufgerufen werden
 * darf, da sonst kein Zeichen ankommen kann.
 *
 * Mehrere gleichzeitige Aufrufe von rx_buff_getc() müssen verhindert werden.
 * Andernfalls könnten es mehrere Threads durch die while()-Schleife schaffen
 * und rx_tail an rx_head vorbeibewegen, womit der Puffer aufhört zu
 * funktionieren.
 */
static char rx_buff_getc(void)
{
 	char c; 
 
 	BUG_ON(irq_disabled()); 
 	BUG_ON(rx_head - rx_tail > RX_BUFFSIZE); 
 
 	/*
 	 * Blockieren solange Puffer leer. barrier() sorgt dafür, dass wir
 	 * tatsächlich jedesmal rx_head aus dem Speicher lesen.
 	 */
 	while (rx_head - rx_tail == 0)
 		barrier(); 
 
 	/*
 	 * Zeichen lesen und konsumieren. Die barrier() stellt sicher, dass das
 	 * Zeichen gelesen wird, bevor rx_tail erhöht wird. Wäre der Zugriff
 	 * umgekehrt (rx_tail setzen und dann von alter Position lesen), könnte
 	 * zwischendrin ein rx_buff_putc() das Zeichen bereits überschreiben.
 	 */
 	c = rx_buff[rx_tail & RX_BUFFMASK]; 
 	barrier(); 
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

//void dbgu_handle_irq(void){}

void dbgu_handle_irq(void)
{
 	unsigned int sr = dbgu->sr; 
 
#if 0
 	/* Zeichen empfangen? */ 
 	if (sr & RXRDY)
 		rx_buff_putc(dbgu->rhr); 
#else
 	/*
 	 * Ausnahme für Aufgabe 4: bei Zeichenempfang statt Zeichen puffern
 	 * direkt Thread starten
 	 */
 	if (sr & RXRDY) {
 		char c = dbgu->rhr; 
 
 		/*
 		 * Um den Compiler glücklich zu machen und den Sonderfall-Code
 		 * für diese Aufgabe nicht überall zu haben: CTRL+C puffern
 		 */
 		if (c == 3)
 			rx_buff_putc(dbgu->rhr); 
 
 	#if 0
 		/* Kernel zu Testzwecken abstürzen lassen */ 
 		if (c == 'd')
 			*(int *)0xa0000000 = 0;
 		else if (c == 'u')
  			asm (".word 0xE7F000F0"); 
 		else if (c == 'x')
  			asm ("mov pc, #0x04"); 
 		else if (c == 's')
  			asm ("swi 0");
 		else if (c == 'S')
  			asm ("mov r11, #0xde00; swi 0"); 
 	#endif
 
  		extern void test_print_thread(void* x); 
  		//extern void test_print_thread_swi(void* x); 

		if (start_new_thread(test_print_thread, &c, sizeof(c)))
 			printf("Limit reached - cannot start new thread\n"); 
 		else
 			request_reschedule(); 
		 		
 	}
#endif
}
