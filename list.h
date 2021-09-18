#ifndef LIST_H
#define LIST_H

#include "spaceList.h"
#include "usedList.h"

typedef struct ListNode {
	int size;
	int duration;
	int arrival;
	struct ListNode* next;
} ListNode;		//nodes of List

typedef struct List {
	ListNode* head;
	int count;
} List;		//list for waiting processes

List* list_init();									//initialize List
void list_destroy(List* l);							//destroy List
void list_insert(List* l, int size, int duration, int arrival);	//insert in List
ListNode* list_search(List* l, int size);			//search in List based on size
void list_delete(List* l);							//delete first node from List
void list_print(List* l);							//print List
int list_empty(List* l);							//check if list is empty

void checkWaitingList(List* l, SpaceList* sl, UsedList* ul, char* alg, FILE* logfile, int i);	//checks waiting list processes and available memory

#endif