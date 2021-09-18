#ifndef SPACELIST_H
#define SPACELIST_H

typedef struct SpaceListNode {
    int start;
    int end;
    struct SpaceListNode* next;
} SpaceListNode; //nodes of SpaceList

typedef struct SpaceList {
    SpaceListNode* head;
    int count;
} SpaceList; //list for available memory spaces

SpaceList* spaceList_init(); //initialize SpaceList
void spaceList_destroy(SpaceList* l); //destroy SpaceList
void spaceList_insert(SpaceList* l, int start, int end); //insert in SpaceList based on size
SpaceListNode* spaceList_search(SpaceList* l, int start); //search in SpaceList based on start
void spaceList_delete(SpaceList* l, int start); //delete from SpaceList
void spaceList_print(SpaceList* l); //print SpaceList
int spaceList_full(SpaceList* l); //check if SpaceList is full

SpaceListNode* spaceList_searchStart(SpaceList* l, int end); //search SpaceListNodes that start on end parameter 
SpaceListNode* spaceList_searchEnd(SpaceList* l, int start); //search SpaceListNodes that end on start parameter

SpaceListNode* bestFit(SpaceList* l, int size); //best-fit algorithm
SpaceListNode* worstFit(SpaceList* l, int size); //worst-fit algorithm
SpaceListNode* buddy(SpaceList* l, int size); //buddy algorithm


#endif