#include <stdio.h>
#include <stdlib.h>

#include "list.h"


List* list_init() {

	List* new = malloc(sizeof(List));
	new->head = NULL;
	new->count = 0;

	return new;
}

void list_destroy(List* l) {

	ListNode* node = l->head;
	ListNode* temp = node;

	while(temp != NULL) {
		node = node->next;
		// free(temp->next);
		free(temp);
		temp = node;
	}
	free(l);
}

void list_insert(List* l, int start, int end) {

	ListNode* temp = l->head;
	ListNode* prev;

	ListNode* new = malloc(sizeof(ListNode));
	new->id = l->count;
	l->count++;
	new->start = start;
	new->end = end;

	int new_size = end - start;
	int node_size;

	int first = 1;

	while(temp != NULL) {
		node_size = temp->end - temp->start;
		if(new_size < node_size) {
			if(first) {
				new->next = temp;
				l->head = new;
			} else {
				new->next = temp;
				prev->next = new;
			}
			return;
		} else {
			prev = temp;
			temp = temp->next;
			first = 0;
		}
	}

	if(first) {
		l->head = new;
	} else {
		prev->next = new;
	}

	new->next = NULL;

	return;
}

ListNode* list_search(List* l, int id) {
	
	ListNode* temp = l->head;

	while(temp != NULL) {
		if(temp->id == id) {
			printf("Found ( %d, %d, %d)\n", temp->id, temp->start, temp->end);
			printf("---------------\n");
			return temp;
		} else {
			temp = temp->next;
		}
	}

	printf("Process with id: %d not found\n", id);
	printf("---------------\n");

	return NULL;
}

void list_delete(List* l, int id) {

	ListNode* temp = list_search(l, id);
	ListNode* prev = l->head;

	if(temp != l->head) {
		while((prev != temp) && (prev != NULL)) {
			if(prev->next == temp) {
				prev->next = temp->next;
				// free(temp->next);
				free(temp);
				printf("Process with id: %d deleted\n", id);
				printf("---------------\n");
				return;
			}
			prev = prev->next;
		}
	} else if(temp == l->head) {
		l->head = temp->next;
		// free(temp->next);
		free(temp);
		printf("Process with id: %d deleted\n", id);
		printf("---------------\n");
	} else {
		printf("Process with id: %d not found\n", id);
		printf("---------------\n");
	}

	return;
}

void list_print(List* l) {

	ListNode* temp = l->head;

	if(temp == NULL) {
		printf("List is empty!\n");
		printf("---------------\n");
		return;
	} else {
		printf("List items:\n");
	}

	while(temp != NULL) {
		printf("(%d, %d, %d)\n", temp->id, temp->start, temp->end);
		temp = temp->next;
	}
	printf("---------------\n");

	return;
}
