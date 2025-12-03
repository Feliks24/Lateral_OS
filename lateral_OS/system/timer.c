
/*
 all sources will be realted to manual 701

 general info about clocks page 267
 CKGR_MOR register page 283
 PMC_SR register page 291
 CKGR_MOR and PMC_SR are both in PMC
 
 write MOSCEN into CKGR_MOR to activate oscillator
 set OSCOUNTER for countdown 
 check MOSCS in PMC_SR where 1 -> stable


*/
#define PMC_BASE 0xFFFFFC00
#define CKGR_MOR 0x0020
#define PMC_SR 0x0068

//write
#define MOSCEN (1<<0)

//read
#define MOSCS (1<<0)

//127 x 8 cycles which should be about 31ms
//as 255 is 62ms as said in manual
#define OSCOUNT 8
#define OSCOUNT_VALUE 0b01111111
#define OSCOUNT_MASK 0xFF00



void activate_OSC(void){
	//set OSCOUNT
	//get values
	
	volatile unsigned int *ckgr_mor_register = (volatile unsigned int *)(PMC_BASE+CKGR_MOR);
	//extract first 8 bits
	unsigned int masked_value = *ckgr_mor_register & (~OSCOUNT_MASK);
	//combined first 8 bits with what we want to write into OSCOUNT
	unsigned int final_value = masked_value | (OSCOUNT_VALUE<<OSCOUNT);
	//also set MOSCEN bit
	final_value = final_value | MOSCEN;
	//write
	*ckgr_mor_register = final_value;

	while(!(*(volatile unsigned int *)(PMC_BASE+PMC_SR) & MOSCS)){};
	
}


void set_timer(unsigned int frequency){
	(void) frequency;
	activate_OSC();

}
