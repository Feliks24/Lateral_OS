#include <system.h>
#include <lib.h>
#include <dbgu.h>
 
/*
 * start_kernel() - unser C-Eintrittspunkt
 *
 * Wird direkt nach Initialisierung des Supervisor-Stacks gestartet.
 */
void start_kernel(void)
{

	print_title();
 	/*
 	 * System initialisieren.
 	 */
  	init_other_stacks(); 
  	init_exceptions();
 
 	


	set_timer(0xFFFF);

	

	//Test program
	while(1){
		char c = char_get();
		int i;
		for(i=0;i<10;i++){
			_sleep();
			printf("%c",c);
		}
		
	}

 	
}
