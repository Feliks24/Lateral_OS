#define TABLE_START 0x21000000


//TODO: temporary full rights
#define AP (0b11<<10) 

//domains
#define D0 (0b0<<5)
#define D1 (1b0<<5)

//1 C B 1 0
//set C and B 0
#define ENDBITS (0b10010<<0)


#define INT_PERI 0xF0000000
//entry for 0xF0000000
#define IP_L1_START 3840


void write_l1_table(){

	unsigned int *l1_table = (unsigned int *)TABLE_START;

	int i;
  
	//first set everything to 0
    	for (i = 0; i < 4096; i++) {
        	l1_table[i] = 0;
    	}
	
	//identity map from 0x0 - 0x3000 0000
	//plus 1 MB for l1 table

	for (i = 0; i < (768+1); i++) {
        	unsigned int phys = 0x0 + (i << 20);
		l1_table[i] = phys | AP | D0 | ENDBITS;
    	}

	//internal peripharels identity map
	//starting from 0xF000 0000 - 0xFFFF FFFF
	for (i = 0; i < 256; i++) {
        	unsigned int phys = INT_PERI + (i << 20);
		l1_table[i+IP_L1_START] = phys | AP | D0 | ENDBITS;
    	}


}



void mmu_init(){

	unsigned int value;

	//write l1 table into memory
	write_l1_table();



	//put address of l1 table into TTB
	value = TABLE_START;
	__asm__ volatile("mcr p15, 0, %0, c2, c0, 0" : :"r" (value));

	// set domain acces control
	// set first two domains to manager 11
	// TODO: maybe change them to client 01
	value = 0b1111;
	//domain 0 and domain 1 are now in manager
	__asm__ volatile("mcr p15, 0, %0, c3, c0, 0" : :"r" (value));

	//enable MMU
	//get register
	__asm__ volatile("mrc p15, 0, %0, c1, c0, 0" : "=r" (value));
	//turn on mmu, M(0) -> 1
	unsigned int mmu_enable_mask = 0b1;
	value = value | mmu_enable_mask;
	//disable cache, C(2), I(12) -> 0
	unsigned int cache_disable_mask = 0b0111111111011;
	value = value & cache_disable_mask; 
	//write back
	__asm__ volatile("mcr p15, 0, %0, c1, c0, 0" : :"r" (value));
	//mmu should be on
}
