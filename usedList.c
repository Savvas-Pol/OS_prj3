#include <stdio.h>
#include <stdlib.h>

#include "usedList.h"


UsedList* usedList_init() {	//initialize UsedList

	UsedList* new = malloc(sizeof(UsedList));
	new->head = NULL;
	new->count = 0;

	return new;
}

void usedList_destroy(UsedList* l) {	//destroy UsedList

	UsedListNode* node = l->head;
	UsedListNode* temp = node;

	while(temp != NULL) {
		node = node->next;
		free(temp);
		temp = node;
	}
	free(l);
}

void usedList_insert(UsedList* l, int start, int end, int duration) {	//insert into UsedList based on size

	UsedListNode* temp = l->head;
	UsedListNode* prev;

	UsedListNode* new = malloc(sizeof(UsedListNode));
	new->id = l->count;	//assing an increasing id to processes
	l->count++;
	new->start = start;
	new->end = end;
	new->duration = duration;

	int new_size = end - start;	//size of the new process
	int node_size;

	int first = 1;	//flag to know if we are in the first node of the list

	while(temp != NULL) {
		node_size = temp->end - temp->start;	//size of current process
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

UsedListNode* usedList_search(UsedList* l, int id) {	//search in UsedList
	
	UsedListNode* temp = l->head;

	while(temp != NULL) {
		if(temp->id == id) {
			// printf("Found ( %d, %d, %d, %d)\n", temp->id, temp->start, temp->end, temp->duration);
			// printf("---------------\n");
			return temp;
		} else {
			temp = temp->next;
		}
	}

	// printf("Process with id: %d not found\n", id);
	// printf("---------------\n");

	return NULL;
}

void usedList_delete(UsedList* l, int id) {	//delete from UsedList

	UsedListNode* temp = usedList_search(l, id);
	UsedListNode* prev = l->head;

	if(temp != l->head) {
		while((prev != temp) && (prev != NULL)) {
			if(prev->next == temp) {
				prev->next = temp->next;
				free(temp);
				// printf("Process with id: %d deleted\n", id);
				// printf("---------------\n");
				return;
			}
			prev = prev->next;
		}
	} else if(temp == l->head) {
		l->head = temp->next;
		free(temp);
		// printf("Process with id: %d deleted\n", id);
		// printf("---------------\n");
	} else {
		// printf("Process with id: %d not found\n", id);
		// printf("---------------\n");
	}

	return;
}

void usedList_print(UsedList* l) {	//print UsedList

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

void usedList_reduceDurations(UsedList* l) {	//reduce duration of all processes by 1

	UsedListNode* temp = l->head;

	while(temp != NULL) {
		temp->duration--;
		temp = temp->next;
	}

	return;
}

void checkFinishedProcesses(UsedList* ul, SpaceList* sl, FILE* logfile, int i) {	//check if any process has finished

	UsedListNode* temp = ul->head;
	SpaceListNode* slnode;

	while(temp != NULL) {
		if(temp->duration == 0) {		//if process has finished
			if((slnode = spaceList_searchStart(sl, temp->end)) != NULL) {	//search if any available space starts where current process was ending
				slnode->start = temp->start;		//update start
			} else if((slnode = spaceList_searchEnd(sl, temp->start)) != NULL) {	//search if any available space ends where current process was starting
				slnode->end = temp->end;			//update end
			} else {
				spaceList_insert(sl, temp->start, temp->end);		//insert in spaceList
			}
			fprintf(logfile, "Process with id: %d finished at: %d time\n", temp->id, i);
			usedList_delete(ul, temp->id);			//remove from usedList
		}
		temp = temp->next;
	}
}