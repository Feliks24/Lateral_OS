#include <system.h>
#include <list.h>
#include <lib.h>
#include <stddef.h>
#include <st.h>
#include <dbgu.h>
#include "memlayout.h"
 
#define MAX_THREADS 16
#define SCHEDULER_TIMESLIZE_MS 50
 
enum thread_status { 
 	THREAD_FINISHED = 0,
 	THREAD_READY,
 	THREAD_RUNNING,
 	THREAD_IDLE,
 	THREAD_BLOCKED,
};
 
struct tcb { 
 	struct list_node node; 
 
 	/* Verwaltungsdaten */ 
 	unsigned int thread_id; 
 	unsigned int stack_bottom; 
 	unsigned int waitslices; 
  	enum thread_status status; 
 
 	/* Kontext des Threads */ 
  	enum psr psr;
 	unsigned int registers[16];
};
 
/* Makros um die Verwendung der Listenfunktionen zu vereinfachen */ 
#define LIST_BASE(X) ((struct list_node **)(&(X)))
#define LIST_NODE(X) (&((X).node))
#define TCB(X)       ((struct tcb *)(X))
 
/* reschedule_request = 1 veranlasst einen Prozesswechsel, falls möglich */ 
static unsigned reschedule_request; 
 
static struct tcb idle_thread; 
static struct tcb user_threads[MAX_THREADS]; 
 
/* Zeiger auf genau einen laufenden thread */ 
static struct tcb *running_thread    = NULL; 
 
/* Listen von Threads in dem jeweiligen Zustand */ 
static struct tcb *threads_ready     = NULL; 
static struct tcb *threads_finished  = NULL; 
static struct tcb *threads_charwait  = NULL; 
static struct tcb *threads_slicewait = NULL; 
 
static void request_reschedule(void)
{
 	reschedule_request = 1; 
}
 
/* Initialisierung des Schedulers */ 
void scheduler_init(void)
{
  	int i;
 
 	idle_thread.status	  = THREAD_IDLE; 
 	idle_thread.registers[15] = (unsigned int)idle; 
 	idle_thread.psr		  = PSR_SYS; 
 
  	for (i = 0; i < MAX_THREADS; i++) {
 		user_threads[i].thread_id	= i; 
 		user_threads[i].status		= THREAD_FINISHED; 
 		user_threads[i].stack_bottom	= USER_STACK_BOTTOM - i * USER_STACK_SIZE; 
 		list_push_back(LIST_BASE(threads_finished), LIST_NODE(user_threads[i])); 
 	}
 	request_reschedule(); 
}
 
/*
 * schedule() - ein einfacher Round-Robin-Scheduler
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
 
 	/* Alten Kontext sichern. Der Kontext vom Idle-Thread muss nicht gesichert werden. */ 
 	if (running_thread != NULL && running_thread->status != THREAD_IDLE) { 
 		if (running_thread->status == THREAD_RUNNING) { 
 			running_thread->status = THREAD_READY; 
 			list_push_back(LIST_BASE(threads_ready),
 				       LIST_NODE(*running_thread)); 
 
 		}
 		memcpy(running_thread->registers, regs, sizeof(running_thread->registers)); 
 		running_thread->psr = get_spsr(); 
 	}
 
 	/* nächsten Thread heraussuchen */ 
 	if (threads_ready != NULL) { 
 		running_thread = TCB(list_remove(LIST_BASE(threads_ready),
 						 LIST_NODE(*threads_ready))); 
 		BUG_ON(running_thread->status != THREAD_READY); 
 		running_thread->status = THREAD_RUNNING; 
 	} else {
 		running_thread = &idle_thread; 
 	}
 
 	/* Neuen Kontext laden */ 
 	memcpy(regs, running_thread->registers, sizeof(running_thread->registers)); 
 	set_spsr(running_thread->psr); 
 
 	/* Time-Slice zurücksetzen */ 
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
 	 * pc: übergebener Einsprungspunkt
 	 *
 	 * Rest 0: kein Informationsleck, bzw. falls der Thread frühzeitig
 	 * abstürzt ist der Register-Dump lesbarer.
 	 *
 	 * Insbesondere ist auch das Link-Register 0. Per Definition muss sich
 	 * ein Thread mittels SWI beenden. Das Link-Register sollte also nicht
 	 * benutzt werden.
 	 */
 	thread->psr = PSR_USR; 
 	thread->registers[0] = arg_position; 
  	for (i = 1; i < 13; i++)
 		thread->registers[i] = 0; 
 	thread->registers[13] = arg_position; 
 	thread->registers[14] = 0; 
 	thread->registers[15] = (unsigned int)entry; 
 
 	/* Und in die Liste der auszuführenden Threads einfügen */ 
 	thread->status = THREAD_READY; 
 	list_push_front(LIST_BASE(threads_ready), LIST_NODE(*thread)); 
 
 	return 0;
}
 
