/**
* Test a live shortest Job first vs a pre-determined shortest job first.
*
* Completion time: 2
*
* @author Gutierrez
* @version 1.0
*/

////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
  int id;
  int tau;
  float alpha;
  int* time;
}Process;

int ticks;
int processCount;
Process** processes;
int buff;

/**
 * sorts processes based on times at buff index
 * @param a
 * @param b
 * @return
 */
int compare (const void * a, const void * b) {
    Process* pa = *(Process**)a;
    Process* pb = *(Process**)b;
    return ( pa->time[buff] - pb->time[buff]);
}

/**
 * sorts processes based on tau (estimate) values
 * @param a
 * @param b
 * @return
 */
int compareLive (const void * a, const void * b) {
    Process* pa = *(Process**)a;
    Process* pb = *(Process**)b;
    return ( pa->tau - pb->tau);
}

/**
 * sorts processes based on process id
 * @param a
 * @param b
 * @return
 */
int resetOrder (const void * a, const void * b) {
    Process* pa = *(Process**)a;
    Process* pb = *(Process**)b;
    return ( pa->id - pb->id);
}

/**
 * runs processes when it knows the data
 */
void displayData() {
    int runningSum = 0;
    int turnAroundTime = 0;
    int waitingTime = 0;
    printf("== Shortest - Job - First ==\n");
    for (int i = 0; i < ticks; ++i) {
        printf("Simulating %dth tick of processes @ time %d:\n", i, runningSum);
        buff = i;
        int iter = processCount;
        qsort(processes, processCount, sizeof(processes), compare);
        for (int j = 0; j < processCount; ++j) {
            int time = processes[j]->time[i];
            printf("\tProcess %d took %d.\n", processes[j]->id, time);
            turnAroundTime += time * iter;
            iter--;
            waitingTime += time * iter;
            runningSum += time;
        }
    }
    printf("Turnaround time: %d\n", turnAroundTime);
    printf("Waiting time: %d\n", waitingTime);
}

/**
 * Tries to predict the upcoming time
 */
void displayDataLive(){
    int runningSum = 0;
    int turnAroundTime = 0;
    int waitingTime = 0;
    int estimationError = 0;

    printf("\n== Shortest - Job - First Live==\n");
    for (int i = 0; i < ticks; ++i) {
        printf("Simulating %dth tick of processes @ time %d:\n", i, runningSum);
        buff = i;
        int iter = processCount;
        qsort(processes, processCount, sizeof(processes), compareLive);
        for (int j = 0; j < processCount; ++j) {
            Process* p= processes[j];
            int time = p->time[i];
            printf("\tProcess %d was estimated for %d and took %d.\n", p->id, p->tau, p->time[i]);
            estimationError += abs(p->time[i] - p->tau);
            p->tau = (int)(p->alpha * (float)p->time[i] + (float)p->tau - p->alpha * (float)p->tau);
            turnAroundTime += time * iter;
            iter--;
            waitingTime += time * iter;
            runningSum += time;
        }
    }
    printf("Turnaround time: %d\n", turnAroundTime);
    printf("Waiting time: %d\n", waitingTime);
    printf("Estimation Error; %d\n", estimationError);
}

/**
 * destroys all the data memory allocated
 */
void freeData() {
    for (int i = 0; i < processCount; ++i) {
        free(processes[i]->time);
        free(processes[i]);
    }
    free(processes);
}

/**
 * reads data from the file into structs
 * @param data
 */
void readData(FILE *data) {
    fscanf(data, "%d %d", &ticks, &processCount);

    for (int i = 0; i < processCount; ++i) {
        Process* p = (Process*)malloc(sizeof(Process));
        fscanf(data, "%d %d %f", &p->id, &p->tau, &p->alpha);
        int* times = (int*) malloc(sizeof(int) * ticks);
        for (int j = 0; j < ticks; ++j) {
            fscanf(data, "%d", &times[j]);
        }
        p->time = times;
        processes[i] = p;
    }
}

/**
 * entry function to program
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Data file not provided\n");
        exit(1);
    }

    char* fileName = argv[1];
    int dflen;
    dflen = strlen(fileName);
    if(dflen < 5 || strcmp(&fileName[dflen-4], ".txt") != 0 || (access(fileName, F_OK) == -1)){
        printf("Unable to open file.\n");
        exit(1);
    }

    processes = (Process**)malloc(sizeof(Process*) * processCount);

    printf("Importing data from %s\n", fileName);
    FILE* data = fopen(fileName, "r");
    readData(data);
    fclose(data);

    displayData();
    qsort(processes, processCount, sizeof(processes), resetOrder);
    displayDataLive();
    freeData();

    return 0;
}