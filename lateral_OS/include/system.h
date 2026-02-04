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
void print_thread_info(unsigned int regs[16], enum psr cpsr); 
void print_exception_info(unsigned int regs[16]); 
 
__attribute__ ((noreturn))
void print_bug_info(unsigned int regs[16], const char *func, const char *file, int line); 
 
__attribute__ ((noreturn))
void stop_execution(void); 
 
/* system/cpu.c */ 
void get_banked_sp_lr(enum psr_mode mode, unsigned int regs[2]); 
enum psr get_banked_spsr(enum psr_mode mode); 
void init_other_stacks(void); 
 
/* system/cpu_asm.S */ 
enum psr get_cpsr(void); 
enum psr get_spsr(void);
void set_spsr(enum psr spsr);
int calc_store_pc_offset(void); 
void disable_irq(void); 
void enable_irq(void); 
int irq_disabled(void); 
 
__attribute__ ((noreturn))
void idle(void); 
 
/* system/scheduler.c */ 
void scheduler_init(void); 
void schedule(unsigned int regs[16]); 
int start_new_thread(void (*entry)(void *), const void *arg, unsigned int arg_size); 
void end_current_thread(void); 
void thread_wait_char(void); 
void thread_wait_slices(unsigned int slices); 
void scheduler_tick(void); 
void wakeup_charwait(void); 

void mmu_init(void);
 
#endif /* _SYSTEM_H_ */
