#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "config.h"
#include "sharedMemory.h"

void lock(int processNo, int numOfProcs) {
  int* choosing = attachMem(CHOOSING, MEM_SIZE);
  if (choosing == NULL) {
    exit(1);
  }
  int* ticket = attachMem(TICKET, MEM_SIZE);
  if (ticket == NULL) {
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

  choosing[processNo] = 1; //set choosing to true
  int maxTicket = 0;
  int i;
  for (i = 0; i < numOfProcs; i++) {
    if (ticket[i] > maxTicket) {
      maxTicket = ticket[i];
    }
  }
  ticket[i] = maxTicket + 1;
  choosing[processNo] = 0; //set choosing to false

  for (i = 0; i < numOfProcs; i++) {
    while (choosing[i] == 1);
    while (ticket[i] != 0 && (ticket[i] < ticket[processNo] || (ticket[i] == ticket[processNo] && i < processNo)));
  }

  detachMem(choosing);
  detachMem(ticket);
}

void unlock(int processNo, int numOfProcs) {
  int* ticket = attachMem(TICKET, MEM_SIZE);
  if (ticket == NULL) {
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

  ticket[processNo] = 0;

  detachMem(ticket);
}

int main(int argc, char* argv[]) {
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
  destroyMem(CHOOSING);
  destroyMem(TICKET);

  return 0;
}
