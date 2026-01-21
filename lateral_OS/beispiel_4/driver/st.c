#include <lib.h>
#include <aic.h>
#include <system.h>
 
/*
 * Hardware Interface zum System Timer
 */
 
#define ST_BASE 0xFFFFFD00
 
#define PITS (1 << 0)
 
struct st {
 	unsigned int unused0; 
 	unsigned int pimr;
 	unsigned int unused1[2]; 
 	unsigned int sr;
 	unsigned int ier;
};
 
static volatile struct st * const st = (struct st *)ST_BASE; 
 
 
/*
 * st_init() - System Timer initialisieren
 */
void st_init(void)
{
 	/* Interrupt für Period Interval Timer aktivieren */ 
 	st->ier = PITS;
 
 	/* Beim AIC Bescheid sagen */ 
 	aic_enable_irq(SYSIRQ); 
}
 
/*
 * st_set_interval() - Abstand zwischen Timer-Interrupts setzen
 *
 * @msec: Abstand in Millisekunden
 */
void st_set_interval(unsigned int msec)
{
 	BUG_ON(msec > 2000);
 	BUG_ON(msec == 0);
 
 	/* Registerinhalt aus Zeitübergabe berechnen */ 
 	if (msec >= 2000)
 		st->pimr = 0;
 	else
 		st->pimr = (32768 * msec) / 1000;
}
 
/*
 * st_handle_irq() - Timer-Interrupt behandeln
 */
void st_handle_irq(void)
{
 	unsigned int sr = st->sr;
 
 	if (sr & PITS) {
 		printf("!");
 		request_reschedule(); 
 	}
}
