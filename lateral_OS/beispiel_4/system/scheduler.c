#include <system.h>
#include <list.h>
#include <lib.h>
#include <stddef.h>
#include <st.h>
#include "memlayout.h"
 
#define MAX_THREADS 16
#define SCHEDULER_TIMESLIZE_MS 500
 
enum thread_status { 
 	thread_finished = 0,
 	thread_ready,
 	thread_is_idle_process,
};
 
struct tcb { 
 	struct list_node node; 
 
 	/* Verwaltungsdaten */ 
 	unsigned thread_id; 
 	unsigned stack_bottom; 
 	unsigned stack_size; 
  	enum thread_status status; 
 
 	/* Kontext des Threads */ 
 	unsigned psr;
 	unsigned registers[16];
};
 
/* Makros um die Verwendung der Listenfunktionen zu vereinfachen */ 
#define LIST_BASE(X) ((struct list_node **)(&(X)))
#define LIST_NODE(X) (&((X).node))
#define TCB(X)       ((struct tcb *)(X))
 
#define IDLE	(&idle_thread)
 
/* reschedule_request = 1 veranlasst einen Prozesswechsel, falls möglich */ 
static unsigned reschedule_request; 
 
static struct tcb idle_thread; 
static struct tcb user_threads[MAX_THREADS]; 
 
/* Zeiger auf genau einen laufenden thread */ 
static struct tcb *running_thread   = NULL; 
 
/* Listen von Threads in dem jeweiligen Zustand */ 
static struct tcb *threads_ready    = NULL; 
static struct tcb *threads_finished = NULL; 
 
/* Initialisierung des Schedulers */ 
void scheduler_init(void)
{
  	int i;
 
 	idle_thread.status	  = thread_is_idle_process; 
 	idle_thread.registers[15] = (unsigned int)idle; 
 	idle_thread.psr		  = (unsigned int)PSR_SYS; 
 
  	for (i = 0; i < MAX_THREADS; i++) {
 		user_threads[i].thread_id	= i; 
 		user_threads[i].status		= thread_finished; 
 		user_threads[i].stack_bottom	= USER_STACK_BOTTOM - i * USER_STACK_SIZE; 
 		user_threads[i].stack_size	= USER_STACK_SIZE; 
 		list_push_back(LIST_BASE(threads_finished), LIST_NODE(user_threads[i])); 
 	}
 	request_reschedule(); 
}
 
void request_reschedule(void)
{
 	reschedule_request = 1; 
}
 
/*
 * schedule() - eine einfacher Round-Robin-Scheduler
 *
 * @regs: Die Inhalte der General-Purpose-Register des unterbrochenen Threads.
 *
 * Im Falle eines Kontextwechsels werden die Werte in regs gesichert und
 * mit denen eines anderen Threads ausgewechselt, sowie das SPSR-Register
 * des aktuellen Modus ausgetauscht.
 */
void schedule(unsigned int regs[16])
{
 	if (!reschedule_request)
 		return;
 
 	struct tcb *interrupted_thread = running_thread; 
 
 	if (interrupted_thread != NULL && interrupted_thread->status == thread_finished) { 
 		/* Thread wurde beendet */ 
 		running_thread = IDLE; 
 	}
 
 	if (threads_ready != NULL) { 
 		/* Falls andere Threads bereit sind einen neuen heraussuchen */ 
 		running_thread = TCB(list_remove(LIST_BASE(threads_ready),
 						LIST_NODE(*threads_ready))); 
 	} else if (interrupted_thread == NULL) { 
 		/*
 		 * Scheduler wurde zum ersten Mal aufgerufen, aber es gibt noch
 		 * keinen Thread zum Ausführen
 		 */
 		running_thread = IDLE; 
 	}
 
 	if (running_thread != interrupted_thread) { 
 		/* YAY, wir können einen Kontextwechsel machen :) */ 
 		printf("\n");
 
 		if (interrupted_thread != NULL && interrupted_thread != IDLE) { 
 			/* Alten Kontext sichern. Der Kontext vom IDLE Thread muss nicht gesichert werden. */ 
 			memcpy(interrupted_thread->registers, regs, sizeof(unsigned[16])); 
  			interrupted_thread->psr = get_spsr(); 
 
 			switch(interrupted_thread->status) { 
 			case thread_finished:
 				list_push_back(LIST_BASE(threads_finished),
 					       LIST_NODE(*interrupted_thread)); 
  				break; 
 			case thread_ready:
 				list_push_back(LIST_BASE(threads_ready),
 					       LIST_NODE(*interrupted_thread)); 
  				break; 
 			case thread_is_idle_process:
 			default:
 				BUG();
 			}
 		}
 
 		/* Neuen Kontext vorbereiten */
 		memcpy(regs, running_thread->registers, sizeof(unsigned[16])); 
 		set_spsr(running_thread->psr); 
 	}
 	/* Der Timer wird zurückgesetzt */ 
 	st_set_interval(SCHEDULER_TIMESLIZE_MS);
 	reschedule_request = 0; 
}
 
