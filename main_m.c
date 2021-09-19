#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "helpers.h"
#include "usedList.h"
#include "spaceList.h"
#include "list.h"
#include "shared_memory.h"

int main_m(int argc, char** argv) {

    const char * LOGFILE = "logfile.txt";

    int i = 0, D, lo, hi, t, T, S;
    char command[20] = {0};
    int fakeid, size, duration;
    char* alg;
    double E = 0.0;

    FILE* logfile = fopen(LOGFILE, "w+");

    if (readArguments(argc, argv, &D, &lo, &hi, &t, &T, &S, &alg)) { //read arguments from command line
        return -1;
    }

    shared_memory_attach();

    printf("M started with pid: %d\n", getpid());

    SpaceList* spaceList = spaceList_init(); //list for memory spaces
    UsedList* runningList = usedList_init(); //list for used memory spaces
    List* waitingList = list_init(); //list for waiting processes

    spaceList_insert(spaceList, 0, S - 1);
    SpaceListNode* spaceListSpace;

    while (++i < 1000000) {
        shared_memory_m_obtain(command, &fakeid, &size, &duration);

        if (strcmp(command, "EXIT") == 0) {
            break;
        } else if (strcmp(command, "VP_START") == 0) {
            printf("[ M ]: VPSTART received: <%d,%d,%d>\n", fakeid, size, duration);

            E += size*duration;

            if ((spaceList_full(spaceList) == 0)) {
                if (strcmp(alg, "best-fit") == 0) {
                    spaceListSpace = bestFit(spaceList, size);
                } else if (strcmp(alg, "worst-fit") == 0) {
                    spaceListSpace = worstFit(spaceList, size);
                } else { //buddy
                    spaceListSpace = buddy(spaceList, size);
                }

                if (spaceListSpace != NULL) {
                    usedList_insert(runningList, fakeid, spaceListSpace->start, spaceListSpace->start + size - 1, duration);
                    fprintf(logfile, "Process with id: %d placed in memory from: %d to: %d at: %d time\n", runningList->count - 1, spaceListSpace->start, spaceListSpace->start + size - 1, i);
                    spaceListSpace->start = spaceListSpace->start + size;

                    shared_memory_m_place("STARTED", fakeid, size, duration);
                } else {
                    list_insert(waitingList, fakeid, size, duration, i); //if it doesn't fit in available spaces, then place in L list
                    fprintf(logfile, "Process with size: %d and duration: %d can't fit in available memory spaces! Placed in waiting list at: %d time\n", size, duration, i);
                    shared_memory_m_place("FAILED", fakeid, size, duration);
                }
            } else {
                list_insert(waitingList, fakeid, size, duration, fakeid); //if memory is full, then place in L list
                fprintf(logfile, "Memory full! Process with size: %d and duration: %d placed in waiting list at: %d time\n", size, duration, i);
                shared_memory_m_place("FAILED", fakeid, size, duration);
            }
        } else if (strcmp(command, "VP_STOP") == 0) {
            printf("[ M ]: VP_STOP received: <%d,%d,%d>\n", fakeid, size, duration);

            removeFinishedProcess(runningList, spaceList, logfile, fakeid);

            checkWaitingList(waitingList, spaceList, runningList, alg, logfile, i); //check waiting list

            shared_memory_m_place("#", 0, 0, 0);    //notify G
        }
    }

    fprintf(logfile, "Totally %d processes were placed in waiting list\n", waitingList->count);

    free(alg);

    shared_memory_detach();
    
    fprintf(logfile, "==========================\n");
    fprintf(logfile, "Calculation of statistics\n");
    fprintf(logfile, "==========================\n");

    SpaceListNode* node = spaceList->head;

    double mean = 0.0;
    int counter =0;
    double var = 0;

    E /= S*D;

    while (node != NULL) {
        counter++;
        mean += node->end + 1 - node->start;
        node=node->next;       
    }
    mean /= counter;
    
    node = spaceList->head;
    
    while (node != NULL) {
        var += ((node->end + 1 - node->start) - mean)*((node->end + 1 - node->start) - mean);
        node=node->next;       
    }
    var /= counter;
    
    fprintf(logfile, "Mean: %lf\n", mean);
    fprintf(logfile, "Var: %lf\n", var);
    fprintf(logfile, "E: %lf\n", E);

    spaceList_destroy(spaceList);
    usedList_destroy(runningList);
    list_destroy(waitingList);

    fclose(logfile);
    printf("M finished with pid: %d \n", getpid());

    return 0;
}
