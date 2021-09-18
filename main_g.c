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

int main_g(int argc, char** argv) {
    srand(time(0));

    int i, D, lo, hi, t, T, S;
    int size, nextArrival, duration;
    char* alg;
    char result[20] = {0};

    if (readArguments(argc, argv, &D, &lo, &hi, &t, &T, &S, &alg)) { //read arguments from command line
        return -1;
    }

    shared_memory_attach();

    printf("G started with pid: %d \n", getpid());


    UsedList* runningList = usedList_init();
    List* waitingList = list_init(); //list for waiting processes

    int fake_pid = 0;

    for (i = 0; i < D; i++) {
        printf("  ---- Simulation step %d of %d  --- \n", i + 1, D);

        if (i == 0) { //beginning of simulation
            nextArrival = generatePoissonVariable(t);
            size = generateUniformVariable(lo, hi);
            duration = (int) generateExponentialVariable((double) T);
            fake_pid++;
        } else if (nextArrival == 0) { //arrival of process
            nextArrival = generatePoissonVariable(t);

            printf("[ G ] Process #%d arrived, next arrival in %d steps \n", fake_pid, nextArrival);

            shared_memory_g_place("VP_START", fake_pid, size, duration);

            shared_memory_g_obtain(result, &fake_pid, &size, &duration);

            if (strcmp(result, "STARTED") == 0) {
                usedList_insert(runningList, fake_pid, 0, 0, duration);
            }



            size = generateUniformVariable(lo, hi);
            duration = (int) generateExponentialVariable((double) T);
            fake_pid++;
        }

        usedList_reduceDurations(runningList); //reduce duration of processes on every iteration


        UsedListNode* temp = runningList->head;
        SpaceListNode* slnode;

        while (temp != NULL) {
            if (temp->duration == 0) { //if process has finished
                
                UsedListNode* victim = temp;
                temp = temp->next;
                
                shared_memory_g_place("VP_STOP", victim->id, 0, 0);
                
                // while .... move processes from waiting list to runninglist
                 
                usedList_delete(runningList, victim->id); //remove from startedList                
            } else {
                temp = temp->next;
            }
        }
        
        nextArrival--;
        usleep(50000);
    }

    shared_memory_g_place("EXIT", 0, 0, 0);

    free(alg);

    shared_memory_detach();

    list_destroy(waitingList);
    usedList_destroy(runningList);

    printf("G finished with pid: %d \n", getpid());

    return 0;
}
