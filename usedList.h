#ifndef USEDLIST_H
#define USEDLIST_H

#include "spaceList.h"

typedef struct UsedListNode {
	int id;
	int start;
	int end;
	int duration;
	struct UsedListNode* next;
} UsedListNode;	//nodes of UsedList

typedef struct UsedList {
	UsedListNode* head;
	int count;
} UsedList;	//list for used memory segments

UsedList* usedList_init();												//initialize UsedList
void usedList_destroy(UsedList* l);										//destroy UsedList
void usedList_insert(UsedList* l, int start, int end, int duration);	//insert into UsedList based on size
UsedListNode* usedList_search(UsedList* l, int id);						//search in UsedList based on id
void usedList_delete(UsedList* l, int id);								//delete from UsedList
void usedList_print(UsedList* l);										//print UsedList

void usedList_reduceDurations(UsedList* l);								//reduce duration of all processes by 1
void checkFinishedProcesses(UsedList* ul, SpaceList* sl);				//check if any process has finished

#endif