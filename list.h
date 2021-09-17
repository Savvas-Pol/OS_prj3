#ifndef LIST_H
#define LIST_H

typedef struct ListNode {
	int size;
	int duration;
	struct ListNode* next;
} ListNode;		//nodes of List

typedef struct List {
	ListNode* head;
	int count;
} List;		//list for waiting processes

List* list_init();									//initialize List
void list_destroy(List* l);							//destroy List
void list_insert(List* l, int size, int duration);	//insert in List
ListNode* list_search(List* l, int size);			//search in List based on size
void list_delete(List* l);							//delete first node from List
void list_print(List* l);							//print List

#endif