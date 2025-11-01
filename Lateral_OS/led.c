#define PIOB 0xfffff600
#define YELLOW_LED (1<<27)
#define PIO_PER 0x00
#define PIO_OER 0x10
#define PIO_SODR 0x30

//this apperently is needed to turn off the led
#define PIO_CODR 0x34


static inline
void write_u32 (unsigned int addr, unsigned int val) {
	*(volatile unsigned int *)addr = val;
}

void _sleep() {
		volatile long i;
		for(i = 0; i < 100000000;i++);
}

void yellow_on (void) {

	write_u32 (PIOB + PIO_PER, YELLOW_LED);
	write_u32 (PIOB + PIO_OER, YELLOW_LED);
	write_u32 (PIOB + PIO_SODR, YELLOW_LED);
	while(1){
		//turn off LED - sleep - turn on - sleep
		write_u32 (PIOB + PIO_CODR, YELLOW_LED);
		_sleep();
		write_u32 (PIOB + PIO_SODR, YELLOW_LED);
		_sleep();

	}

}