/*
 * start_new_thread() - Generiert einen neuen Thread im User-Modus
 *
 * @entry: Einsprungspunkt, bzw. die Funktion, die der Thread ausführen soll
 * @arg: Pointer auf Argument, was der Funktion übergeben wird; das
 *       Argument wird auf den Stack vom Thread kopiert
 * @arg_size: Länge von arg in Bytes
 *
 * Liefert 0 bei Erfolg und != 0 bei Fehler.
 */
int start_new_thread(void (*entry)(void *), const void *arg, unsigned int arg_size)
{
 	struct tcb *thread; 
 	unsigned int arg_position; 
  	int i;
 
 	/* Argument zu groß? */
 	if (arg_size > USER_STACK_SIZE / 2)
 		return 2;
 
 	/* Freien TCB holen */
 	if (threads_finished == NULL) { 
 		/* Kein freier Thread mehr übrig! */ 
 		return 1;
 	}
 	thread = TCB(list_remove(LIST_BASE(threads_finished), LIST_NODE(*threads_finished))); 
 
 	/*
 	 * Argument auf Thread-Stack kopieren. So kann der Aufrufer den Puffer
 	 * gleich weiter verwenden. Wir müssen allerdings auf ein vernünftiges
 	 * Alignment achten. Gemäß AAPCS sind das 8-Byte.
 	 */
  	arg_position = (thread->stack_bottom - arg_size) & ~0x7; 
 	memcpy((void *)arg_position, arg, arg_size); 
 
 	/*
 	 * Kontext des neuen Threads initialisieren.
 	 *
 	 * psr: User-Modus, alle anderen Bits auf Null => Interrupts aktiv
 	 * r0: Zeiger auf Argument (am oberen Ende des Stacks)
 	 * sp: Zeiger auf Stack (unterhalb des Arguments)
 	 * lr: Rücksprung soll den Thread beenden => Terminate-Funktion
 	 * pc: übergebener Einsprungspunkt
 	 *
 	 * Rest 0: kein Informationsleck, bzw. falls der Thread frühzeitig
 	 * abstürzt ist der Register-Dump lesbarer.
 	 */
 	thread->psr = PSR_USR; 
 	thread->registers[0] = arg_position; 
  	for (i = 1; i < 13; i++)
 		thread->registers[i] = 0; 
 	thread->registers[13] = arg_position; 
 	thread->registers[14] = (unsigned int)terminate; 
 	thread->registers[15] = (unsigned int)entry; 
 
 	/* Und in die Liste der auszuführenden Threads einfügen */ 
 	thread->status = thread_ready; 
 	list_push_front(LIST_BASE(threads_ready), LIST_NODE(*thread)); 
 
 	return 0;
}
 
/*
 * end_current_thread() - Beendet den Thread, indem er aus der Queue vom Scheduler entfernt wird.
 */
void end_current_thread(void)
{
 	BUG_ON(running_thread == NULL || running_thread == IDLE); 
 	running_thread->status = thread_finished; 
 	request_reschedule(); 
}
 
