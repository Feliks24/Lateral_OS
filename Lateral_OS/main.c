#define DBGU_CR 0x0000
#define DBGU_THR 0x001C

#define TXEN 6
#define ENABLE_THR (0x1 << 6)



static inline
void write_u32 (unsigned int addr, unsigned int val) {
	*(volatile unsigned int *)addr = val;
}

void _sleep() {
		volatile long i;
		for(i = 0; i < 100000000;i++);
}

void yellow_on (void) {

	//enable thr
	write_u32 (DBGU_CR, ENABLE_THR);

	//write to thr
	write_u32(DBGU_THR,6);
	_sleep();
	while(1){
		write_u32(DBGU_THR,'hello');
	}

}
