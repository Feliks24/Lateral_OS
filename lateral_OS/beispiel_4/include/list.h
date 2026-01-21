#ifndef _LIST_H_
#define _LIST_H_
 
/*
 * Funktionen zum Einfügen und Entfernen von Elementen aus einer
 * doppelt verketteten zirkulären Liste
 */
 
struct list_node { 
 	struct list_node *next; 
 	struct list_node *prev; 
};
 
/*
 * list_insert() - Fügt der Liste ein Element hinzu.
 *
 * @list_base: Ein Zeiger auf die Liste.
 *             *list_base zeigt auf das erste Element der Liste, bzw. NULL, falls die Liste leer ist.
 * @current: Ein Listenelement, hinter dem element eingefügt werden soll,
 *          oder NULL um am Anfang der Liste einzufügen. current muss
 *          bereits Teil der Liste sein.
 * @element: Einzufügendes Element.
*/
void list_insert(struct list_node **list_base, struct list_node *current, struct list_node *element); 
 
/*
 * list_push_front() - Fügt ein Element am Anfang der Liste ein,
 */
void list_push_front(struct list_node **list_base, struct list_node *element); 
 
/*
 * list_push_back() - Fügt ein Element am Ende der Liste ein
 */
void list_push_back(struct list_node **list_base, struct list_node *element); 
 
/*
 * list_remove() - Entfernt Element aus der Liste
 * @list_base: Verweis auf die Liste. Darf keine leere Liste sein.
 *             *list_base wird auf das erste Element der Liste gesetzt, bzw NULL, falls die Liste leer ist.
 *
 * @element: Element, welches aus der Liste entfent werden soll.
 */
struct list_node *list_remove(struct list_node **list_base, struct list_node* element); 
 
/*
 * list_debug() - Gibt die Liste zu Debugzwecken aus
 */
void list_debug(struct list_node **list_base); 
 
#endif /* _LIST_H_ */