static void check_slicewait_threads(void)
{
 	if (threads_slicewait == NULL)
 		return;
 
 	/*
 	 * Liste schlafender Threads durchgehen und waitslices decrementieren,
 	 * wenn Zähler 0 erreicht Thread aufwecken.
 	 */
 	struct tcb *t = threads_slicewait; 
  	do {
 		t = TCB(t->node.next);
 		t->waitslices--; 
 		if (t->waitslices)
  			continue;
 
 		/* nächsten Thread merken wegen Listenmanipulation */ 
 		struct tcb *n = TCB(t->node.next); 
 
 		/* Aufwecken */
 		list_remove(LIST_BASE(threads_slicewait), LIST_NODE(*t)); 
 		BUG_ON(t->status != THREAD_BLOCKED);
 		t->status = THREAD_READY;
 		list_push_back(LIST_BASE(threads_ready), LIST_NODE(*t)); 
 
 		/* weiter mit nächstem Thread */ 
 		t = n;
 	} while (threads_slicewait != NULL && t != threads_slicewait); 
}
 
/*
 * end_current_thread() - Beendet den Thread, indem er aus der Queue vom Scheduler entfernt wird.
 */
void end_current_thread(void)
{
 	BUG_ON(running_thread == NULL); 
 	BUG_ON(running_thread->status != THREAD_RUNNING); 
 
 	running_thread->status = THREAD_FINISHED; 
 	list_push_back(LIST_BASE(threads_finished), LIST_NODE(*running_thread)); 
 	request_reschedule(); 
}
 
void thread_wait_char(void)
{
 	BUG_ON(running_thread == NULL); 
 	BUG_ON(running_thread->status != THREAD_RUNNING); 
 
 	running_thread->status = THREAD_BLOCKED; 
 	list_push_back(LIST_BASE(threads_charwait), LIST_NODE(*running_thread)); 
 	request_reschedule(); 
}
 
void thread_wait_slices(unsigned int slices)
{
 	BUG_ON(running_thread == NULL); 
 	BUG_ON(running_thread->status != THREAD_RUNNING); 
 
 	if (slices) {
 		running_thread->status = THREAD_BLOCKED; 
 		list_push_back(LIST_BASE(threads_slicewait), LIST_NODE(*running_thread)); 
 		running_thread->waitslices = slices; 
 	}
 	request_reschedule(); 
}
 
void wakeup_charwait(void)
{
 	BUG_ON(!dbgu_has_char()); 
 
 	if (!threads_charwait)
 		return;
 
 	/* Ein wartender Thread kann aufgeweckt werden */ 
 	struct tcb *t = TCB(list_remove(LIST_BASE(threads_charwait),
 					LIST_NODE(*threads_charwait))); 
 	BUG_ON(t->status != THREAD_BLOCKED);
 	t->status = THREAD_READY;
 
 	/* Rückgabewert für SWI setzen */ 
 	t->registers[0] = dbgu_getc(); 
 
 	/* Thread bevorzugt behandeln: vorne einfügen + Kontextwechsel */ 
 	list_push_front(LIST_BASE(threads_ready), LIST_NODE(*t)); 
 	request_reschedule(); 
}
 
void scheduler_tick(void)
{
 	check_slicewait_threads(); 
 	request_reschedule(); 
}
