#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "sharedMemory.h"

int main(int argc, char *argv[])
{
  printf("I am a slave %s\n", argv[1]);

  char *sharedMem = attachMem(FILENAME, MEM_SIZE);
  if (sharedMem == NULL)
  {
    exit(1);
  }

  printf("%s\n", sharedMem);

  detachMem(sharedMem);

  return 0;
}
