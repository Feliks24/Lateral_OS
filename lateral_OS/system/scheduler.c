#include <lib.h>
#include <system.h>



void null_thread(){
	while(1);
}

void dummy_thread(char character, unsigned int n){
	while(n>0){
		_sleep(5);
		printf("%c", character);
		n--;
	}
}



void init_tcbs(){

	int i;
	for(i=0; i < THREAD_NUM; i++){
		all_tcbs[i].thread_id=i; 
		all_tcbs[i].status = 0;
		all_tcbs[i].priv_SP = &thread_stacks[i][STACK_SIZE_PRIV];
	}

	//init first null thread
	unsigned int *stk = &all_tcbs[0].priv_SP[STACK_SIZE_PRIV-1];
	
	//void  *(*foo)(void) = &null_thread;
	*(--stk) = (unsigned int)&null_thread; 

	//LR stays empty because doesn't return
    	*(--stk) = (unsigned int)0; 

	//r13 to r0
	int j;
    	for(j=0; j < 15; j++){
        	*(--stk) = 0; 
	}
    	
	//set SPSR with I bit on 0
    	*(--stk) = 0x10; 
	running_threads = 1;

}

void init_thread(char character, unsigned int n){
	if(running_threads >= THREAD_NUM){
		printf("ERROR: too many threads");
		return;
	}
	
	//first find a place in tcb
	int i;
	struct tcb *my_tcb = NULL;
	for(i=0; i<THREAD_NUM; i++){
		if(all_tcbs[i].status){
			continue;
		}
		//found empty
		my_tcb = &all_tcbs[i];
		break;
	}

	if(my_tcb == NULL){
		printf("ERROR: empty tcb not fond");
		return;
	}
	my_tcb->status = 2;
	

	unsigned int *stk = &my_tcb->priv_SP[STACK_SIZE_PRIV-1];
	
	//void  *(*foo)(void) = &null_thread;
	*(--stk) = (unsigned int)&dummy_thread; 

	//LR stays empty because doesn't return
    	*(--stk) = (unsigned int)0; 

	//r13 to r2
	int j;
    	for(j=0; j < 13; j++){
        	*(--stk) = 0; 
	}
	//second input in r1
	*(--stk) = (unsigned int) n;
	//first input in r0
	*(--stk) = (unsigned int) character;
	
    	
	//set SPSR with I bit on 0
    	*(--stk) = 0x10;
}

struct tcb *reschedule(){
	//for now just always go next
	int i;
	for(i=1; i<THREAD_NUM+1; i++){
		if(all_tcbs[(i+current->thread_id)%THREAD_NUM].status == 2){
			unsigned int temp = (((unsigned int)i+current->thread_id)%THREAD_NUM);
			current = &all_tcbs[temp];
			return current;
		}
	}
	printf("Error finding reading thread in scheduling");
	return NULL;
}

