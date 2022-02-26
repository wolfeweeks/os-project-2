#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <stdbool.h>

char *attachMem(char *fileName, int size);

bool detachMem(char *memory);

bool destroyMem(char *fileName);

#define MEM_SIZE 4096
#define FILENAME "cstest"

#endif // !SHARED_MEMORY_H