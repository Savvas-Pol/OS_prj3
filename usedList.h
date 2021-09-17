#ifndef USEDLIST_H
#define USEDLIST_H

#include "spaceList.h"

typedef struct UsedListNode {
	int id;
	int start;
	int end;
	int duration;
	struct UsedListNode* next;
} UsedListNode;

typedef struct UsedList {
	UsedListNode* head;
	int count;
} UsedList;

UsedList* usedList_init();
void usedList_destroy(UsedList* l);
void usedList_insert(UsedList* l, int start, int end, int duration);
UsedListNode* usedList_search(UsedList* l, int id);
void usedList_delete(UsedList* l, int id);
void usedList_print(UsedList* l);
void usedList_reduceDurations(UsedList* l);
void check_finished_processes(UsedList* ul, SpaceList* sl);

#endif