#include <system.h>
#include <lib.h>
#include <dbgu.h>
#include <st.h>
 
/*
 * Schwache Referenz auf die main()-Funktion der Anwendung. Falls keine
 * Anwendung mitgelinkt wird, ist user_main 0.
 */
__attribute__ ((weakref("main"))) static void user_main(void *data); 
 
/*
 * start_kernel() - unser C-Eintrittspunkt
 *
 * Wird direkt nach Initialisierung des Supervisor-Stacks gestartet.
 */
void start_kernel(void)
{
 	/*
 	 * System initialisieren.
 	 */
  	init_other_stacks(); 
  	init_exceptions();
 
 	dbgu_init(); 
 	st_init();
 
 	scheduler_init(); 
 
 	/*
 	 * Initialen Thread starten.
 	 */
 	if (!user_main) {
 		printf("Kein initialer Thread!\n"); 
 		return;
 	}
 
 	if (start_new_thread(user_main, NULL, 0))
 		BUG();
}
