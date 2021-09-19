#include <stdio.h>
#include <stdlib.h>

#include "spaceList.h"

SpaceList* spaceList_init() { //initialize SpaceList

    SpaceList* new = malloc(sizeof (SpaceList));
    new->head = NULL;
    new->count = 0;

    return new;
}

void spaceList_destroy(SpaceList* l) { //destroy SpaceList

    SpaceListNode* node = l->head;
    SpaceListNode* temp = node;

    while (temp != NULL) {
        node = node->next;
        free(temp);
        temp = node;
    }
    free(l);
}

void spaceList_insert(SpaceList* l, int start, int end) { //insert in SpaceList based on size

    SpaceListNode* temp = l->head;
    SpaceListNode* prev;

    SpaceListNode* new = malloc(sizeof (SpaceListNode));
    l->count++;
    new->start = start;
    new->end = end;

    int new_size = end - start;
    int node_size;

    int first = 1;

    while (temp != NULL) {
        node_size = temp->end - temp->start;
        if (new_size < node_size) {
            if (first) {
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

    if (first) {
        l->head = new;
    } else {
        prev->next = new;
    }

    new->next = NULL;

    return;
}

SpaceListNode* spaceList_search(SpaceList* l, int start) { //search in SpaceList based on start

    SpaceListNode* temp = l->head;

    while (temp != NULL) {
        if (temp->start == start) {
            // printf("Found ( %d, %d)\n", temp->start, temp->end);
            // printf("---------------\n");
            return temp;
        } else {
            temp = temp->next;
        }
    }

    // printf("Process that starts at: %d not found\n", start);
    // printf("---------------\n");

    return NULL;
}

void spaceList_delete(SpaceList* l, int start) { //delete from SpaceList

    SpaceListNode* temp = spaceList_search(l, start);
    SpaceListNode* prev = l->head;

    if (temp != l->head) {
        while ((prev != temp) && (prev != NULL)) {
            if (prev->next == temp) {
                prev->next = temp->next;
                free(temp);
                // printf("Process that starts at: %d deleted\n", start);
                // printf("---------------\n");
                return;
            }
            prev = prev->next;
        }
    } else if (temp == l->head) {
        l->head = temp->next;
        free(temp);
        // printf("Process that starts at: %d deleted\n", start);
        // printf("---------------\n");
    } else {
        // printf("Process that starts at: %d not found\n", start);
        // printf("---------------\n");
    }

    return;
}

void spaceList_print(SpaceList* l) { //print SpaceList

    SpaceListNode* temp = l->head;

    if (temp == NULL) {
        printf("SpaceList is empty!\n");
        printf("---------------\n");
        return;
    } else {
        printf("SpaceList items:\n");
    }

    while (temp != NULL) {
        printf("(%d, %d)\n", temp->start, temp->end);
        temp = temp->next;
    }
    printf("---------------\n");

    return;
}

int spaceList_full(SpaceList* l) { //check if SpaceList is full

    if ((l->head->next == NULL) && (l->head->start == l->head->end)) {
        return 1;
    } else {
        return 0;
    }
}

SpaceListNode* spaceList_searchStart(SpaceList* l, int end) { //search SpaceListNodes that start on end parameter 

    SpaceListNode* temp = l->head;

    while (temp != NULL) {
        if (temp->start == end) {
            return temp;
        }
        temp = temp->next;
    }

    return NULL;
}

SpaceListNode* spaceList_searchEnd(SpaceList* l, int start) { //search SpaceListNodes that end on start parameter

    SpaceListNode* temp = l->head;

    while (temp != NULL) {
        if (temp->end == start) {
            return temp;
        }
        temp = temp->next;
    }

    return NULL;
}

SpaceListNode* bestFit(SpaceList* l, int size) { //best-fit algorithm

    SpaceListNode* temp = l->head;
    SpaceListNode* minSpace = NULL;

    while (temp != NULL) {
        if (size <= temp->end - temp->start + 1) { //if the process fits
            if (minSpace != NULL) {
                if (temp->end - temp->start < minSpace->end - minSpace->start) { //find min
                    minSpace = temp;
                }
            } else {
                minSpace = temp;
            }
        }
        temp = temp->next;
    }

    return minSpace;
}

SpaceListNode* worstFit(SpaceList* l, int size) { //worst-fit algorithm

    SpaceListNode* temp = l->head;
    SpaceListNode* maxSpace = NULL;

    while (temp != NULL) {
        if (size <= temp->end - temp->start + 1) { //if the process fits
            if (maxSpace != NULL) {
                if (temp->end - temp->start > maxSpace->end - maxSpace->start) { //find max
                    maxSpace = temp;
                }
            } else {
                maxSpace = temp;
            }
        }
        temp = temp->next;
    }

    return maxSpace;
}

SpaceListNode* buddy(SpaceList* l, int size) { //buddy algorithm

    SpaceListNode* temp = l->head;
    SpaceListNode* buddy = NULL;
    int middle, fits = 0;

    while (temp != NULL) {
        while(fits != 1) {
            middle = (temp->end + 1) / 2;
            if ((size > middle) && (size <= temp->end + 1)) {
                buddy = temp;
                buddy->used = 1;
                fits = 1;
                return buddy;
            } else {
                if(temp->next == NULL) {
                    spaceList_insert(l, temp->start, middle - 1);
                    temp->start = middle;
                    temp = temp->next;
                } else {
                    if(temp->next->end - temp->next->start + 1 < size) {
                        //borei na ksanaspasei
                        buddy = temp;
                        buddy->used = 1;
                        fits = 1;
                        return buddy;
                    } else {
                        if(temp->next->used == 1) {
                            middle = (temp->end - temp->start + 1) / 2;
                            spaceList_insert(l, temp->start, temp->start + middle - 1);
                            temp->start = temp->start + middle;
                        }
                        temp = temp->next;
                    }
                }
            }
        }
        temp = temp->next;
    }

    return buddy;
}