#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "config.h"
#include "sharedMemory.h"

void lock(int processNo, int numOfProcs) {
  int* sharedMem = attachMem(FILENAME, MEM_SIZE);
  if (sharedMem == NULL) {
    exit(1);
  }

  struct tm* timeInfo;
  time_t rawtime;
  time(&rawtime);
  timeInfo = localtime(&rawtime);

  char formattedTime[9];
  strftime(formattedTime, 9, "%X", timeInfo);

  //replace with logfile write
  printf("%s Process %d attempting to enter critical section\n", formattedTime, processNo + 1);

  sharedMem[processNo] = 1; //set choosing to true
  int maxTicket = 0;
  int i;
  for (i = 0; i < numOfProcs; i++) {
    if (sharedMem[i + numOfProcs] > maxTicket) {
      maxTicket = sharedMem[i + numOfProcs];
    }
  }
  sharedMem[processNo + numOfProcs] = maxTicket + 1;
  sharedMem[processNo] = 0; //set choosing to false

  for (i = 0; i < numOfProcs; i++) {
    int ticketNo = i + numOfProcs;
    while (sharedMem[i] == 1);
    while (sharedMem[ticketNo] != 0 && (sharedMem[ticketNo] <
      sharedMem[processNo + numOfProcs] || (sharedMem[ticketNo] ==
        sharedMem[processNo + numOfProcs] && i < processNo)));
  }

  detachMem(sharedMem);
}

void unlock(int processNo, int numOfProcs) {
  int* sharedMem = attachMem(FILENAME, MEM_SIZE);
  if (sharedMem == NULL) {
    exit(1);
  }

  struct tm* timeInfo;
  time_t rawtime;
  time(&rawtime);
  timeInfo = localtime(&rawtime);

  char formattedTime[9];
  strftime(formattedTime, 9, "%X", timeInfo);

  //replace with logfile write
  printf("%s Process %d leaving critical section\n\n", formattedTime, processNo + 1);

  sharedMem[processNo + numOfProcs] = 0;

  detachMem(sharedMem);
}

int main(int argc, char* argv[]) {
  // int* sharedMem = attachMem(FILENAME, MEM_SIZE);
  // if (sharedMem == NULL) {
  //   exit(1);
  // }
  int processNo = atoi(argv[1]);

  lock(processNo, atoi(argv[2]));
  printf("Process %d doing critical stuff\n", processNo + 1);
  unlock(processNo, atoi(argv[2]));

  // struct tm* timeInfo;
  // time_t rawtime;
  // time(&rawtime);
  // timeInfo = localtime(&rawtime);

  // char formattedTime[9];
  // strftime(formattedTime, 9, "%X", timeInfo);

  // printf("%s Queue %d File modified by process number %s\n", formattedTime, sharedMem[processNo - 1], argv[1]);

  // detachMem(sharedMem);
  destroyMem(FILENAME);

  return 0;
}
