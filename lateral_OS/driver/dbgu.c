/*
 * Alle Quellenangaben beziehen sich auf das Handbuch
 * zum Atmel AT91RM9200 (Revision 1768I).
 */

/* Adresse der Debug-Schnittstelle (Abb. 8-1) */
#define DBGU_BASE 0xFFFFF200

/* genutzte Register der Debug-Unit (Tab. 26-2) */
struct dbgu
{
	volatile unsigned int cr;
	volatile unsigned int mr;
	volatile unsigned int ier;
	volatile unsigned int idr;
	volatile unsigned int imr;
	volatile unsigned int sr;
	volatile unsigned int rhr;
	volatile unsigned int thr;
};

/* genutzte Bits des Statusregisters (Kap. 26.5.6) */
enum dbgu_status
{
	RXRDY = 1 << 0,
	TXRDY = 1 << 1,
};

static volatile struct dbgu *const dbgu = (struct dbgu *)DBGU_BASE;

/* Zeichen schreiben (Kap. 26.4.3) */
void dbgu_putc(char chr)
{
	do
	{
	} while (!(dbgu->sr & TXRDY));
	dbgu->thr = chr;
}

/* Zeichen lesen (Kap. 26.4.2) - Polling-Version */
char dbgu_getc(void)
{
	do
	{
	} while (!(dbgu->sr & RXRDY));
	return dbgu->rhr;
}

/* String ausgeben */
void dbgu_print(const char *str)
{
	while (*str)
	{
		if (*str == '\n')
			dbgu_putc('\r');
		dbgu_putc(*str++);
	}
}

// ring buffer for received characters
#define RX_BUFFER_SIZE 128
volatile char rx_buffer[RX_BUFFER_SIZE];
volatile unsigned int rx_write_pos = 0;
volatile unsigned int rx_read_pos = 0;

void dbgu_irq_handler(void)
{
	unsigned int status = dbgu->sr;

	if (status & (1 << 0))
	{
		char c = (char)(dbgu->rhr & 0xFF);

		// Store character in ring buffer
		unsigned int next_pos = (rx_write_pos + 1) % RX_BUFFER_SIZE;

		if (next_pos != rx_read_pos)
		{
			rx_buffer[rx_write_pos] = c;
			rx_write_pos = next_pos;
		}
	}
}

void dbgu_init(void)
{
	// Reset Receiver und Transmitter
	dbgu->cr = (1 << 2) | (1 << 3); // RSTRX | RSTTX
	// kofigure Mode Register
	dbgu->mr = 0x00000800;
	// Enable Receiver und Transmitter
	dbgu->cr = (1 << 4) | (1 << 6); // RXEN | TXEN
}
