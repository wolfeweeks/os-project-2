/**
 * @file master.c
 * @author Wolfe Weeks
 * @date 2022-02-27
 */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h> //for getopt variables (e.g. opterr, optopt, etc.)
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include "config.h"
#include "sharedMemory.h"

void handleInterrupt(int sig) {
  destroyMem(CHOOSING);
  destroyMem(TICKET);
  kill(0, SIGKILL);
  exit(1);
}

int main(int argc, char* argv[]) {
  signal(SIGALRM, handleInterrupt);
  signal(SIGINT, handleInterrupt);

  int numOfProcs;
  int maxTime = 100;

  int options = 0;

  // read through the user's command line options and set any necessary args
  while (options != -1) {
    options = getopt(argc, argv, "ht:");
    switch (options) {
    case 'h':
      printf("usage: master [-t maxSeconds] numOfProcs\n");
      return 0;
    case 't':
      maxTime = atoi(optarg);
      break;
    case ':':
      printf("Option needs a value\n");
      return 1;
    case '?':
      printf("%c is not an option\n", optopt);
      return 1;
    default:
      break;
    }
  }

  if (argc < 2 || argc > 4) // ensure proper number of args
  {
    fprintf(stderr, "usage: master [-t maxSeconds] numOfProcs\n");
    exit(1);
  }

  // set number of processes to the user entered amount or a max of 18
  if (argc == 2) {
    if (atoi(argv[1]) > MAX_CHILDREN) {
      numOfProcs = MAX_CHILDREN;
    } else {
      numOfProcs = atoi(argv[1]);
    }
  } else {
    if (atoi(argv[3]) > MAX_CHILDREN) {
      numOfProcs = MAX_CHILDREN;
    } else {
      numOfProcs = atoi(argv[3]);
    }
  }

  int* choosing = attachMem(CHOOSING, MEM_SIZE);
  if (choosing == NULL) {
    exit(1);
  }
  int* ticket = attachMem(TICKET, MEM_SIZE);
  if (ticket == NULL) {
    exit(1);
  }

  int i;
  for (i = 0; i < numOfProcs; i++) {
    choosing[i] = 0;
    ticket[i] = 0;
  }

  detachMem(choosing);
  detachMem(ticket);

  alarm(maxTime);

  FILE* file = NULL;
  file = fopen("cstest", "w");
  fclose(file);

  pid_t childPid = 0;
  int status = 0;

  for (i = 0; i < numOfProcs; i++) {
    if (childPid = fork() == 0) {
      char processNo[3];
      sprintf(processNo, "%d", i);
      char numOfProcsString[3];
      sprintf(numOfProcsString, "%d", numOfProcs);

      execl("./slave", "./slave", processNo, numOfProcsString, NULL);
    } else if (childPid == -1) {
      break;
    }
  }

  while (wait(&status) > 0);

  detachMem(choosing);
  detachMem(ticket);
  destroyMem(CHOOSING);
  destroyMem(TICKET);

  return 0;
}
