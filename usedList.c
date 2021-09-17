#include <stdio.h>
#include <stdlib.h>

#include "usedList.h"


UsedList* usedList_init() {

	UsedList* new = malloc(sizeof(UsedList));
	new->head = NULL;
	new->count = 0;

	return new;
}

void usedList_destroy(UsedList* l) {

	UsedListNode* node = l->head;
	UsedListNode* temp = node;

	while(temp != NULL) {
		node = node->next;
		free(temp);
		temp = node;
	}
	free(l);
}

void usedList_insert(UsedList* l, int start, int end, int duration) {

	UsedListNode* temp = l->head;
	UsedListNode* prev;

	UsedListNode* new = malloc(sizeof(UsedListNode));
	new->id = l->count;
	l->count++;
	new->start = start;
	new->end = end;
	new->duration = duration;

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

UsedListNode* usedList_search(UsedList* l, int id) {
	
	UsedListNode* temp = l->head;

	while(temp != NULL) {
		if(temp->id == id) {
			printf("Found ( %d, %d, %d, %d)\n", temp->id, temp->start, temp->end, temp->duration);
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

void usedList_delete(UsedList* l, int id) {

	UsedListNode* temp = usedList_search(l, id);
	UsedListNode* prev = l->head;

	if(temp != l->head) {
		while((prev != temp) && (prev != NULL)) {
			if(prev->next == temp) {
				prev->next = temp->next;
				free(temp);
				printf("Process with id: %d deleted\n", id);
				printf("---------------\n");
				return;
			}
			prev = prev->next;
		}
	} else if(temp == l->head) {
		l->head = temp->next;
		free(temp);
		printf("Process with id: %d deleted\n", id);
		printf("---------------\n");
	} else {
		printf("Process with id: %d not found\n", id);
		printf("---------------\n");
	}

	return;
}

void usedList_print(UsedList* l) {

	UsedListNode* temp = l->head;

	if(temp == NULL) {
		printf("UsedList is empty!\n");
		printf("---------------\n");
		return;
	} else {
		printf("UsedList items:\n");
	}

	while(temp != NULL) {
		printf("(%d, %d, %d, %d)\n", temp->id, temp->start, temp->end, temp->duration);
		temp = temp->next;
	}
	printf("---------------\n");

	return;
}

void usedList_reduceDurations(UsedList* l) {

	UsedListNode* temp = l->head;

	while(temp != NULL) {
		temp->duration--;
		temp = temp->next;
	}

	return;
}

void check_finished_processes(UsedList* ul, SpaceList* sl) {

	UsedListNode* temp = ul->head;
	SpaceListNode* slnode;

	while(temp != NULL) {
		if(temp->duration == 0) {
			if((slnode = spaceList_searchStart(sl, temp->end)) != NULL) {
				slnode->start = temp->start;
			} else if((slnode = spaceList_searchEnd(sl, temp->start)) != NULL) {
				slnode->end = temp->end;
			} else {
				spaceList_insert(sl, temp->start, temp->end);
			}
			usedList_delete(ul, temp->id);
		}
		temp = temp->next;
	}
}