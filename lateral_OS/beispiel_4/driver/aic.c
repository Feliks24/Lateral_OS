#include <lib.h>
 
/*
 * Hardware-Interface zum AIC
 */
 
#define AIC_BASE 0xFFFFF000
 
struct aic { 
 	unsigned int smr[32];
 	unsigned int unused0[32 + 8]; 
 	unsigned int iecr; 
};
 
static volatile struct aic * const aic = (struct aic *)AIC_BASE; 
 
 
/*
 * aic_enable_irq() - IRQ am AIC aktivieren
 *
 * @irq: zu aktivierender Interrupt
 */
void aic_enable_irq(int irq)
{
 	BUG_ON(irq >= 32);
 
 	/* IRQ auf Level-sensitive und Priorität 0 einstellen */ 
 	aic->smr[irq] = 0; 
 
 	/* IRQ anschalten */ 
 	aic->iecr = 1 << irq; 
}
