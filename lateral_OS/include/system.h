#ifndef _SYSTEM_H_
#define _SYSTEM_H_
 
/* Prozessor-Statusregister (siehe ARM Architecture Reference Manual, A2.5) */ 
enum psr {
 	/* Bedingungsbits */ 
 	PSR_N = 1 << 31,
 	PSR_Z = 1 << 30,
 	PSR_C = 1 << 29,
 	PSR_V = 1 << 28,
 
 	/* Kontrollbits */
 	PSR_I = 1 << 7,
 	PSR_F = 1 << 6,
 	PSR_T = 1 << 5,
 	PSR_MODE = 0x1f,
};
 
enum psr_mode {
 	/* Modusbits aufgeschlüsselt */ 
 	PSR_USR = 0x10,
 	PSR_FIQ = 0x11,
 	PSR_IRQ = 0x12,
 	PSR_SVC = 0x13,
 	PSR_ABT = 0x17,
 	PSR_UND = 0x1b,
 	PSR_SYS = 0x1f,
};



/* system/exceptions.c */ 
void init_exceptions(void); 
 
/* system/info.c */
void print_debug_info(unsigned int regs[16]); 
void stop_execution(void); 
 
/* system/cpu.c */ 
void get_banked_sp_lr(enum psr_mode mode, unsigned int regs[2]); 
void init_other_stacks(void); 
 
/* system/cpu_asm.S */ 
enum psr get_cpsr(void); 
enum psr get_spsr(void);
int calc_store_pc_offset(void); 
 
void set_timer(unsigned int frequency);
void check_interrupt();





/*scheduler stuff start*/
#define THREAD_NUM 16
#define STACK_SIZE_PRIV 16
 
struct tcb{
        /* this should be printed */
        unsigned int *priv_SP;
        unsigned int thread_id;
        char character;
        unsigned int n;

        /*
                0 -> empty
                1 -> running
                2 -> ready
                3 -> finished
        */
        unsigned int status;
};


struct tcb *current;
unsigned int running_threads;
unsigned int thread_stacks[THREAD_NUM][STACK_SIZE_PRIV];
struct tcb all_tcbs[THREAD_NUM];
void init_tcbs();
//struct tcb make_thread(char character, unsigned int n, struct tcb *next);
//void add_thread(char character, unsigned int n);
//void pop_thread();


/* scheduler stuff end*/

#endif /* _SYSTEM_H_ */
