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
    const char * LOGFILE = "logfile_m.txt";

    int i = 0, D, lo, hi, t, T, S;
    char command[20] = {0};
    int fakeid, size, duration;
    char* alg;

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
            removeFinishedProcess(runningList, spaceList, logfile, fakeid);
            
            //        checkWaitingList(L, spaceList, usedList, alg, logfile, i); //check waiting list
            
            // while ... move processes from wailting list to running list
            // and notify G
        }
    }

    fprintf(logfile, "Totally %d processes were placed in waiting list\n", waitingList->count);

    fclose(logfile);

    free(alg);

    shared_memory_detach();

    spaceList_destroy(spaceList);
    usedList_destroy(runningList);
    list_destroy(waitingList);

    printf("M finished with pid: %d \n", getpid());

    return 0;
}
