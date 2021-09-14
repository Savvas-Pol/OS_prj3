#include <stdlib.h>

#include "list.h"


List* list_init() {

	List* new = malloc(sizeof(List));
	new->count = 0;

	return new;
}

void list_destroy(List* l) {

	ListNode* node = l->head;
	ListNode* temp = node;

	while(temp != NULL) {
		node = node->next;
		free(temp->next);
		free(temp);
		temp = node;
	}

	free(l);
}

void list_insert(List* l, int id, int start, int end) {

	// ListNode* temp = l->head;
	// ListNode* new = malloc(sizeof(ListNode));
	// new->id = l->count;
	// l->count++;
	// new->start = start;
	// new->end = end;

	// int new_size = end - start;
	// int node_size;

	// while(temp != NULL) {
	// 	node_size = temp->end - temp->start;
	// 	if(new_size < node_size) {
	// 		//insert
	// 	} else {
	// 		temp = temp->next;
	// 	}
	// }

}

void list_delete() {

}

// List* list_search() {
	
// }