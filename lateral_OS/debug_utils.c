#define DBGU_REGISTER 0xFFFFF200
#define DBGU_RHR 0x0018
#define DBGU_THR 0x001C

#define TXEN 6
#define ENABLE_THR (0x1 << TXEN)



static inline
void write_u32 (unsigned int addr, unsigned int val) {
	*(volatile unsigned int *)addr = val;
}

void _sleep() {
		volatile long i;
		for(i = 0; i < 100000000;i++);
}

void char_put (char _char) {
	//enable thr
	//this is apparently not necessary
	//write_u32 (DBGU_CR, ENABLE_THR);

	//write to the THR registry of the DBGU
	write_u32(DBGU_REGISTER + DBGU_THR,_char);

}

char char_get (void) {
	//read the RHR register of DBGU unit
	return (*((volatile char *)DBGU_REGISTER+DBGU_RHR));
}


