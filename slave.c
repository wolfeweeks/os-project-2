#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include "config.h"
#include "sharedMemory.h"

/**
 * @brief wait until process' turn (using bakery algorithm)
 *        to enter a critical section
 *
 * @param processNo the number of this process
 * @param numOfProcs the total number of processes
 * @param logfile the logfile for this process to write to
 * @return ** int the ticket number the process recieved
 */
int lock(int processNo, int numOfProcs, FILE* logfile) {
  //attach shared memory for choosing booleans and ticket numbers
  int* choosing = attachMem(CHOOSING, MEM_SIZE);
  if (choosing == NULL) {
    exit(1);
  }
  int* ticket = attachMem(TICKET, MEM_SIZE);
  if (ticket == NULL) {
    exit(1);
  }

  //get time and format it according to HH:MM:SS
  struct tm* timeInfo;
  time_t rawtime;
  time(&rawtime);
  timeInfo = localtime(&rawtime);
  char formattedTime[9];
  strftime(formattedTime, 9, "%X", timeInfo);

  //write log to logfile
  fprintf(logfile, "%s Process %d attempting to enter critical section\n",
    formattedTime, processNo);

  //bakery algorithm
  choosing[processNo] = 1; //set this process' choosing to true
  int maxTicket = 0;
  int j;

  //set this process' ticket to be the max of all other processes
  for (j = 0; j < numOfProcs; j++) {
    if (ticket[j] > maxTicket) {
      maxTicket = ticket[j];
    }
  }
  ticket[processNo] = maxTicket + 1;

  choosing[processNo] = 0; //set this process' choosing to false

  //wait until each process with smaller ticket numbers is finished
  for (j = 0; j < numOfProcs; j++) {
    while (choosing[j] == 1);
    while (ticket[j] != 0 && (ticket[j] < ticket[processNo] || (ticket[j]
      == ticket[processNo] && j < processNo)));
  }

  //detach shared memory
  detachMem(choosing);
  detachMem(ticket);

  return maxTicket + 1; //return the ticket this process chose
}

/**
 * @brief reset this process' ticket to 0, allowing other processes to continue
 *
 * @param processNo the number of this process
 * @param logfile the logfile for this process to write to
 * @return ** void
 */
void unlock(int processNo, FILE* logfile) {
  //attach shared memory for ticket numbers
  int* ticket = attachMem(TICKET, MEM_SIZE);
  if (ticket == NULL) {
    exit(1);
  }

  //get time and format it according to HH:MM:SS
  struct tm* timeInfo;
  time_t rawtime;
  time(&rawtime);
  timeInfo = localtime(&rawtime);
  char formattedTime[9];
  strftime(formattedTime, 9, "%X", timeInfo);

  //write log to logfile
  fprintf(logfile, "%s Process %d leaving critical section\n\n", formattedTime,
    processNo);

  ticket[processNo] = 0; //reset this process' ticket to 0

  detachMem(ticket); //detach shared memory
}

int main(int argc, char* argv[]) {
  int processNo = atoi(argv[1]); //convert this process' number to integer

  int sizeOfLogfileName = sizeof(char)
    * (strlen("logfile." + strlen(argv[1]) + 1));
  char* logfileName = (char*)malloc(sizeOfLogfileName);
  strcat(logfileName, "logfile.");
  // realloc(logfileName, ((strlen(logfileName) + 1) + strlen(argv[1])) * sizeof(char));
  strcat(logfileName, argv[1]);

  FILE* logfile = NULL;
  logfile = fopen(logfileName, "w");

  int ticketNo = lock(processNo, atoi(argv[2]), logfile);
  //critical section************************************************************
  sleep(2);

  //get time and format it according to HH:MM:SS
  struct tm* timeInfo;
  time_t rawtime;
  time(&rawtime);
  timeInfo = localtime(&rawtime);
  char formattedTime[9];
  strftime(formattedTime, 9, "%X", timeInfo);

  //open cstest in append mode, write a log to it, and close it
  FILE* file = NULL;
  file = fopen("cstest", "a");
  fprintf(file, "%s Queue %d File modified by process number %d\n",
    formattedTime, ticketNo, processNo);
  fclose(file);

  sleep(2);
  //critical section************************************************************
  unlock(processNo, logfile);

  fclose(logfile);

  return 0;
}
