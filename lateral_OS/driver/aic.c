#include "lib.h"
#include "dbgu.h"

#define AIC_BASE_ADDR 0xFFFFF000
#define ST_SR (*(volatile unsigned int *)(0xFFFFFD00 + 0x0010))

// ? READ/WRITE
// Source Mode Register 0 to 31
#define AIC_SMR(n) (*(volatile unsigned int *)(AIC_BASE_ADDR + ((n) * 0x4)))
// Source Vector Register 0 to 31
#define AIC_SVR(n) (*(volatile unsigned int *)(AIC_BASE_ADDR + 0x80 + ((n) * 0x4)))

// ? WRITE
// Interrupt Disable Command Register
#define AIC_IDCR (*(volatile unsigned int *)(AIC_BASE_ADDR + 0x124))
// Interupt Clear Command Register
#define AIC_ICCR (*(volatile unsigned int *)(AIC_BASE_ADDR + 0x128))
// Iterrupt Enable Command Register
#define AIC_IECR (*(volatile unsigned int *)(AIC_BASE_ADDR + 0x120))

#define SYSIRQ_ID 1

void system_timer_irq_handler(void)
{
    volatile unsigned int status = ST_SR;

    if (status & (1 << 0))
    {
        printf("!\r\n");
    }
}

void set_aic_interrupt(void)
{
    // deactivate all interrupts
    AIC_IDCR = 0xFFFFFFFF;

    // clear all interrupts
    AIC_ICCR = 0xFFFFFFFF;

    AIC_SMR(SYSIRQ_ID) = 0x67;
    AIC_SVR(SYSIRQ_ID) = (unsigned int)system_timer_irq_handler;

    // Interrupt aktivateeee
    AIC_IECR = (1 << SYSIRQ_ID);
}