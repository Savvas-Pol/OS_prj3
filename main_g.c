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
        printf("  --- Simulation step %d of %d  --- \n", i + 1, D);

        if (i == 0) { //beginning of simulation
            nextArrival = generatePoissonVariable(t);
            size = generateUniformVariable(lo, hi);
            duration = (int) generateExponentialVariable((double) T);
            fake_pid++;
        } else if (nextArrival == 0) { //arrival of process
            nextArrival = generatePoissonVariable(t);

            printf("[ G ] Process #%d arrived, duration:%d, estimated completion at: %d, next arrival in %d steps \n", fake_pid, duration, i + duration, nextArrival);

            shared_memory_g_place("VP_START", fake_pid, size, duration);

            shared_memory_g_obtain(result, &fake_pid, &size, &duration);

            if (strcmp(result, "STARTED") == 0) {
                printf("[ G ] STARTED result for PID: %d \n", fake_pid);
                usedList_insert(runningList, fake_pid, 0, 0, duration);
            } else if (strcmp(result, "FAILED") == 0) {
                printf("[ G ] FAILED result\n");
                list_insert(waitingList, fake_pid, size, duration, i);
            } else {
                exit(3);
            }

            size = generateUniformVariable(lo, hi);
            duration = (int) generateExponentialVariable((double) T);
            fake_pid++;
        }

        usedList_reduceDurations(runningList); //reduce duration of processes on every iteration

        UsedListNode* temp = runningList->head;
        
        while (temp != NULL) {
            if (temp->duration == 0) { //if process has finished

                UsedListNode* victim = temp;
                temp = temp->next;

                printf("[ G ] Sending VP_STOP %d to M \n", victim->id);                        
                        
                shared_memory_g_place("VP_STOP", victim->id, 0, 0);
                
                while (1) {
                    int wake_id = 0;
                    int wake_size = 0;
                    int wake_duration = 0;
                    
                    shared_memory_g_obtain(result, &wake_id, &wake_size, &wake_duration);
                    if (strcmp(result,"#") == 0) {
                        break;
                    }
                    
                    printf("[ G ] Received VP_RESUME for %d \n", wake_id);        
                    
                    shared_memory_g_place("OK", 0, 0, 0);
                    
                    list_delete(waitingList, wake_id); 
                    
                    usedList_insert(runningList, fake_pid, 0, 0, wake_duration);                    
                }

                usedList_delete(runningList, victim->id); //remove from startedList                
            } else {
                temp = temp->next;
            }
        }

        nextArrival--;
        // usleep(50000);
    }

    shared_memory_g_place("EXIT", 0, 0, 0);

    free(alg);

    shared_memory_detach();

    list_destroy(waitingList);
    usedList_destroy(runningList);

    printf("G finished with pid: %d \n", getpid());

    return 0;
}
