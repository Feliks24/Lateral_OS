#include <system.h>
#include <lib.h>
#include <dbgu.h>
#include <st.h>
 
/*
 * start_kernel() - unser C-Eintrittspunkt
 *
 * Wird direkt nach Initialisierung des Supervisor-Stacks gestartet.
 */


void user_program(){
	while(1){
		char c = getc_call();
		make_call(c);
	}
}

void start_kernel(void)
{
 	/*
 	 * System initialisieren.
 	 */
  	init_other_stacks(); 
  	init_exceptions();
 
 	dbgu_init(); 
 
 	st_init();
 	st_set_interval(500);
 
 	scheduler_init(); 

	//start_new_thread(user_program, NULL, sizeof(NULL));
	
}
