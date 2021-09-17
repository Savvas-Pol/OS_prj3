#include <stdio.h>
#include <stdlib.h>

#include "list.h"

List* list_init() {			//initialize List

	List* new = malloc(sizeof(List));
	new->head = NULL;
	new->count = 0;

	return new;
}

void list_destroy(List* l) {	//destroy List

	ListNode* node = l->head;
	ListNode* temp = node;

	while(temp != NULL) {
		node = node->next;
		free(temp);
		temp = node;
	}
	free(l);
}

void list_insert(List* l, int size, int duration) {	//insert in List

	ListNode* temp = l->head;

	ListNode* new = malloc(sizeof(ListNode));
	l->count++;
	new->size = size;
	new->duration = duration;
	new->next = NULL;

	if(temp != NULL) {
		while(temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = new;
	} else {
		l->head = new;
	}
	return;
}

ListNode* list_search(List* l, int size) {	//search in List based on size
	
	ListNode* temp = l->head;

	while(temp != NULL) {
		if(temp->size == size) {
			printf("Found ( %d, %d)\n", temp->size, temp->duration);
			printf("---------------\n");
			return temp;
		} else {
			temp = temp->next;
		}
	}

	printf("Process with size: %d not found\n", size);
	printf("---------------\n");

	return NULL;
}

void list_delete(List* l) {	//delete first node from List
	
	ListNode* temp = l->head;

	if(temp != NULL) {
		l->head = l->head->next;
		free(temp);
	}

	return;
}

void list_print(List* l) {	//print List
	
	ListNode* temp = l->head;

	if(temp == NULL) {
		printf("List is empty!\n");
		printf("---------------\n");
		return;
	} else {
		printf("List items:\n");
	}

	while(temp != NULL) {
		printf("(%d, %d)\n", temp->size, temp->duration);
		temp = temp->next;
	}
	printf("---------------\n");

	return;
}