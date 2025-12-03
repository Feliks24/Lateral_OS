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
 
 	/*
 	 * Test-Programm vorübergehend hier, bis es durch etwas richtiges
 	 * ersetzt wird. :)
 	 */


	//set_timer(0xFFFF);

	

	while(1){
		char c = char_get();
		int i;
		for(i=0;i<10;i++){
			_sleep();
			printf("%c",c);
		}

		set_timer(0xFFFF);
		
	}

 	
}
