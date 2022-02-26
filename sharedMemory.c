#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/shm.h>
#include "sharedMemory.h"

static int getSharedMem(char *fileName, int size)
{
  key_t key;
  key = ftok(fileName, 0);

  if (key == -1)
  {
    return -1;
  }

  return shmget(key, size, 0644 | IPC_CREAT);
}

char *attachMem(char *fileName, int size)
{
  int sharedId = getSharedMem(fileName, size);
  char *result;

  if (sharedId == -1)
  {
    return NULL;
  }

  result = shmat(sharedId, NULL, 0);
  if (result == (char *)-1)
  {
    return NULL;
  }

  return result;
}

bool detachMem(char *memory)
{
  return (shmdt(memory) != -1);
}

bool destroyMem(char *fileName)
{
  int sharedId = getSharedMem(fileName, 0);

  if (sharedId == -1)
  {
    return NULL;
  }

  return (shmctl(sharedId, IPC_RMID, NULL) != -1);
}