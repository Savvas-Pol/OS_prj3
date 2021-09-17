#ifndef SPACELIST_H
#define SPACELIST_H

typedef struct SpaceListNode {
	int start;
	int end;
	struct SpaceListNode* next;
} SpaceListNode;

typedef struct SpaceList {
	SpaceListNode* head;
	int count;
} SpaceList;

SpaceList* spaceList_init();
void spaceList_destroy(SpaceList* l);
void spaceList_insert(SpaceList* l, int start, int end);
SpaceListNode* spaceList_search(SpaceList* l, int start);
void spaceList_delete(SpaceList* l, int start);
void spaceList_print(SpaceList* l);
int spaceList_full(SpaceList* l);
SpaceListNode* spaceList_searchStart(SpaceList* l, int end);
SpaceListNode* spaceList_searchEnd(SpaceList* l, int start);

SpaceListNode* spaceList_findBestFit(SpaceList* l, int size);

#endif