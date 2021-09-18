#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#include "helpers.h"
#include "usedList.h"
#include "spaceList.h"
#include "list.h"
#include "shared_memory.h"

int main_g(int argc, char** argv);
int main_m(int argc, char** argv);

int main(int argc, char** argv) {    
    shared_memory_init();
    
    pid_t pid = fork();
    
    if (pid == 0) {
        int out_m = main_m(argc, argv);

        return out_m;
    } else {
        int out_g = main_g(argc, argv);

        waitpid(pid, 0, 0);
                
        shared_memory_destroy();

        return out_g;
    }
}
