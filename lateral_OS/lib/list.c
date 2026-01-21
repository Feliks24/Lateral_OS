#include <list.h>
#include <lib.h>
 
//#define LIST_DEBUG
 
void list_insert(struct list_node **list_base, struct list_node *current, struct list_node *element)
{
#ifdef LIST_DEBUG
 	printf("=================\n");
 	printf("list_insert(list_base=%p, current=%p, element=%p)\n", list_base, current, element); 
 	list_debug(list_base); 
#endif
 
 	BUG_ON(list_base == NULL || element == NULL); 
 	BUG_ON(*list_base == element || current == element); 
 
 	if (*list_base == NULL) { 
 		/* Die Liste war leer und das erste Element wird eingefügt */ 
 		*list_base    = element; 
 		element->next = element;
 		element->prev = element;
 	} else {
 		if (current != NULL) {
 			/* Die Liste war nicht leer und das Element wird hinter current eingefügt */ 
 			element->next = current->next;
 			element->prev = current;
 			element->next->prev = element;
 			element->prev->next = element;
 		} else {
 			/* Die Liste war nicht leer und wir fügen das Element am Anfang der Liste ein */ 
 			element->next = *list_base; 
 			element->prev = (*list_base)->prev; 
 			element->next->prev = element;
 			element->prev->next = element;
 			*list_base = element; 
 		}
 	}
 
#ifdef LIST_DEBUG
 	printf("---------------\n");
 	list_debug(list_base); 
#endif
}
 
 
void list_push_front(struct list_node **list_base, struct list_node *element)
{
 	list_insert(list_base, NULL, element); 
}
 
void list_push_back(struct list_node **list_base, struct list_node *element)
{
 	BUG_ON(list_base == NULL); 
 	list_insert(list_base, *list_base == NULL ? NULL : (*list_base)->prev, element); 
}
 
struct list_node *list_remove(struct list_node **list_base, struct list_node* element)
{
 	BUG_ON(list_base == NULL); 
 
#ifdef LIST_DEBUG
 	printf("=================\n");
 	printf("list_remove(list_base=%p, element=%p)\n", list_base, element); 
 	list_debug(list_base); 
#endif
 
 	BUG_ON(*list_base == NULL || element == NULL); 
 	BUG_ON(element->prev == NULL || element->next == NULL);
 
 	if (element == *list_base) { 
 		/* Das vorderste Element soll entfernt werden */ 
 		*list_base = element->next; 
 		if (element == *list_base) { 
 			/* Das letzte Element der Liste soll entfernt werden */ 
 			*list_base = NULL; 
 		}
 
 	}
 
 	/* Das Element wird aus der Liste entfernt */ 
 	element->prev->next = element->next;
 	element->next->prev = element->prev;
 
 	element->next = NULL;
 	element->prev = NULL;
 
#ifdef LIST_DEBUG
 	printf("---------------\n");
 	list_debug(list_base); 
#endif
 
 	return element;
}
 
void list_debug(struct list_node **list_base)
{
 	if (*list_base == NULL) { 
 		printf("NULL\n");
 	} else {
 		struct list_node *elm = *list_base; 
 		unsigned node = 0; 
  		do {
 			printf("node %x @ %p: \\/ %p /\\ %p\n", ++node, elm, elm->next, elm->prev); 
 			elm = elm->next;
 		} while(elm != *list_base); 
 	}
}
 
