#include <stdint.h>
#include <lib.h>
#include <dbgu.h>
#include "aic.h"

// System Timer (ST) 
#define ST_BASE 0xFFFFFD00

// ? READ/WRITE
// System Timer Control Register
#define ST_CR (*(volatile unsigned int *)(ST_BASE + 0x0000))
// System Timer Periodic Interval Mode Register
#define ST_PIMR (*(volatile unsigned int *)(ST_BASE + 0x0004))
// System Timer Status Register
#define ST_SR (*(volatile unsigned int *)(ST_BASE + 0x0010))
// System Timer Interrupt Enable Register
#define ST_IER (*(volatile unsigned int *)(ST_BASE + 0x0014))

void set_timer_interrupt(uint16_t frequency)
{
    // set timer frequency
    ST_PIMR = frequency;
    // Reset timer
    ST_CR = 1;
    // Enable PITS interrupt
    ST_IER = (1 << 0);
}

// timer init
void set_timer(unsigned int frequency)
{
    set_timer_interrupt((uint16_t)frequency);
    // dbgu_enable_interrupt(); -> is now part of dbgu_init()
    set_aic_interrupt();
    _cpsr_interrupt_enable();
}