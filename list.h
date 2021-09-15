#ifndef LIST_H
#define LIST_H

typedef struct ListNode {
	int id;
	int start;
	int end;
	struct ListNode* next;
} ListNode;

typedef struct List {
	ListNode* head;
	int count;
} List;

List* list_init();
void list_destroy(List* l);
void list_insert(List* l, int start, int end);
void list_delete(List* l, int id);
ListNode* list_search(List* l, int id);
void list_print(List* l);


#endif