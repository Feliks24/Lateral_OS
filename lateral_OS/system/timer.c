
/*
 enable interrupt by writing PITS bit in ST_SR
 set frequency PIV (16 bits) in ST_PIMR

 255 cycles is roughly 60ms(62 per manual)
 max PIV can be programmed to 65536 as per manual into PIV
 as 65536 = 2^16
 so 65536 = 256*32
 therefore max timer is 62ms*32=1984
 therefore max frequency is roughly 2 seconds

*/
#include <stdint.h>
#include <lib.h>
#include <dbgu.h>

#define ST_BASE 0xFFFFFD00

#define ST_SR 0x0010
#define ST_IER 0x0014
#define ST_PIMR 0x0004

#define PITS 0
#define PIV 0

#define AIC_BASE 0xFFFFF00

#define AIC_IECR_OFFSET 0x120
#define IECR_ENABLE_MASK 0x4

void _cpsr_interrupt_enable(void);

void set_timer_interrupt(uint16_t frequency){
	//set timer
	*(volatile unsigned int *)(ST_BASE+ST_PIMR) = (frequency);
	//enable interrupt
	*(volatile unsigned int *)(ST_BASE+ST_IER) = (1<<PITS);

}

void set_aic_interrupt(void){
	*(volatile unsigned int *)(AIC_BASE+AIC_IECR_OFFSET)= IECR_ENABLE_MASK;
}

void check_interrupt(void){
	//test function to check for interrupts without actually handling
	uint16_t st_sr_bits = *(volatile unsigned int *)(ST_BASE+ST_SR);
	if(st_sr_bits & 1<<0){
		printf("PITS detected\n");
	}
	if(st_sr_bits & 1<<1){
		printf("WDOVF detected\n");
	}
	if(st_sr_bits & 1<<2){
		printf("RTTINC detected\n");
	}
	if(st_sr_bits & 1<<3){
		printf("ALMS detected\n");
	}
}





void set_timer(unsigned int frequency){

	set_timer_interrupt((uint16_t)frequency);
	dbgu_enable_interrupt();
	set_aic_interrupt();
}
