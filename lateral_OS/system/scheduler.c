#include <lib.h>
#include <stdio.h>
#include <stdlib.h>

struct tcb{
	struct tcb *next;
	/* this should be printed */
	char character;
	unsigned int n;
	unsigned int *priv_SP;
};

struct dynamic_tcbs{
	/* we just need to know what is
	currently running */
	struct tcb *current;
	struct tcb *prev;
	unsigned int num;
};

struct dynamic_tcbs runqueue;

void init_tcbs(){
	runqueue.current = NULL;	
	runqueue.prev = NULL;
	runqueue.num = 0;
	//maybe let a dummy thread run
}

struct tcb *make_thread(char character, struct tcb *next){
	/*TODO: do this with malloc*/
	struct tcb *new_tcb = malloc(sizeof(struct tcb));
	new_tcb->character = character;
	new_tcb->next = next;
	return new_tcb;
}

void add_thread(char character){
	

	runqueue.num += 1;

	if(runqueue.num == 1){
		struct tcb *new_tcb = make_thread(character,  NULL);
		/*cycle into itself since it's only one*/
		new_tcb->next = new_tcb;

		runqueue.current = new_tcb;
		runqueue.prev = new_tcb;
		return;

	} else {
		struct tcb *new_tcb = make_thread(character, runqueue.current->next);
		runqueue.current->next = new_tcb;
		return;
	}
}

void pop_thread(){
	if(runqueue.num == 0){
		printf("ERROR: removing thread while runqueue empty");
		return;
	} 

	runqueue.num -= 1;

	if(runqueue.num == 0){
		runqueue.prev->next = NULL;
		runqueue.current = NULL;
		//TODO: free memory
		return;
	}
	
	runqueue.prev->next = runqueue.current->next;
	runqueue.current = runqueue.current->next;
	// TODO: free memory when change to malloc
}

