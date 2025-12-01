#ifndef _MC_H_
#define _MC_H_

void init_stack(void);
void set_ivt(void);

void mc_remap(void);
void *mc_get_abort_address(void);

#endif
